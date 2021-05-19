## **libx264主干接口代码调用**
> **[雷神](https://blog.csdn.net/leixiaohua1020/article/details/45644367)**

### **x264_encoder_open()** 
- x264_sps_init()：根据输入参数生成H.264码流的SPS信息。
- x264_pps_init()：根据输入参数生成H.264码流的PPS信息。
- x264_predict_16x16_init()：初始化Intra16x16帧内预测汇编函数。[扫描方式]
- x264_predict_4x4_init()：初始化Intra4x4帧内预测汇编函数。
- x264_pixel_init()：初始化像素值计算相关的汇编函数（包括SAD、SATD、SSD等）。
    ```sh
    # 像素计算中的概念
    # SAD和SATD主要用于帧内预测模式以及帧间预测模式的判断。有关SAD、SATD、SSD的定义如下：
    # 1. SAD（Sum of Absolute Difference）也可以称为SAE（Sum of Absolute Error），即绝对误差和。
    #       它的计算方法就是求出两个像素块对应像素点的差值，将这些差值分别求绝对值之后再进行累加。
    # 2. SATD（Sum of Absolute Transformed Difference）即Hadamard变换后再绝对值求和。
    #       它和SAD的区别在于多了一个“变换”。
    # 3. SSD（Sum of Squared Difference）也可以称为SSE（Sum of Squared Error），即差值的平方和。
    #       它和SAD的区别在于多了一个“平方”。
    #
    # H.264中使用SAD和SATD进行宏块预测模式的判断。早期的编码器使用SAD进行计算，近期的编码器多使用SATD进行计算。
    # 为什么使用SATD而不使用SAD呢？
    # 关键原因在于编码之后码流的大小是和图像块DCT变换后频域信息紧密相关的，而和变换前的时域信息关联性小一些。
    # SAD只能反应时域信息；SATD却可以反映频域信息，而且计算复杂度也低于DCT变换，因此是比较合适的模式选择的依据。
    #
    # 使用SAD进行模式选择。使用Vertical，Horizontal，DC和Plane四种帧内预测模式预测的像素。
    # 通过计算可以得到这几种预测像素和原始像素之间的SAD（SAE）分别为3985，5097，4991，2539。
    # 由于Plane模式的SAD取值最小，由此可以断定Plane模式对于这个宏块来说是最好的帧内预测模式。
    #
    ```
- x264_dct_init()：初始化DCT变换和DCT反变换相关的汇编函数。
    ```sh
    # DCT变换的核心理念
    # 1. 把图像的低频信息（对应大面积平坦区域）变换到系数矩阵的左上角
    # 2. 把高频信息变换到系数矩阵的右下角
    # 3. 这样就可以在压缩的时候（量化）去除掉人眼不敏感的高频信息（位于矩阵右下角的系数）从而达到压缩数据的目的。
    ```
- x264_mc_init()：初始化运动补偿相关的汇编函数。
    ```sh
    # x264_mc_init()中包含了大量的像素内插、拷贝、求平均的函数。这些函数都是用于在H.264编码过程中进行运动估计和运动补偿的。# x264_mc_init()的参数x264_mc_functions_t是一个结构体，其中包含了运动补偿函数相关的函数接口。
    ```
- x264_quant_init()：初始化量化和反量化相关的汇编函数。
    ```sh
    #
    # A. 量化是H.264视频压缩编码中对视频质量影响最大的地方，也是会导致“信息丢失”的地方。
    #
    # B. Qstep越大，视频压缩编码后体积越小，视频质量越差。
    #       量化：FQ=round(y/Qstep)
    #     反量化：y’＝FQ*Qstep
    #
    # C. 《H.264标准》中规定，量化过程除了完成本职工作外，还需要完成它前一步DCT变换中“系数相乘”的工作。
    #       |Zij| = (|Wij|*MF + f)>>qbits
    #       sign(Zij) = sign (Wij)
    #
    #    sign()为符号函数。 Wij为DCT变换后的系数。
    #    MF的值需要查表。表中只列出对应QP 值为0 到5 的MF 值。QP大于6之后，将QP实行对6取余数操作，再找到MF的值。
    #    qbits计算公式为“qbits = 15 + floor(QP/6)”。即它的值随QP 值每增加6 而增加1。
    #    f 是偏移量（用于改善恢复图像的视觉效果）。对帧内预测图像块取2^qbits/3，对帧间预测图像块取2^qbits/6。
    ```
- x264_deblock_init()：初始化去块效应滤波器相关的汇编函数。
    ```sh
    # 原因： 
    # 1. DCT量化
    # 2. 运动补偿
    #
    # 环路滤波分类
    # 1. 环路滤波器根据滤波的强度可以分为两种：
    #   普通滤波器。针对边界的Bs（边界强度）为1、2、3的滤波器。
    #   此时环路滤波涉及到方块边界周围的6个点（边界两边各3个点）：p2，p1，p0，q0，q1，q2。需要处理4个点（边界两边各2个点，只以p点为例）：
    #       p0’ = p0 + (((q0 - p0 ) << 2) + (p1 - q1) + 4) >> 3
    #       p1’ = ( p2 + ( ( p0 + q0 + 1 ) >> 1) – 2p1 ) >> 1
    #   强滤波器。针对边界的Bs（边界强度）为4的滤波器。
    #   此时环路滤波涉及到方块边界周围的8个点（边界两边各4个点）：p3，p2，p1，p0，q0，q1，q2，q3。需要处理6个点（边界两边各3个点，只以p点为例）：
    #       p0’ = ( p2 + 2*p1 + 2*p0 + 2*q0 + q1 + 4 ) >> 3
    #       p1’ = ( p2 + p1 + p0 + q0 + 2 ) >> 2
    #       p2’ = ( 2*p3 + 3*p2 + p1 + p0 + q0 + 4 ) >> 3
    #
    # 环路滤波的门限
    #   1. 不是所有的块的边界处都需要环路滤波。例如画面中物体的边界正好和块的边界重合的话，就不能进行滤波，否则会使画面中物体的边界变模糊。因此需要区别开物体边界和块效应边界。
    #   2. 一般情况下，物体边界两边的像素值差别很大，而块效应边界两边像素值差别比较小。《H.264标准》以这个特点定义了2个变量alpha和beta来判决边界是否需要进行环路滤波。
    #   3. 只有满足下面三个条件的时候才能进行环路滤波：
    #       | p0 - q0 | < alpha
    #       | p1 – p0 | < beta
    #       | q1 - q0 | < beta
    #
    #   简而言之，就是边界两边的两个点的像素值不能太大，即不能超过alpha；
    #   边界一边的前两个点之间的像素值也不能太大，即不能超过beta。其中alpha和beta是根据量化参数QP推算出来（具体方法不再记录）。
    #   总体说来QP越大，alpha和beta的值也越大，也就越容易触发环路滤波。由于QP越大表明压缩的程度越大，所以也可以得知高压缩比的情况下更需要进行环路滤波。
    #
    # 边界强度Bs的判定方式：
    #   ------------------------------------------------------------------
    #   条件（针对两边的图像块）                         Bs
    #   ------------------------------------------------------------------
    #   有一个块为帧内预测 + 边界为宏块边界               4
    #   有一个块为帧内预测                               3
    #   有一个块对残差编码                               2
    #   运动矢量差不小于1像素                            1
    #   运动补偿参考帧不同                               1
    #   其它                                            0
    #   ------------------------------------------------------------------
    #
	# 方块的横向边界的滤波，即如下所示：
	#          p2
	#          p1
	#          p0
	#   =====图像边界=====
	#          q0
	#          q1
	#          q2
	#
	# 对应的是方块的横向边界的滤波，即如下所示：
	#            ||
	#   p2 p1 p0 || q0 q1 q2
	#            ||
    ```
- x264_ratecontrol_new()：初始化码率控制相关的变量。
- mbcmp_init()：决定像素比较的时候使用SAD还是SATD。
    ```c
    //b_lossless一般为0
    //主要看i_subpel_refine，大于1的话就使用SATD
    int satd = !h->mb.b_lossless && h->param.analyse.i_subpel_refine > 1;
    ```

### **x264_encoder_headers()**
> 在输出每个NALU之前，需要调用x264_nal_start()，在输出NALU之后，需要调用x264_nal_end()。
- x264_sps_write()
```c
// (2条消息) x264源代码简单分析：编码器主干部分-1_雷霄骅(leixiaohua1020)的专栏-CSDN博客
// 输出SPS
void x264_sps_write( bs_t *s, x264_sps_t *sps )
{
    bs_realign( s );
    //型profile，8bit
    bs_write( s, 8, sps->i_profile_idc );
    bs_write1( s, sps->b_constraint_set0 );
    bs_write1( s, sps->b_constraint_set1 );
    bs_write1( s, sps->b_constraint_set2 );
    bs_write1( s, sps->b_constraint_set3 );
 
    bs_write( s, 4, 0 );    /* reserved */
    //级level，8bit
    bs_write( s, 8, sps->i_level_idc );
    //本SPS的 id号
    bs_write_ue( s, sps->i_id );
 
    if( sps->i_profile_idc >= PROFILE_HIGH )
    {
    	//色度取样格式
		//0代表单色
		//1代表4:2:0
		//2代表4:2:2
		//3代表4:4:4
        bs_write_ue( s, sps->i_chroma_format_idc );
        if( sps->i_chroma_format_idc == CHROMA_444 )
            bs_write1( s, 0 ); // separate_colour_plane_flag
        //亮度
        //颜色位深=bit_depth_luma_minus8+8
        bs_write_ue( s, BIT_DEPTH-8 ); // bit_depth_luma_minus8
        //色度与亮度一样
        bs_write_ue( s, BIT_DEPTH-8 ); // bit_depth_chroma_minus8
        bs_write1( s, sps->b_qpprime_y_zero_transform_bypass );
        bs_write1( s, 0 ); // seq_scaling_matrix_present_flag
    }
    //log2_max_frame_num_minus4主要是为读取另一个句法元素frame_num服务的
    //frame_num 是最重要的句法元素之一
    //这个句法元素指明了frame_num的所能达到的最大值：
    //MaxFrameNum = 2^( log2_max_frame_num_minus4 + 4 )
    bs_write_ue( s, sps->i_log2_max_frame_num - 4 );
    //pic_order_cnt_type 指明了poc (picture order count) 的编码方法
    //poc标识图像的播放顺序。
    //由于H.264使用了B帧预测，使得图像的解码顺序并不一定等于播放顺序，但它们之间存在一定的映射关系
    //poc 可以由frame-num 通过映射关系计算得来，也可以索性由编码器显式地传送。
    //H.264 中一共定义了三种poc 的编码方法
    bs_write_ue( s, sps->i_poc_type );
    if( sps->i_poc_type == 0 )
        bs_write_ue( s, sps->i_log2_max_poc_lsb - 4 );
    //num_ref_frames 指定参考帧队列可能达到的最大长度，解码器依照这个句法元素的值开辟存储区，这个存储区用于存放已解码的参考帧，
    //H.264 规定最多可用16 个参考帧，因此最大值为16。
    bs_write_ue( s, sps->i_num_ref_frames );
    bs_write1( s, sps->b_gaps_in_frame_num_value_allowed );
    //pic_width_in_mbs_minus1加1后为图像宽（以宏块为单位）：
    //           PicWidthInMbs = pic_width_in_mbs_minus1 + 1
    //以像素为单位图像宽度（亮度）：width=PicWidthInMbs*16
    bs_write_ue( s, sps->i_mb_width - 1 );
    //pic_height_in_map_units_minus1加1后指明图像高度（以宏块为单位）
    bs_write_ue( s, (sps->i_mb_height >> !sps->b_frame_mbs_only) - 1);
    bs_write1( s, sps->b_frame_mbs_only );
    if( !sps->b_frame_mbs_only )
        bs_write1( s, sps->b_mb_adaptive_frame_field );
    bs_write1( s, sps->b_direct8x8_inference );
 
    bs_write1( s, sps->b_crop );
    if( sps->b_crop )
    {
        int h_shift = sps->i_chroma_format_idc == CHROMA_420 || sps->i_chroma_format_idc == CHROMA_422;
        int v_shift = sps->i_chroma_format_idc == CHROMA_420;
        bs_write_ue( s, sps->crop.i_left   >> h_shift );
        bs_write_ue( s, sps->crop.i_right  >> h_shift );
        bs_write_ue( s, sps->crop.i_top    >> v_shift );
        bs_write_ue( s, sps->crop.i_bottom >> v_shift );
    }
 
    bs_write1( s, sps->b_vui );
    if( sps->b_vui )
    {
        bs_write1( s, sps->vui.b_aspect_ratio_info_present );
        if( sps->vui.b_aspect_ratio_info_present )
        {
            int i;
            static const struct { uint8_t w, h, sar; } sar[] =
            {
                // aspect_ratio_idc = 0 -> unspecified
                {  1,  1, 1 }, { 12, 11, 2 }, { 10, 11, 3 }, { 16, 11, 4 },
                { 40, 33, 5 }, { 24, 11, 6 }, { 20, 11, 7 }, { 32, 11, 8 },
                { 80, 33, 9 }, { 18, 11, 10}, { 15, 11, 11}, { 64, 33, 12},
                {160, 99, 13}, {  4,  3, 14}, {  3,  2, 15}, {  2,  1, 16},
                // aspect_ratio_idc = [17..254] -> reserved
                { 0, 0, 255 }
            };
            for( i = 0; sar[i].sar != 255; i++ )
            {
                if( sar[i].w == sps->vui.i_sar_width &&
                    sar[i].h == sps->vui.i_sar_height )
                    break;
            }
            bs_write( s, 8, sar[i].sar );
            if( sar[i].sar == 255 ) /* aspect_ratio_idc (extended) */
            {
                bs_write( s, 16, sps->vui.i_sar_width );
                bs_write( s, 16, sps->vui.i_sar_height );
            }
        }
 
        bs_write1( s, sps->vui.b_overscan_info_present );
        if( sps->vui.b_overscan_info_present )
            bs_write1( s, sps->vui.b_overscan_info );
 
        bs_write1( s, sps->vui.b_signal_type_present );
        if( sps->vui.b_signal_type_present )
        {
            bs_write( s, 3, sps->vui.i_vidformat );
            bs_write1( s, sps->vui.b_fullrange );
            bs_write1( s, sps->vui.b_color_description_present );
            if( sps->vui.b_color_description_present )
            {
                bs_write( s, 8, sps->vui.i_colorprim );
                bs_write( s, 8, sps->vui.i_transfer );
                bs_write( s, 8, sps->vui.i_colmatrix );
            }
        }
 
        bs_write1( s, sps->vui.b_chroma_loc_info_present );
        if( sps->vui.b_chroma_loc_info_present )
        {
            bs_write_ue( s, sps->vui.i_chroma_loc_top );
            bs_write_ue( s, sps->vui.i_chroma_loc_bottom );
        }
 
        bs_write1( s, sps->vui.b_timing_info_present );
        if( sps->vui.b_timing_info_present )
        {
            bs_write32( s, sps->vui.i_num_units_in_tick );
            bs_write32( s, sps->vui.i_time_scale );
            bs_write1( s, sps->vui.b_fixed_frame_rate );
        }
 
        bs_write1( s, sps->vui.b_nal_hrd_parameters_present );
        if( sps->vui.b_nal_hrd_parameters_present )
        {
            bs_write_ue( s, sps->vui.hrd.i_cpb_cnt - 1 );
            bs_write( s, 4, sps->vui.hrd.i_bit_rate_scale );
            bs_write( s, 4, sps->vui.hrd.i_cpb_size_scale );
 
            bs_write_ue( s, sps->vui.hrd.i_bit_rate_value - 1 );
            bs_write_ue( s, sps->vui.hrd.i_cpb_size_value - 1 );
 
            bs_write1( s, sps->vui.hrd.b_cbr_hrd );
 
            bs_write( s, 5, sps->vui.hrd.i_initial_cpb_removal_delay_length - 1 );
            bs_write( s, 5, sps->vui.hrd.i_cpb_removal_delay_length - 1 );
            bs_write( s, 5, sps->vui.hrd.i_dpb_output_delay_length - 1 );
            bs_write( s, 5, sps->vui.hrd.i_time_offset_length );
        }
 
        bs_write1( s, sps->vui.b_vcl_hrd_parameters_present );
 
        if( sps->vui.b_nal_hrd_parameters_present || sps->vui.b_vcl_hrd_parameters_present )
            bs_write1( s, 0 );   /* low_delay_hrd_flag */
 
        bs_write1( s, sps->vui.b_pic_struct_present );
        bs_write1( s, sps->vui.b_bitstream_restriction );
        if( sps->vui.b_bitstream_restriction )
        {
            bs_write1( s, sps->vui.b_motion_vectors_over_pic_boundaries );
            bs_write_ue( s, sps->vui.i_max_bytes_per_pic_denom );
            bs_write_ue( s, sps->vui.i_max_bits_per_mb_denom );
            bs_write_ue( s, sps->vui.i_log2_max_mv_length_horizontal );
            bs_write_ue( s, sps->vui.i_log2_max_mv_length_vertical );
            bs_write_ue( s, sps->vui.i_num_reorder_frames );
            bs_write_ue( s, sps->vui.i_max_dec_frame_buffering );
        }
    }
 
    //RBSP拖尾
    //无论比特流当前位置是否字节对齐 ， 都向其中写入一个比特1及若干个（0~7个）比特0 ， 使其字节对齐
    bs_rbsp_trailing( s );
    bs_flush( s );
}
```
- x264_pps_write()
```c
// x264_pps_write()将x264_pps_t结构体中的信息输出出来形成了一个NALU。
//输出PPS
void x264_pps_write( bs_t *s, x264_sps_t *sps, x264_pps_t *pps )
{
    bs_realign( s );
    //PPS的ID
    bs_write_ue( s, pps->i_id );
    //该PPS引用的SPS的ID
    bs_write_ue( s, pps->i_sps_id );
    //entropy_coding_mode_flag
    //0表示熵编码使用CAVLC，1表示熵编码使用CABAC
    bs_write1( s, pps->b_cabac );
    bs_write1( s, pps->b_pic_order );
    bs_write_ue( s, pps->i_num_slice_groups - 1 );
 
    bs_write_ue( s, pps->i_num_ref_idx_l0_default_active - 1 );
    bs_write_ue( s, pps->i_num_ref_idx_l1_default_active - 1 );
    //P Slice 是否使用加权预测？
    bs_write1( s, pps->b_weighted_pred );
    //B Slice 是否使用加权预测？
    bs_write( s, 2, pps->b_weighted_bipred );
    //pic_init_qp_minus26加26后用以指明亮度分量的QP的初始值。
    bs_write_se( s, pps->i_pic_init_qp - 26 - QP_BD_OFFSET );
    bs_write_se( s, pps->i_pic_init_qs - 26 - QP_BD_OFFSET );
    bs_write_se( s, pps->i_chroma_qp_index_offset );
 
    bs_write1( s, pps->b_deblocking_filter_control );
    bs_write1( s, pps->b_constrained_intra_pred );
    bs_write1( s, pps->b_redundant_pic_cnt );
 
    if( pps->b_transform_8x8_mode || pps->i_cqm_preset != X264_CQM_FLAT )
    {
        bs_write1( s, pps->b_transform_8x8_mode );
        bs_write1( s, (pps->i_cqm_preset != X264_CQM_FLAT) );
        if( pps->i_cqm_preset != X264_CQM_FLAT )
        {
            scaling_list_write( s, pps, CQM_4IY );
            scaling_list_write( s, pps, CQM_4IC );
            bs_write1( s, 0 ); // Cr = Cb
            scaling_list_write( s, pps, CQM_4PY );
            scaling_list_write( s, pps, CQM_4PC );
            bs_write1( s, 0 ); // Cr = Cb
            if( pps->b_transform_8x8_mode )
            {
                if( sps->i_chroma_format_idc == CHROMA_444 )
                {
                    scaling_list_write( s, pps, CQM_8IY+4 );
                    scaling_list_write( s, pps, CQM_8IC+4 );
                    bs_write1( s, 0 ); // Cr = Cb
                    scaling_list_write( s, pps, CQM_8PY+4 );
                    scaling_list_write( s, pps, CQM_8PC+4 );
                    bs_write1( s, 0 ); // Cr = Cb
                }
                else
                {
                    scaling_list_write( s, pps, CQM_8IY+4 );
                    scaling_list_write( s, pps, CQM_8PY+4 );
                }
            }
        }
        bs_write_se( s, pps->i_chroma_qp_index_offset );
    }
 
    //RBSP拖尾
    //无论比特流当前位置是否字节对齐 ， 都向其中写入一个比特1及若干个（0~7个）比特0 ， 使其字节对齐
    bs_rbsp_trailing( s );
    bs_flush( s );
}
```
- x264_sei_version_write()
```c
// 从源代码可以看出，x264_sei_version_write()首先调用了x264_param2string()将当前的配置参数保存到字符串opts[]中，
// 然后调用sprintf()结合opt[]生成完整的SEI信息，最后调用x264_sei_write()输出SEI信息。
// 在这个过程中涉及到一个libx264的API函数x264_param2string()。
// 输出SEI（其中包含了配置信息）
int x264_sei_version_write( x264_t *h, bs_t *s )
{
    // random ID number generated according to ISO-11578
    static const uint8_t uuid[16] =
    {
        0xdc, 0x45, 0xe9, 0xbd, 0xe6, 0xd9, 0x48, 0xb7,
        0x96, 0x2c, 0xd8, 0x20, 0xd9, 0x23, 0xee, 0xef
    };
    //把设置信息转换为字符串
    char *opts = x264_param2string( &h->param, 0 );
    char *payload;
    int length;
 
    if( !opts )
        return -1;
    CHECKED_MALLOC( payload, 200 + strlen( opts ) );
 
    memcpy( payload, uuid, 16 );
    //配置信息的内容
    //opts字符串内容还是挺多的
    sprintf( payload+16, "x264 - core %d%s - H.264/MPEG-4 AVC codec - "
             "Copy%s 2003-2014 - http://www.videolan.org/x264.html - options: %s",
             X264_BUILD, X264_VERSION, HAVE_GPL?"left":"right", opts );
    length = strlen(payload)+1;
    //输出SEI
    //数据类型为USER_DATA_UNREGISTERED
    x264_sei_write( s, (uint8_t *)payload, length, SEI_USER_DATA_UNREGISTERED );
 
    x264_free( opts );
    x264_free( payload );
    return 0;
fail:
    x264_free( opts );
    return -1;
}
```
### **x264_encoder_close**
```c
/****************************************************************************
 * x264_encoder_close:
 * 注释和处理：雷霄骅
 * http://blog.csdn.net/leixiaohua1020
 * leixiaohua1020@126.com
 ****************************************************************************/
void    x264_encoder_close  ( x264_t *h )
{
    int64_t i_yuv_size = FRAME_SIZE( h->param.i_width * h->param.i_height );
    int64_t i_mb_count_size[2][7] = {{0}};
    char buf[200];
    int b_print_pcm = h->stat.i_mb_count[SLICE_TYPE_I][I_PCM]
                   || h->stat.i_mb_count[SLICE_TYPE_P][I_PCM]
                   || h->stat.i_mb_count[SLICE_TYPE_B][I_PCM];
 
    x264_lookahead_delete( h );
 
#if HAVE_OPENCL
    x264_opencl_lookahead_delete( h );
    x264_opencl_function_t *ocl = h->opencl.ocl;
#endif
 
    if( h->param.b_sliced_threads )
        x264_threadpool_wait_all( h );
    if( h->param.i_threads > 1 )
        x264_threadpool_delete( h->threadpool );
    if( h->param.i_lookahead_threads > 1 )
        x264_threadpool_delete( h->lookaheadpool );
    if( h->i_thread_frames > 1 )
    {
        for( int i = 0; i < h->i_thread_frames; i++ )
            if( h->thread[i]->b_thread_active )
            {
                assert( h->thread[i]->fenc->i_reference_count == 1 );
                x264_frame_delete( h->thread[i]->fenc );
            }
 
        x264_t *thread_prev = h->thread[h->i_thread_phase];
        x264_thread_sync_ratecontrol( h, thread_prev, h );
        x264_thread_sync_ratecontrol( thread_prev, thread_prev, h );
        h->i_frame = thread_prev->i_frame + 1 - h->i_thread_frames;
    }
    h->i_frame++;
 
    /*
     * x264控制台输出示例
     *
     * x264 [info]: using cpu capabilities: MMX2 SSE2Fast SSSE3 SSE4.2 AVX
     * x264 [info]: profile High, level 2.1
     * x264 [info]: frame I:2     Avg QP:20.51  size: 20184  PSNR Mean Y:45.32 U:47.54 V:47.62 Avg:45.94 Global:45.52
     * x264 [info]: frame P:33    Avg QP:23.08  size:  3230  PSNR Mean Y:43.23 U:47.06 V:46.87 Avg:44.15 Global:44.00
     * x264 [info]: frame B:65    Avg QP:27.87  size:   352  PSNR Mean Y:42.76 U:47.21 V:47.05 Avg:43.79 Global:43.65
     * x264 [info]: consecutive B-frames:  3.0% 10.0% 63.0% 24.0%
     * x264 [info]: mb I  I16..4: 15.3% 37.5% 47.3%
     * x264 [info]: mb P  I16..4:  0.6%  0.4%  0.2%  P16..4: 34.6% 21.2% 12.7%  0.0%  0.0%    skip:30.4%
     * x264 [info]: mb B  I16..4:  0.0%  0.0%  0.0%  B16..8: 21.2%  4.1%  0.7%  direct: 0.8%  skip:73.1%  L0:28.7% L1:53.0% BI:18.3%
     * x264 [info]: 8x8 transform intra:37.1% inter:51.0%
     * x264 [info]: coded y,uvDC,uvAC intra: 74.1% 83.3% 58.9% inter: 10.4% 6.6% 0.4%
     * x264 [info]: i16 v,h,dc,p: 21% 25%  7% 48%
     * x264 [info]: i8 v,h,dc,ddl,ddr,vr,hd,vl,hu: 25% 23% 13%  6%  5%  5%  6%  8% 10%
     * x264 [info]: i4 v,h,dc,ddl,ddr,vr,hd,vl,hu: 22% 20%  9%  7%  7%  8%  8%  7% 12%
     * x264 [info]: i8c dc,h,v,p: 43% 20% 27% 10%
     * x264 [info]: Weighted P-Frames: Y:0.0% UV:0.0%
     * x264 [info]: ref P L0: 62.5% 19.7% 13.8%  4.0%
     * x264 [info]: ref B L0: 88.8%  9.4%  1.9%
     * x264 [info]: ref B L1: 92.6%  7.4%
     * x264 [info]: PSNR Mean Y:42.967 U:47.163 V:47.000 Avg:43.950 Global:43.796 kb/s:339.67
     *
     * encoded 100 frames, 178.25 fps, 339.67 kb/s
     *
     */
 
    /* Slices used and PSNR */
    /* 示例
     * x264 [info]: frame I:2     Avg QP:20.51  size: 20184  PSNR Mean Y:45.32 U:47.54 V:47.62 Avg:45.94 Global:45.52
     * x264 [info]: frame P:33    Avg QP:23.08  size:  3230  PSNR Mean Y:43.23 U:47.06 V:46.87 Avg:44.15 Global:44.00
     * x264 [info]: frame B:65    Avg QP:27.87  size:   352  PSNR Mean Y:42.76 U:47.21 V:47.05 Avg:43.79 Global:43.65
     */
    for( int i = 0; i < 3; i++ )
    {
        static const uint8_t slice_order[] = { SLICE_TYPE_I, SLICE_TYPE_P, SLICE_TYPE_B };
        int i_slice = slice_order[i];
 
        if( h->stat.i_frame_count[i_slice] > 0 )
        {
            int i_count = h->stat.i_frame_count[i_slice];
            double dur =  h->stat.f_frame_duration[i_slice];
            if( h->param.analyse.b_psnr )
            {
            	//输出统计信息-包含PSNR
            	//注意PSNR都是通过SSD换算过来的，换算方法就是调用x264_psnr()方法
                x264_log( h, X264_LOG_INFO,
                          "frame %c:%-5d Avg QP:%5.2f  size:%6.0f  PSNR Mean Y:%5.2f U:%5.2f V:%5.2f Avg:%5.2f Global:%5.2f\n",
                          slice_type_to_char[i_slice],
                          i_count,
                          h->stat.f_frame_qp[i_slice] / i_count,
                          (double)h->stat.i_frame_size[i_slice] / i_count,
                          h->stat.f_psnr_mean_y[i_slice] / dur, h->stat.f_psnr_mean_u[i_slice] / dur, h->stat.f_psnr_mean_v[i_slice] / dur,
                          h->stat.f_psnr_average[i_slice] / dur,
                          x264_psnr( h->stat.f_ssd_global[i_slice], dur * i_yuv_size ) );
            }
            else
            {
            	//输出统计信息-不包含PSNR
                x264_log( h, X264_LOG_INFO,
                          "frame %c:%-5d Avg QP:%5.2f  size:%6.0f\n",
                          slice_type_to_char[i_slice],
                          i_count,
                          h->stat.f_frame_qp[i_slice] / i_count,
                          (double)h->stat.i_frame_size[i_slice] / i_count );
            }
        }
    }
    /* 示例
     * x264 [info]: consecutive B-frames:  3.0% 10.0% 63.0% 24.0%
     *
     */
    if( h->param.i_bframe && h->stat.i_frame_count[SLICE_TYPE_B] )
    {
    	//B帧相关信息
        char *p = buf;
        int den = 0;
        // weight by number of frames (including the I/P-frames) that are in a sequence of N B-frames
        for( int i = 0; i <= h->param.i_bframe; i++ )
            den += (i+1) * h->stat.i_consecutive_bframes[i];
        for( int i = 0; i <= h->param.i_bframe; i++ )
            p += sprintf( p, " %4.1f%%", 100. * (i+1) * h->stat.i_consecutive_bframes[i] / den );
        x264_log( h, X264_LOG_INFO, "consecutive B-frames:%s\n", buf );
    }
 
    for( int i_type = 0; i_type < 2; i_type++ )
        for( int i = 0; i < X264_PARTTYPE_MAX; i++ )
        {
            if( i == D_DIRECT_8x8 ) continue; /* direct is counted as its own type */
            i_mb_count_size[i_type][x264_mb_partition_pixel_table[i]] += h->stat.i_mb_partition[i_type][i];
        }
 
    /* MB types used */
    /* 示例
     * x264 [info]: mb I  I16..4: 15.3% 37.5% 47.3%
     * x264 [info]: mb P  I16..4:  0.6%  0.4%  0.2%  P16..4: 34.6% 21.2% 12.7%  0.0%  0.0%    skip:30.4%
     * x264 [info]: mb B  I16..4:  0.0%  0.0%  0.0%  B16..8: 21.2%  4.1%  0.7%  direct: 0.8%  skip:73.1%  L0:28.7% L1:53.0% BI:18.3%
     */
    if( h->stat.i_frame_count[SLICE_TYPE_I] > 0 )
    {
        int64_t *i_mb_count = h->stat.i_mb_count[SLICE_TYPE_I];
        double i_count = h->stat.i_frame_count[SLICE_TYPE_I] * h->mb.i_mb_count / 100.0;
        //Intra宏块信息-存于buf
        //从左到右3个信息，依次为I16x16,I8x8,I4x4
        x264_print_intra( i_mb_count, i_count, b_print_pcm, buf );
        x264_log( h, X264_LOG_INFO, "mb I  %s\n", buf );
    }
    if( h->stat.i_frame_count[SLICE_TYPE_P] > 0 )
    {
        int64_t *i_mb_count = h->stat.i_mb_count[SLICE_TYPE_P];
        double i_count = h->stat.i_frame_count[SLICE_TYPE_P] * h->mb.i_mb_count / 100.0;
        int64_t *i_mb_size = i_mb_count_size[SLICE_TYPE_P];
        //Intra宏块信息-存于buf
        x264_print_intra( i_mb_count, i_count, b_print_pcm, buf );
        //Intra宏块信息-放在最前面
        //后面添加P宏块信息
        //从左到右6个信息，依次为P16x16, P16x8+P8x16, P8x8, P8x4+P4x8, P4x4, PSKIP
        x264_log( h, X264_LOG_INFO,
                  "mb P  %s  P16..4: %4.1f%% %4.1f%% %4.1f%% %4.1f%% %4.1f%%    skip:%4.1f%%\n",
                  buf,
                  i_mb_size[PIXEL_16x16] / (i_count*4),
                  (i_mb_size[PIXEL_16x8] + i_mb_size[PIXEL_8x16]) / (i_count*4),
                  i_mb_size[PIXEL_8x8] / (i_count*4),
                  (i_mb_size[PIXEL_8x4] + i_mb_size[PIXEL_4x8]) / (i_count*4),
                  i_mb_size[PIXEL_4x4] / (i_count*4),
                  i_mb_count[P_SKIP] / i_count );
    }
    if( h->stat.i_frame_count[SLICE_TYPE_B] > 0 )
    {
        int64_t *i_mb_count = h->stat.i_mb_count[SLICE_TYPE_B];
        double i_count = h->stat.i_frame_count[SLICE_TYPE_B] * h->mb.i_mb_count / 100.0;
        double i_mb_list_count;
        int64_t *i_mb_size = i_mb_count_size[SLICE_TYPE_B];
        int64_t list_count[3] = {0}; /* 0 == L0, 1 == L1, 2 == BI */
        //Intra宏块信息
        x264_print_intra( i_mb_count, i_count, b_print_pcm, buf );
        for( int i = 0; i < X264_PARTTYPE_MAX; i++ )
            for( int j = 0; j < 2; j++ )
            {
                int l0 = x264_mb_type_list_table[i][0][j];
                int l1 = x264_mb_type_list_table[i][1][j];
                if( l0 || l1 )
                    list_count[l1+l0*l1] += h->stat.i_mb_count[SLICE_TYPE_B][i] * 2;
            }
        list_count[0] += h->stat.i_mb_partition[SLICE_TYPE_B][D_L0_8x8];
        list_count[1] += h->stat.i_mb_partition[SLICE_TYPE_B][D_L1_8x8];
        list_count[2] += h->stat.i_mb_partition[SLICE_TYPE_B][D_BI_8x8];
        i_mb_count[B_DIRECT] += (h->stat.i_mb_partition[SLICE_TYPE_B][D_DIRECT_8x8]+2)/4;
        i_mb_list_count = (list_count[0] + list_count[1] + list_count[2]) / 100.0;
        //Intra宏块信息-放在最前面
        //后面添加B宏块信息
        //从左到右5个信息，依次为B16x16, B16x8+B8x16, B8x8, BDIRECT, BSKIP
        //
        //SKIP和DIRECT区别
        //P_SKIP的CBP为0,无像素残差，无运动矢量残
        //B_SKIP宏块的模式为B_DIRECT且CBP为0,无像素残差，无运动矢量残
        //B_DIRECT的CBP不为0,有像素残差，无运动矢量残
        sprintf( buf + strlen(buf), "  B16..8: %4.1f%% %4.1f%% %4.1f%%  direct:%4.1f%%  skip:%4.1f%%",
                 i_mb_size[PIXEL_16x16] / (i_count*4),
                 (i_mb_size[PIXEL_16x8] + i_mb_size[PIXEL_8x16]) / (i_count*4),
                 i_mb_size[PIXEL_8x8] / (i_count*4),
                 i_mb_count[B_DIRECT] / i_count,
                 i_mb_count[B_SKIP]   / i_count );
        if( i_mb_list_count != 0 )
            sprintf( buf + strlen(buf), "  L0:%4.1f%% L1:%4.1f%% BI:%4.1f%%",
                     list_count[0] / i_mb_list_count,
                     list_count[1] / i_mb_list_count,
                     list_count[2] / i_mb_list_count );
        x264_log( h, X264_LOG_INFO, "mb B  %s\n", buf );
    }
    //码率控制信息
    /* 示例
     * x264 [info]: final ratefactor: 20.01
     */
    x264_ratecontrol_summary( h );
 
    if( h->stat.i_frame_count[SLICE_TYPE_I] + h->stat.i_frame_count[SLICE_TYPE_P] + h->stat.i_frame_count[SLICE_TYPE_B] > 0 )
    {
#define SUM3(p) (p[SLICE_TYPE_I] + p[SLICE_TYPE_P] + p[SLICE_TYPE_B])
#define SUM3b(p,o) (p[SLICE_TYPE_I][o] + p[SLICE_TYPE_P][o] + p[SLICE_TYPE_B][o])
        int64_t i_i8x8 = SUM3b( h->stat.i_mb_count, I_8x8 );
        int64_t i_intra = i_i8x8 + SUM3b( h->stat.i_mb_count, I_4x4 )
                                 + SUM3b( h->stat.i_mb_count, I_16x16 );
        int64_t i_all_intra = i_intra + SUM3b( h->stat.i_mb_count, I_PCM);
        int64_t i_skip = SUM3b( h->stat.i_mb_count, P_SKIP )
                       + SUM3b( h->stat.i_mb_count, B_SKIP );
        const int i_count = h->stat.i_frame_count[SLICE_TYPE_I] +
                            h->stat.i_frame_count[SLICE_TYPE_P] +
                            h->stat.i_frame_count[SLICE_TYPE_B];
        int64_t i_mb_count = (int64_t)i_count * h->mb.i_mb_count;
        int64_t i_inter = i_mb_count - i_skip - i_intra;
        const double duration = h->stat.f_frame_duration[SLICE_TYPE_I] +
                                h->stat.f_frame_duration[SLICE_TYPE_P] +
                                h->stat.f_frame_duration[SLICE_TYPE_B];
        float f_bitrate = SUM3(h->stat.i_frame_size) / duration / 125;
        //隔行
        if( PARAM_INTERLACED )
        {
            char *fieldstats = buf;
            fieldstats[0] = 0;
            if( i_inter )
                fieldstats += sprintf( fieldstats, " inter:%.1f%%", h->stat.i_mb_field[1] * 100.0 / i_inter );
            if( i_skip )
                fieldstats += sprintf( fieldstats, " skip:%.1f%%", h->stat.i_mb_field[2] * 100.0 / i_skip );
            x264_log( h, X264_LOG_INFO, "field mbs: intra: %.1f%%%s\n",
                      h->stat.i_mb_field[0] * 100.0 / i_intra, buf );
        }
        //8x8DCT信息
        if( h->pps->b_transform_8x8_mode )
        {
            buf[0] = 0;
            if( h->stat.i_mb_count_8x8dct[0] )
                sprintf( buf, " inter:%.1f%%", 100. * h->stat.i_mb_count_8x8dct[1] / h->stat.i_mb_count_8x8dct[0] );
            x264_log( h, X264_LOG_INFO, "8x8 transform intra:%.1f%%%s\n", 100. * i_i8x8 / i_intra, buf );
        }
 
        if( (h->param.analyse.i_direct_mv_pred == X264_DIRECT_PRED_AUTO ||
            (h->stat.i_direct_frames[0] && h->stat.i_direct_frames[1]))
            && h->stat.i_frame_count[SLICE_TYPE_B] )
        {
            x264_log( h, X264_LOG_INFO, "direct mvs  spatial:%.1f%% temporal:%.1f%%\n",
                      h->stat.i_direct_frames[1] * 100. / h->stat.i_frame_count[SLICE_TYPE_B],
                      h->stat.i_direct_frames[0] * 100. / h->stat.i_frame_count[SLICE_TYPE_B] );
        }
 
        buf[0] = 0;
        int csize = CHROMA444 ? 4 : 1;
        if( i_mb_count != i_all_intra )
            sprintf( buf, " inter: %.1f%% %.1f%% %.1f%%",
                     h->stat.i_mb_cbp[1] * 100.0 / ((i_mb_count - i_all_intra)*4),
                     h->stat.i_mb_cbp[3] * 100.0 / ((i_mb_count - i_all_intra)*csize),
                     h->stat.i_mb_cbp[5] * 100.0 / ((i_mb_count - i_all_intra)*csize) );
        /*
         * 示例
         * x264 [info]: coded y,uvDC,uvAC intra: 74.1% 83.3% 58.9% inter: 10.4% 6.6% 0.4%
         */
        x264_log( h, X264_LOG_INFO, "coded y,%s,%s intra: %.1f%% %.1f%% %.1f%%%s\n",
                  CHROMA444?"u":"uvDC", CHROMA444?"v":"uvAC",
                  h->stat.i_mb_cbp[0] * 100.0 / (i_all_intra*4),
                  h->stat.i_mb_cbp[2] * 100.0 / (i_all_intra*csize),
                  h->stat.i_mb_cbp[4] * 100.0 / (i_all_intra*csize), buf );
 
        /*
         * 帧内预测信息
         * 从上到下分别为I16x16,I8x8,I4x4
         * 从左到右顺序为Vertical, Horizontal, DC, Plane ....
         *
         * 示例
         *
         * x264 [info]: i16 v,h,dc,p: 21% 25%  7% 48%
         * x264 [info]: i8 v,h,dc,ddl,ddr,vr,hd,vl,hu: 25% 23% 13%  6%  5%  5%  6%  8% 10%
         * x264 [info]: i4 v,h,dc,ddl,ddr,vr,hd,vl,hu: 22% 20%  9%  7%  7%  8%  8%  7% 12%
         * x264 [info]: i8c dc,h,v,p: 43% 20% 27% 10%
         *
         */
        int64_t fixed_pred_modes[4][9] = {{0}};
        int64_t sum_pred_modes[4] = {0};
        for( int i = 0; i <= I_PRED_16x16_DC_128; i++ )
        {
            fixed_pred_modes[0][x264_mb_pred_mode16x16_fix[i]] += h->stat.i_mb_pred_mode[0][i];
            sum_pred_modes[0] += h->stat.i_mb_pred_mode[0][i];
        }
        if( sum_pred_modes[0] )
            x264_log( h, X264_LOG_INFO, "i16 v,h,dc,p: %2.0f%% %2.0f%% %2.0f%% %2.0f%%\n",
                      fixed_pred_modes[0][0] * 100.0 / sum_pred_modes[0],
                      fixed_pred_modes[0][1] * 100.0 / sum_pred_modes[0],
                      fixed_pred_modes[0][2] * 100.0 / sum_pred_modes[0],
                      fixed_pred_modes[0][3] * 100.0 / sum_pred_modes[0] );
 
        for( int i = 1; i <= 2; i++ )
        {
            for( int j = 0; j <= I_PRED_8x8_DC_128; j++ )
            {
                fixed_pred_modes[i][x264_mb_pred_mode4x4_fix(j)] += h->stat.i_mb_pred_mode[i][j];
                sum_pred_modes[i] += h->stat.i_mb_pred_mode[i][j];
            }
            if( sum_pred_modes[i] )
                x264_log( h, X264_LOG_INFO, "i%d v,h,dc,ddl,ddr,vr,hd,vl,hu: %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%% %2.0f%%\n", (3-i)*4,
                          fixed_pred_modes[i][0] * 100.0 / sum_pred_modes[i],
                          fixed_pred_modes[i][1] * 100.0 / sum_pred_modes[i],
                          fixed_pred_modes[i][2] * 100.0 / sum_pred_modes[i],
                          fixed_pred_modes[i][3] * 100.0 / sum_pred_modes[i],
                          fixed_pred_modes[i][4] * 100.0 / sum_pred_modes[i],
                          fixed_pred_modes[i][5] * 100.0 / sum_pred_modes[i],
                          fixed_pred_modes[i][6] * 100.0 / sum_pred_modes[i],
                          fixed_pred_modes[i][7] * 100.0 / sum_pred_modes[i],
                          fixed_pred_modes[i][8] * 100.0 / sum_pred_modes[i] );
        }
        for( int i = 0; i <= I_PRED_CHROMA_DC_128; i++ )
        {
            fixed_pred_modes[3][x264_mb_chroma_pred_mode_fix[i]] += h->stat.i_mb_pred_mode[3][i];
            sum_pred_modes[3] += h->stat.i_mb_pred_mode[3][i];
        }
        if( sum_pred_modes[3] && !CHROMA444 )
            x264_log( h, X264_LOG_INFO, "i8c dc,h,v,p: %2.0f%% %2.0f%% %2.0f%% %2.0f%%\n",
                      fixed_pred_modes[3][0] * 100.0 / sum_pred_modes[3],
                      fixed_pred_modes[3][1] * 100.0 / sum_pred_modes[3],
                      fixed_pred_modes[3][2] * 100.0 / sum_pred_modes[3],
                      fixed_pred_modes[3][3] * 100.0 / sum_pred_modes[3] );
 
        if( h->param.analyse.i_weighted_pred >= X264_WEIGHTP_SIMPLE && h->stat.i_frame_count[SLICE_TYPE_P] > 0 )
            x264_log( h, X264_LOG_INFO, "Weighted P-Frames: Y:%.1f%% UV:%.1f%%\n",
                      h->stat.i_wpred[0] * 100.0 / h->stat.i_frame_count[SLICE_TYPE_P],
                      h->stat.i_wpred[1] * 100.0 / h->stat.i_frame_count[SLICE_TYPE_P] );
 
        /*
         * 参考帧信息
         * 从左到右依次为不同序号的参考帧
         *
         * 示例
         *
         * x264 [info]: ref P L0: 62.5% 19.7% 13.8%  4.0%
         * x264 [info]: ref B L0: 88.8%  9.4%  1.9%
         * x264 [info]: ref B L1: 92.6%  7.4%
         *
         */
        for( int i_list = 0; i_list < 2; i_list++ )
            for( int i_slice = 0; i_slice < 2; i_slice++ )
            {
                char *p = buf;
                int64_t i_den = 0;
                int i_max = 0;
                for( int i = 0; i < X264_REF_MAX*2; i++ )
                    if( h->stat.i_mb_count_ref[i_slice][i_list][i] )
                    {
                        i_den += h->stat.i_mb_count_ref[i_slice][i_list][i];
                        i_max = i;
                    }
                if( i_max == 0 )
                    continue;
                for( int i = 0; i <= i_max; i++ )
                    p += sprintf( p, " %4.1f%%", 100. * h->stat.i_mb_count_ref[i_slice][i_list][i] / i_den );
                x264_log( h, X264_LOG_INFO, "ref %c L%d:%s\n", "PB"[i_slice], i_list, buf );
            }
 
        if( h->param.analyse.b_ssim )
        {
            float ssim = SUM3( h->stat.f_ssim_mean_y ) / duration;
            x264_log( h, X264_LOG_INFO, "SSIM Mean Y:%.7f (%6.3fdb)\n", ssim, x264_ssim( ssim ) );
        }
        /*
         * 示例
         *
         * x264 [info]: PSNR Mean Y:42.967 U:47.163 V:47.000 Avg:43.950 Global:43.796 kb/s:339.67
         *
         */
        if( h->param.analyse.b_psnr )
        {
            x264_log( h, X264_LOG_INFO,
                      "PSNR Mean Y:%6.3f U:%6.3f V:%6.3f Avg:%6.3f Global:%6.3f kb/s:%.2f\n",
                      SUM3( h->stat.f_psnr_mean_y ) / duration,
                      SUM3( h->stat.f_psnr_mean_u ) / duration,
                      SUM3( h->stat.f_psnr_mean_v ) / duration,
                      SUM3( h->stat.f_psnr_average ) / duration,
                      x264_psnr( SUM3( h->stat.f_ssd_global ), duration * i_yuv_size ),
                      f_bitrate );
        }
        else
            x264_log( h, X264_LOG_INFO, "kb/s:%.2f\n", f_bitrate );
    }
 
    //各种释放
 
    /* rc */
    x264_ratecontrol_delete( h );
 
    /* param */
    if( h->param.rc.psz_stat_out )
        free( h->param.rc.psz_stat_out );
    if( h->param.rc.psz_stat_in )
        free( h->param.rc.psz_stat_in );
 
    x264_cqm_delete( h );
    x264_free( h->nal_buffer );
    x264_free( h->reconfig_h );
    x264_analyse_free_costs( h );
 
    if( h->i_thread_frames > 1 )
        h = h->thread[h->i_thread_phase];
 
    /* frames */
    x264_frame_delete_list( h->frames.unused[0] );
    x264_frame_delete_list( h->frames.unused[1] );
    x264_frame_delete_list( h->frames.current );
    x264_frame_delete_list( h->frames.blank_unused );
 
    h = h->thread[0];
 
    for( int i = 0; i < h->i_thread_frames; i++ )
        if( h->thread[i]->b_thread_active )
            for( int j = 0; j < h->thread[i]->i_ref[0]; j++ )
                if( h->thread[i]->fref[0][j] && h->thread[i]->fref[0][j]->b_duplicate )
                    x264_frame_delete( h->thread[i]->fref[0][j] );
 
    if( h->param.i_lookahead_threads > 1 )
        for( int i = 0; i < h->param.i_lookahead_threads; i++ )
            x264_free( h->lookahead_thread[i] );
 
    for( int i = h->param.i_threads - 1; i >= 0; i-- )
    {
        x264_frame_t **frame;
 
        if( !h->param.b_sliced_threads || i == 0 )
        {
            for( frame = h->thread[i]->frames.reference; *frame; frame++ )
            {
                assert( (*frame)->i_reference_count > 0 );
                (*frame)->i_reference_count--;
                if( (*frame)->i_reference_count == 0 )
                    x264_frame_delete( *frame );
            }
            frame = &h->thread[i]->fdec;
            if( *frame )
            {
                assert( (*frame)->i_reference_count > 0 );
                (*frame)->i_reference_count--;
                if( (*frame)->i_reference_count == 0 )
                    x264_frame_delete( *frame );
            }
            x264_macroblock_cache_free( h->thread[i] );
        }
        x264_macroblock_thread_free( h->thread[i], 0 );
        x264_free( h->thread[i]->out.p_bitstream );
        x264_free( h->thread[i]->out.nal );
        x264_pthread_mutex_destroy( &h->thread[i]->mutex );
        x264_pthread_cond_destroy( &h->thread[i]->cv );
        x264_free( h->thread[i] );
    }
#if HAVE_OPENCL
    x264_opencl_close_library( ocl );
#endif
}
```













