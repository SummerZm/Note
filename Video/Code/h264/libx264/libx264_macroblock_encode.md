## **x264源代码简单分析：宏块编码（Encode）部分**


- **x264_macroblock_encode()**
    1. x264_macroblock_encode()用于编码宏块。该函数的定义位于encoder\macroblock.c，如下所示。
        ```C
        //编码-残差DCT变换、量化
        void x264_macroblock_encode( x264_t *h )
        {
            //编码-内部函数
            //YUV444相当于把YUV3个分量都当做Y编码
            if( CHROMA444 )
                x264_macroblock_encode_internal( h, 3, 0 );
            else
                x264_macroblock_encode_internal( h, 1, 1 );
        }
        ```

- **x264_macroblock_encode_internal()完成了如下功能：**
    1. x264_macroblock_encode_skip()：编码Skip类型宏块。
    2. x264_mb_encode_i16x16()：编码Intra16x16类型的宏块。该函数除了进行DCT变换之外，还对16个小块的DC系数进行了Hadamard变换。
    3. x264_mb_encode_i4x4()：编码Intra4x4类型的宏块。
    4. x264_mb_encode_chroma()：编码色度块。

- **x264_macroblock_encode_internal()源代码**
    ```C
    /*****************************************************************************
    * x264_macroblock_encode:
    * 编码-残差DCT变换、量化-内部函数
    *
    * 注释和处理：雷霄骅
    * http://blog.csdn.net/leixiaohua1020
    * leixiaohua1020@126.com
    *****************************************************************************/
    /*
     A. 如果是Skip类型，调用x264_macroblock_encode_skip()编码宏块。
     B. 如果是Intra16x16类型，调用x264_mb_encode_i16x16()编码宏块。
     C. 如果是Intra4x4类型，循环16次调用x264_mb_encode_i4x4()编码宏块。
     D. 如果是Inter类型，则不再调用子函数，而是直接进行编码：
        1. 对16x16块调用x264_dct_function_t的sub16x16_dct()汇编函数，求得编码宏块数据p_fenc与重建宏块数据p_fdec之间的残差（“sub”），并对残差进行DCT变换。
        2. 分成4个8x8的块，对每个8x8块分别调用x264_quant_function_t的quant_4x4x4()汇编函数进行量化。
        3. 分成16个4x4的块，对每个4x4块分别调用x264_quant_function_t的dequant_4x4()汇编函数进行反量化（用于重建帧）。   
        4. 分成4个8x8的块，对每个8x8块分别调用x264_dct_function_t的add8x8_idct()汇编函数，对残差进行DCT反变换，并将反变换后的数据叠加（“add”）至预测数据上（用于重建帧）。
     E. 如果对色度编码，调用x264_mb_encode_chroma()。
    */

    static ALWAYS_INLINE void x264_macroblock_encode_internal( x264_t *h, int plane_count, int chroma )
    {
        int i_qp = h->mb.i_qp;
        int b_decimate = h->mb.b_dct_decimate;
        int b_force_no_skip = 0;
        int nz;
        h->mb.i_cbp_luma = 0;
        for( int p = 0; p < plane_count; p++ )
            h->mb.cache.non_zero_count[x264_scan8[LUMA_DC+p]] = 0;
        //PCM，不常见
        if( h->mb.i_type == I_PCM )
        {
            /* if PCM is chosen, we need to store reconstructed frame data */
            for( int p = 0; p < plane_count; p++ )
                h->mc.copy[PIXEL_16x16]( h->mb.pic.p_fdec[p], FDEC_STRIDE, h->mb.pic.p_fenc[p], FENC_STRIDE, 16 );
            if( chroma )
            {
                int height = 16 >> CHROMA_V_SHIFT;
                h->mc.copy[PIXEL_8x8]  ( h->mb.pic.p_fdec[1], FDEC_STRIDE, h->mb.pic.p_fenc[1], FENC_STRIDE, height );
                h->mc.copy[PIXEL_8x8]  ( h->mb.pic.p_fdec[2], FDEC_STRIDE, h->mb.pic.p_fenc[2], FENC_STRIDE, height );
            }
            return;
        }
    
        if( !h->mb.b_allow_skip )
        {
            b_force_no_skip = 1;
            if( IS_SKIP(h->mb.i_type) )
            {
                if( h->mb.i_type == P_SKIP )
                    h->mb.i_type = P_L0;
                else if( h->mb.i_type == B_SKIP )
                    h->mb.i_type = B_DIRECT;
            }
        }
        //根据不同的宏块类型，进行编码
        if( h->mb.i_type == P_SKIP )
        {
            /* don't do pskip motion compensation if it was already done in macroblock_analyse */
            if( !h->mb.b_skip_mc )
            {
                int mvx = x264_clip3( h->mb.cache.mv[0][x264_scan8[0]][0],
                                    h->mb.mv_min[0], h->mb.mv_max[0] );
                int mvy = x264_clip3( h->mb.cache.mv[0][x264_scan8[0]][1],
                                    h->mb.mv_min[1], h->mb.mv_max[1] );
    
                for( int p = 0; p < plane_count; p++ )
                    h->mc.mc_luma( h->mb.pic.p_fdec[p], FDEC_STRIDE,
                                &h->mb.pic.p_fref[0][0][p*4], h->mb.pic.i_stride[p],
                                mvx, mvy, 16, 16, &h->sh.weight[0][p] );
    
                if( chroma )
                {
                    int v_shift = CHROMA_V_SHIFT;
                    int height = 16 >> v_shift;
    
                    /* Special case for mv0, which is (of course) very common in P-skip mode. */
                    if( mvx | mvy )
                        h->mc.mc_chroma( h->mb.pic.p_fdec[1], h->mb.pic.p_fdec[2], FDEC_STRIDE,
                                        h->mb.pic.p_fref[0][0][4], h->mb.pic.i_stride[1],
                                        mvx, 2*mvy>>v_shift, 8, height );
                    else
                        h->mc.load_deinterleave_chroma_fdec( h->mb.pic.p_fdec[1], h->mb.pic.p_fref[0][0][4],
                                                            h->mb.pic.i_stride[1], height );
    
                    if( h->sh.weight[0][1].weightfn )
                        h->sh.weight[0][1].weightfn[8>>2]( h->mb.pic.p_fdec[1], FDEC_STRIDE,
                                                        h->mb.pic.p_fdec[1], FDEC_STRIDE,
                                                        &h->sh.weight[0][1], height );
                    if( h->sh.weight[0][2].weightfn )
                        h->sh.weight[0][2].weightfn[8>>2]( h->mb.pic.p_fdec[2], FDEC_STRIDE,
                                                        h->mb.pic.p_fdec[2], FDEC_STRIDE,
                                                        &h->sh.weight[0][2], height );
                }
            }
            //编码skip类型宏块
            x264_macroblock_encode_skip( h );
            return;
        }
        if( h->mb.i_type == B_SKIP )
        {
            /* don't do bskip motion compensation if it was already done in macroblock_analyse */
            if( !h->mb.b_skip_mc )
                x264_mb_mc( h );
            x264_macroblock_encode_skip( h );
            return;
        }
    
        if( h->mb.i_type == I_16x16 )
        {
            h->mb.b_transform_8x8 = 0;
            //Intra16x16宏块编码-需要Hadamard变换
            //分别编码Y，U，V
            /*
            * 16x16 宏块
            *
            * +--------+--------+
            * |                 |
            * |                 |
            * |                 |
            * +        +        +
            * |                 |
            * |                 |
            * |                 |
            * +--------+--------+
            *
            */
            for( int p = 0; p < plane_count; p++, i_qp = h->mb.i_chroma_qp )
                x264_mb_encode_i16x16( h, p, i_qp );
        }
        else if( h->mb.i_type == I_8x8 )
        {
            h->mb.b_transform_8x8 = 1;
            /* If we already encoded 3 of the 4 i8x8 blocks, we don't have to do them again. */
            if( h->mb.i_skip_intra )
            {
                h->mc.copy[PIXEL_16x16]( h->mb.pic.p_fdec[0], FDEC_STRIDE, h->mb.pic.i8x8_fdec_buf, 16, 16 );
                M32( &h->mb.cache.non_zero_count[x264_scan8[ 0]] ) = h->mb.pic.i8x8_nnz_buf[0];
                M32( &h->mb.cache.non_zero_count[x264_scan8[ 2]] ) = h->mb.pic.i8x8_nnz_buf[1];
                M32( &h->mb.cache.non_zero_count[x264_scan8[ 8]] ) = h->mb.pic.i8x8_nnz_buf[2];
                M32( &h->mb.cache.non_zero_count[x264_scan8[10]] ) = h->mb.pic.i8x8_nnz_buf[3];
                h->mb.i_cbp_luma = h->mb.pic.i8x8_cbp;
                /* In RD mode, restore the now-overwritten DCT data. */
                if( h->mb.i_skip_intra == 2 )
                    h->mc.memcpy_aligned( h->dct.luma8x8, h->mb.pic.i8x8_dct_buf, sizeof(h->mb.pic.i8x8_dct_buf) );
            }
            for( int p = 0; p < plane_count; p++, i_qp = h->mb.i_chroma_qp )
            {
                for( int i = (p == 0 && h->mb.i_skip_intra) ? 3 : 0 ; i < 4; i++ )
                {
                    int i_mode = h->mb.cache.intra4x4_pred_mode[x264_scan8[4*i]];
                    x264_mb_encode_i8x8( h, p, i, i_qp, i_mode, NULL, 1 );
                }
            }
        }
        //Intra4x4类型
        else if( h->mb.i_type == I_4x4 )
        {
            /*
            * 帧内预测：16x16 宏块被划分为16个4x4子块
            *
            * +----+----+----+----+
            * |    |    |    |    |
            * +----+----+----+----+
            * |    |    |    |    |
            * +----+----+----+----+
            * |    |    |    |    |
            * +----+----+----+----+
            * |    |    |    |    |
            * +----+----+----+----+
            *
            */
            h->mb.b_transform_8x8 = 0;
            /* If we already encoded 15 of the 16 i4x4 blocks, we don't have to do them again. */
            if( h->mb.i_skip_intra )
            {
                h->mc.copy[PIXEL_16x16]( h->mb.pic.p_fdec[0], FDEC_STRIDE, h->mb.pic.i4x4_fdec_buf, 16, 16 );
                M32( &h->mb.cache.non_zero_count[x264_scan8[ 0]] ) = h->mb.pic.i4x4_nnz_buf[0];
                M32( &h->mb.cache.non_zero_count[x264_scan8[ 2]] ) = h->mb.pic.i4x4_nnz_buf[1];
                M32( &h->mb.cache.non_zero_count[x264_scan8[ 8]] ) = h->mb.pic.i4x4_nnz_buf[2];
                M32( &h->mb.cache.non_zero_count[x264_scan8[10]] ) = h->mb.pic.i4x4_nnz_buf[3];
                h->mb.i_cbp_luma = h->mb.pic.i4x4_cbp;
                /* In RD mode, restore the now-overwritten DCT data. */
                if( h->mb.i_skip_intra == 2 )
                    h->mc.memcpy_aligned( h->dct.luma4x4, h->mb.pic.i4x4_dct_buf, sizeof(h->mb.pic.i4x4_dct_buf) );
            }
            //分别编码Y,U,V
            for( int p = 0; p < plane_count; p++, i_qp = h->mb.i_chroma_qp )
            {
                //循环16次，编码16个Intra4x4宏块
                for( int i = (p == 0 && h->mb.i_skip_intra) ? 15 : 0 ; i < 16; i++ )
                {
                    pixel *p_dst = &h->mb.pic.p_fdec[p][block_idx_xy_fdec[i]];
                    int i_mode = h->mb.cache.intra4x4_pred_mode[x264_scan8[i]];
    
                    if( (h->mb.i_neighbour4[i] & (MB_TOPRIGHT|MB_TOP)) == MB_TOP )
                        /* emulate missing topright samples */
                        MPIXEL_X4( &p_dst[4-FDEC_STRIDE] ) = PIXEL_SPLAT_X4( p_dst[3-FDEC_STRIDE] );
                    //Intra4x4宏块编码
                    /*
                    * +----+
                    * |    |
                    * +----+
                    */
                    x264_mb_encode_i4x4( h, p, i, i_qp, i_mode, 1 );
                }
            }
        }
        //包含帧间预测
        else    /* Inter MB */
        {
            int i_decimate_mb = 0;
    
            /* Don't repeat motion compensation if it was already done in non-RD transform analysis */
            if( !h->mb.b_skip_mc )
                x264_mb_mc( h );
    
            if( h->mb.b_lossless )//lossless情况没研究过
            {
                if( h->mb.b_transform_8x8 )
                    for( int p = 0; p < plane_count; p++ )
                        for( int i8x8 = 0; i8x8 < 4; i8x8++ )
                        {
                            int x = i8x8&1;
                            int y = i8x8>>1;
                            nz = h->zigzagf.sub_8x8( h->dct.luma8x8[p*4+i8x8], h->mb.pic.p_fenc[p] + 8*x + 8*y*FENC_STRIDE,
                                                                            h->mb.pic.p_fdec[p] + 8*x + 8*y*FDEC_STRIDE );
                            STORE_8x8_NNZ( p, i8x8, nz );
                            h->mb.i_cbp_luma |= nz << i8x8;
                        }
                else
                    for( int p = 0; p < plane_count; p++ )
                        for( int i4x4 = 0; i4x4 < 16; i4x4++ )
                        {
                            nz = h->zigzagf.sub_4x4( h->dct.luma4x4[p*16+i4x4],
                                                    h->mb.pic.p_fenc[p]+block_idx_xy_fenc[i4x4],
                                                    h->mb.pic.p_fdec[p]+block_idx_xy_fdec[i4x4] );
                            h->mb.cache.non_zero_count[x264_scan8[p*16+i4x4]] = nz;
                            h->mb.i_cbp_luma |= nz << (i4x4>>2);
                        }
            }
            else if( h->mb.b_transform_8x8 )//DCT8x8情况暂时没研究过
            {
                ALIGNED_ARRAY_N( dctcoef, dct8x8,[4],[64] );
                b_decimate &= !h->mb.b_trellis || !h->param.b_cabac; // 8x8 trellis is inherently optimal decimation for CABAC
    
                for( int p = 0; p < plane_count; p++, i_qp = h->mb.i_chroma_qp )
                {
                    CLEAR_16x16_NNZ( p );
                    h->dctf.sub16x16_dct8( dct8x8, h->mb.pic.p_fenc[p], h->mb.pic.p_fdec[p] );
                    h->nr_count[1+!!p*2] += h->mb.b_noise_reduction * 4;
    
                    int plane_cbp = 0;
                    for( int idx = 0; idx < 4; idx++ )
                    {
                        nz = x264_quant_8x8( h, dct8x8[idx], i_qp, ctx_cat_plane[DCT_LUMA_8x8][p], 0, p, idx );
    
                        if( nz )
                        {
                            h->zigzagf.scan_8x8( h->dct.luma8x8[p*4+idx], dct8x8[idx] );
                            if( b_decimate )
                            {
                                int i_decimate_8x8 = h->quantf.decimate_score64( h->dct.luma8x8[p*4+idx] );
                                i_decimate_mb += i_decimate_8x8;
                                if( i_decimate_8x8 >= 4 )
                                    plane_cbp |= 1<<idx;
                            }
                            else
                                plane_cbp |= 1<<idx;
                        }
                    }
    
                    if( i_decimate_mb >= 6 || !b_decimate )
                    {
                        h->mb.i_cbp_luma |= plane_cbp;
                        FOREACH_BIT( idx, 0, plane_cbp )
                        {
                            h->quantf.dequant_8x8( dct8x8[idx], h->dequant8_mf[p?CQM_8PC:CQM_8PY], i_qp );
                            h->dctf.add8x8_idct8( &h->mb.pic.p_fdec[p][8*(idx&1) + 8*(idx>>1)*FDEC_STRIDE], dct8x8[idx] );
                            STORE_8x8_NNZ( p, idx, 1 );
                        }
                    }
                }
            }
            else//最普通的情况
            {
                /*
                * 帧间预测：16x16 宏块被划分为8x8
                * 每个8x8再次被划分为4x4
                *
                * ++====+====++====+====++
                * ||    |    ||    |    ||
                * ++====+====++====+====++
                * ||    |    ||    |    ||
                * ++====+====++====+====++
                * ||    |    ||    |    ||
                * ++====+====++====+====++
                * ||    |    ||    |    ||
                * ++====+====+=====+====++
                *
                */
                ALIGNED_ARRAY_N( dctcoef, dct4x4,[16],[16] );
                for( int p = 0; p < plane_count; p++, i_qp = h->mb.i_chroma_qp )
                {
                    CLEAR_16x16_NNZ( p );
                    //16x16DCT（实际上分解为16个4x4DCT）
                    //求编码帧p_fenc和重建帧p_fdec之间的残差，然后进行DCT变换
                    h->dctf.sub16x16_dct( dct4x4, h->mb.pic.p_fenc[p], h->mb.pic.p_fdec[p] );
    
                    if( h->mb.b_noise_reduction )
                    {
                        h->nr_count[0+!!p*2] += 16;
                        for( int idx = 0; idx < 16; idx++ )
                            h->quantf.denoise_dct( dct4x4[idx], h->nr_residual_sum[0+!!p*2], h->nr_offset[0+!!p*2], 16 );
                    }
    
                    int plane_cbp = 0;
                    //16x16的块分成4个8x8的块
                    for( int i8x8 = 0; i8x8 < 4; i8x8++ )
                    {
                        int i_decimate_8x8 = b_decimate ? 0 : 6;
                        int nnz8x8 = 0;
                        if( h->mb.b_trellis )
                        {
                            for( int i4x4 = 0; i4x4 < 4; i4x4++ )
                            {
                                int idx = i8x8*4+i4x4;
                                if( x264_quant_4x4_trellis( h, dct4x4[idx], CQM_4PY, i_qp, ctx_cat_plane[DCT_LUMA_4x4][p], 0, !!p, p*16+idx ) )
                                {
                                    h->zigzagf.scan_4x4( h->dct.luma4x4[p*16+idx], dct4x4[idx] );
                                    h->quantf.dequant_4x4( dct4x4[idx], h->dequant4_mf[p?CQM_4PC:CQM_4PY], i_qp );
                                    if( i_decimate_8x8 < 6 )
                                        i_decimate_8x8 += h->quantf.decimate_score16( h->dct.luma4x4[p*16+idx] );
                                    h->mb.cache.non_zero_count[x264_scan8[p*16+idx]] = 1;
                                    nnz8x8 = 1;
                                }
                            }
                        }
                        else
                        {
                            //8x8的块分成4个4x4的块，每个4x4的块再分别进行量化
                            nnz8x8 = nz = h->quantf.quant_4x4x4( &dct4x4[i8x8*4], h->quant4_mf[CQM_4PY][i_qp], h->quant4_bias[CQM_4PY][i_qp] );
                            if( nz )
                            {
                                FOREACH_BIT( idx, i8x8*4, nz )
                                {
                                    //这几步用于建立重建帧
                                    h->zigzagf.scan_4x4( h->dct.luma4x4[p*16+idx], dct4x4[idx] );
                                    //反量化
                                    h->quantf.dequant_4x4( dct4x4[idx], h->dequant4_mf[p?CQM_4PC:CQM_4PY], i_qp );
                                    if( i_decimate_8x8 < 6 )
                                        i_decimate_8x8 += h->quantf.decimate_score16( h->dct.luma4x4[p*16+idx] );
                                    h->mb.cache.non_zero_count[x264_scan8[p*16+idx]] = 1;
                                }
                            }
                        }
                        if( nnz8x8 )
                        {
                            i_decimate_mb += i_decimate_8x8;
                            if( i_decimate_8x8 < 4 )
                                STORE_8x8_NNZ( p, i8x8, 0 );
                            else
                                plane_cbp |= 1<<i8x8;
                        }
                    }
    
                    if( i_decimate_mb < 6 )
                    {
                        plane_cbp = 0;
                        CLEAR_16x16_NNZ( p );
                    }
                    else
                    {
                        h->mb.i_cbp_luma |= plane_cbp;
                        FOREACH_BIT( i8x8, 0, plane_cbp )
                        {
                            //用于建立重建帧
                            //残差进行DCT反变换之后，叠加到预测数据上
                            h->dctf.add8x8_idct( &h->mb.pic.p_fdec[p][(i8x8&1)*8 + (i8x8>>1)*8*FDEC_STRIDE], &dct4x4[i8x8*4] );
                        }
                    }
                }
            }
        }
    
        /* encode chroma */
        if( chroma )
        {
            if( IS_INTRA( h->mb.i_type ) )
            {
                int i_mode = h->mb.i_chroma_pred_mode;
                if( h->mb.b_lossless )
                    x264_predict_lossless_chroma( h, i_mode );
                else
                {
                    h->predict_chroma[i_mode]( h->mb.pic.p_fdec[1] );
                    h->predict_chroma[i_mode]( h->mb.pic.p_fdec[2] );
                }
            }
    
            /* encode the 8x8 blocks */
            x264_mb_encode_chroma( h, !IS_INTRA( h->mb.i_type ), h->mb.i_chroma_qp );
        }
        else
            h->mb.i_cbp_chroma = 0;
    
        /* store cbp */
        int cbp = h->mb.i_cbp_chroma << 4 | h->mb.i_cbp_luma;
        if( h->param.b_cabac )
            cbp |= h->mb.cache.non_zero_count[x264_scan8[LUMA_DC    ]] << 8
                |  h->mb.cache.non_zero_count[x264_scan8[CHROMA_DC+0]] << 9
                |  h->mb.cache.non_zero_count[x264_scan8[CHROMA_DC+1]] << 10;
        h->mb.cbp[h->mb.i_mb_xy] = cbp;
    
        /* Check for P_SKIP
        * XXX: in the me perhaps we should take x264_mb_predict_mv_pskip into account
        *      (if multiple mv give same result)*/
        if( !b_force_no_skip )
        {
            if( h->mb.i_type == P_L0 && h->mb.i_partition == D_16x16 &&
                !(h->mb.i_cbp_luma | h->mb.i_cbp_chroma) &&
                M32( h->mb.cache.mv[0][x264_scan8[0]] ) == M32( h->mb.cache.pskip_mv )
                && h->mb.cache.ref[0][x264_scan8[0]] == 0 )
            {
                h->mb.i_type = P_SKIP;
            }
    
            /* Check for B_SKIP */
            if( h->mb.i_type == B_DIRECT && !(h->mb.i_cbp_luma | h->mb.i_cbp_chroma) )
            {
                h->mb.i_type = B_SKIP;
            }
        }
    }
    ```






- **x264_mb_encode_i16x16()**
1. x264_mb_encode_i16x16()用于编码Intra16x16的宏块。该函数的定义位于encoder\macroblock.c，如下所示

    ```C
    /*
    （1）调用predict_16x16[]()汇编函数对重建宏块数据p_fdec进行帧内预测。
    （2）调用x264_dct_function_t的sub16x16_dct()汇编函数，计算重建宏块数据p_fdec与编码宏块数据p_fenc之间的残差，然后对残差做DCT变换。
    （3）抽取出来16个4x4DCT小块的DC系数，存储于dct_dc4x4[]。
    （4）分成4个8x8的块，对每个8x8块分别调用x264_quant_function_t的quant_4x4x4()汇编函数进行量化。
    （5）分成16个4x4的块，对每个4x4块分别调用x264_quant_function_t的dequant_4x4()汇编函数进行反量化（用于重建帧）。
    （6）对于dct_dc4x4[]中16个小块的DC系数作如下处理：
        a)调用x264_dct_function_t的dct4x4dc()汇编函数进行Hadamard变换。
        b)调用x264_quant_function_t的quant_4x4_dc()汇编函数进行DC系数的量化。
        c)调用x264_dct_function_t的idct4x4dc()汇编函数进行Hadamard反变换。
        d)调用x264_quant_function_t的dequant_4x4_dc()汇编函数进行DC系数的反量化。
        e)将反量化后的DC系数重新放到16x16块对应的位置上。
    （7）调用x264_dct_function_t的add16x16_idct()汇编函数，对残差进行DCT反变换，并将反变换后的数据叠加（“add”）至预测数据上（用于重建帧）。
    */
    //编码I16x16宏块-需要Hadamard变换
    /*
    * 16x16 宏块
    *
    * +--------+--------+
    * |                 |       D   D   D   D
    * |                 |
    * |                 |       D   D   D   D
    * +        +        +   +
    * |                 |       D   D   D   D
    * |                 |
    * |                 |       D   D   D   D
    * +--------+--------+
    *
    */
    //p代表分量
    static void x264_mb_encode_i16x16( x264_t *h, int p, int i_qp )
    {
        //编码帧
        pixel *p_src = h->mb.pic.p_fenc[p];
        //重建帧
        pixel *p_dst = h->mb.pic.p_fdec[p];
    
        ALIGNED_ARRAY_N( dctcoef, dct4x4,[16],[16] );
        ALIGNED_ARRAY_N( dctcoef, dct_dc4x4,[16] );
    
        int nz, block_cbp = 0;
        int decimate_score = h->mb.b_dct_decimate ? 0 : 9;
        int i_quant_cat = p ? CQM_4IC : CQM_4IY;
        int i_mode = h->mb.i_intra16x16_pred_mode;
    
        if( h->mb.b_lossless )
            x264_predict_lossless_16x16( h, p, i_mode );
        else
            h->predict_16x16[i_mode]( h->mb.pic.p_fdec[p] );   //帧内预测.p_fdec是重建帧。p_fenc是编码帧。
    
        if( h->mb.b_lossless )
        {
            for( int i = 0; i < 16; i++ )
            {
                int oe = block_idx_xy_fenc[i];
                int od = block_idx_xy_fdec[i];
                nz = h->zigzagf.sub_4x4ac( h->dct.luma4x4[16*p+i], p_src+oe, p_dst+od, &dct_dc4x4[block_idx_yx_1d[i]] );
                h->mb.cache.non_zero_count[x264_scan8[16*p+i]] = nz;
                block_cbp |= nz;
            }
            h->mb.i_cbp_luma |= block_cbp * 0xf;
            h->mb.cache.non_zero_count[x264_scan8[LUMA_DC+p]] = array_non_zero( dct_dc4x4, 16 );
            h->zigzagf.scan_4x4( h->dct.luma16x16_dc[p], dct_dc4x4 );
            return;
        }
    
        CLEAR_16x16_NNZ( p );
    
        h->dctf.sub16x16_dct( dct4x4, p_src, p_dst );  //求残差，然后进行DCT变换
    
        if( h->mb.b_noise_reduction )
            for( int idx = 0; idx < 16; idx++ )
                h->quantf.denoise_dct( dct4x4[idx], h->nr_residual_sum[0], h->nr_offset[0], 16 );
        //获取DC系数
        for( int idx = 0; idx < 16; idx++ )
        {
            //每个4x4DCT块的[0]元素
            dct_dc4x4[block_idx_xy_1d[idx]] = dct4x4[idx][0];
            //抽取出来之后，赋值0
            dct4x4[idx][0] = 0;
        }
    
        if( h->mb.b_trellis )
        {
            for( int idx = 0; idx < 16; idx++ )
                if( x264_quant_4x4_trellis( h, dct4x4[idx], i_quant_cat, i_qp, ctx_cat_plane[DCT_LUMA_AC][p], 1, !!p, idx ) )
                {
                    block_cbp = 0xf;
                    h->zigzagf.scan_4x4( h->dct.luma4x4[16*p+idx], dct4x4[idx] );
                    h->quantf.dequant_4x4( dct4x4[idx], h->dequant4_mf[i_quant_cat], i_qp );
                    if( decimate_score < 6 ) decimate_score += h->quantf.decimate_score15( h->dct.luma4x4[16*p+idx] );
                    h->mb.cache.non_zero_count[x264_scan8[16*p+idx]] = 1;
                }
        }
        else
        {
            //先分成4个8x8？
            for( int i8x8 = 0; i8x8 < 4; i8x8++ )
            {
                //每个8x8做4个4x4量化
                nz = h->quantf.quant_4x4x4( &dct4x4[i8x8*4], h->quant4_mf[i_quant_cat][i_qp], h->quant4_bias[i_quant_cat][i_qp] );
                if( nz )
                {
                    block_cbp = 0xf;
                    FOREACH_BIT( idx, i8x8*4, nz )
                    {
                        //建立重建的帧
                        //之子扫描
                        h->zigzagf.scan_4x4( h->dct.luma4x4[16*p+idx], dct4x4[idx] );
                        //反量化，用于重建图像
                        h->quantf.dequant_4x4( dct4x4[idx], h->dequant4_mf[i_quant_cat], i_qp );
                        if( decimate_score < 6 ) decimate_score += h->quantf.decimate_score15( h->dct.luma4x4[16*p+idx] );
                        h->mb.cache.non_zero_count[x264_scan8[16*p+idx]] = 1;
                    }
                }
            }
        }
    
        /* Writing the 16 CBFs in an i16x16 block is quite costly, so decimation can save many bits. */
        /* More useful with CAVLC, but still useful with CABAC. */
        if( decimate_score < 6 )
        {
            CLEAR_16x16_NNZ( p );
            block_cbp = 0;
        }
        else
            h->mb.i_cbp_luma |= block_cbp;
    
        //16个DC系数-Hadamard变换
        h->dctf.dct4x4dc( dct_dc4x4 );
        if( h->mb.b_trellis )
            nz = x264_quant_luma_dc_trellis( h, dct_dc4x4, i_quant_cat, i_qp, ctx_cat_plane[DCT_LUMA_DC][p], 1, LUMA_DC+p );
        else
            //DC-Hadamard变换之后-量化
            nz = h->quantf.quant_4x4_dc( dct_dc4x4, h->quant4_mf[i_quant_cat][i_qp][0]>>1, h->quant4_bias[i_quant_cat][i_qp][0]<<1 );
    
        h->mb.cache.non_zero_count[x264_scan8[LUMA_DC+p]] = nz;
        if( nz )
        {
    
            //之子扫描
            h->zigzagf.scan_4x4( h->dct.luma16x16_dc[p], dct_dc4x4 );
    
            /* output samples to fdec */
            //DC-反变换
            h->dctf.idct4x4dc( dct_dc4x4 );
            //DC-反量化
            h->quantf.dequant_4x4_dc( dct_dc4x4, h->dequant4_mf[i_quant_cat], i_qp );  /* XXX not inversed */
            if( block_cbp )
                for( int i = 0; i < 16; i++ )//循环16个4x4DCT块
                    dct4x4[i][0] = dct_dc4x4[block_idx_xy_1d[i]];//把DC系数重新赋值到每个DCT数组的[0]元素上
        }
    
        /* put pixels to fdec */
        // fdec代表重建帧
        if( block_cbp )
            h->dctf.add16x16_idct( p_dst, dct4x4 );//DCT反变换后，叠加到预测数据上（通用）
        else if( nz )
            h->dctf.add16x16_idct_dc( p_dst, dct_dc4x4 );//DCT反变换后，叠加到预测数据上（只有DC系数的时候）
    }
    ```

-  **DCT变换知识**
1. DCT变换的核心理念就是把图像的低频信息（对应大面积平坦区域）变换到系数矩阵的左上角，而把高频信息变换到系数矩阵的右下角。
2. 这样就可以在压缩的时候（量化）去除掉人眼不敏感的高频信息（位于矩阵右下角的系数）从而达到压缩数据的目的。二维8x8DCT变换常见的示意图如下所示。  
    ![../image/DCTpic.jpg](../image/DCTpic.jpg)
3. DCT 变换公式：  
    ![../image/DCTcaculation.jpg](../image/DCTcaculation.jpg)   
4. 逆DCT 变换公式：  
    ![../image/iDCTcaculation.jpg](../image/iDCTcaculation.jpg)   
5. 蝶形变换【逆DCT】：    
    ![../image/iDCTcaculation.jpg](../image/iDCTcaculation1.jpg)  
    ```sh
    # DCT反变换的矩阵乘法也可以改造成为2次一维IDCT变换：
    # 首先对4x4块中的每行像素进行一维IDCT变换，然后再对4x4块中的每列像素进行一维IDCT变换。
    # 而一维的IDCT变换也可以改造成为蝶形快速算法
    ```

- **DCT相关源码**
    ```c
    /****************************************************************************
    * x264_dct_init:
    ****************************************************************************/
    void x264_dct_init( int cpu, x264_dct_function_t *dctf )
    {
        //C语言版本
        //4x4DCT变换
        dctf->sub4x4_dct    = sub4x4_dct;
        dctf->add4x4_idct   = add4x4_idct;
        //8x8块：分解成4个4x4DCT变换，调用4次sub4x4_dct()
        dctf->sub8x8_dct    = sub8x8_dct;
        dctf->sub8x8_dct_dc = sub8x8_dct_dc;
        dctf->add8x8_idct   = add8x8_idct;
        dctf->add8x8_idct_dc = add8x8_idct_dc;
    
        dctf->sub8x16_dct_dc = sub8x16_dct_dc;
        //16x16块：分解成4个8x8块，调用4次sub8x8_dct()
        //实际上每个sub8x8_dct()又分解成4个4x4DCT变换，调用4次sub4x4_dct()
        dctf->sub16x16_dct  = sub16x16_dct;
        dctf->add16x16_idct = add16x16_idct;
        dctf->add16x16_idct_dc = add16x16_idct_dc;
        //8x8DCT，注意：后缀是_dct8
        dctf->sub8x8_dct8   = sub8x8_dct8;
        dctf->add8x8_idct8  = add8x8_idct8;
    
        dctf->sub16x16_dct8  = sub16x16_dct8;
        dctf->add16x16_idct8 = add16x16_idct8;
        //Hadamard变换
        dctf->dct4x4dc  = dct4x4dc;
        dctf->idct4x4dc = idct4x4dc;
    
        dctf->dct2x4dc = dct2x4dc;
    
    #if HIGH_BIT_DEPTH
    #if HAVE_MMX
        if( cpu&X264_CPU_MMX )
        {
            dctf->sub4x4_dct    = x264_sub4x4_dct_mmx;
            dctf->sub8x8_dct    = x264_sub8x8_dct_mmx;
            dctf->sub16x16_dct  = x264_sub16x16_dct_mmx;
        }
        if( cpu&X264_CPU_SSE2 )
        {
            dctf->add4x4_idct     = x264_add4x4_idct_sse2;
            dctf->dct4x4dc        = x264_dct4x4dc_sse2;
            dctf->idct4x4dc       = x264_idct4x4dc_sse2;
            dctf->sub8x8_dct8     = x264_sub8x8_dct8_sse2;
            dctf->sub16x16_dct8   = x264_sub16x16_dct8_sse2;
            dctf->add8x8_idct     = x264_add8x8_idct_sse2;
            dctf->add16x16_idct   = x264_add16x16_idct_sse2;
            dctf->add8x8_idct8    = x264_add8x8_idct8_sse2;
            dctf->add16x16_idct8    = x264_add16x16_idct8_sse2;
            dctf->sub8x8_dct_dc   = x264_sub8x8_dct_dc_sse2;
            dctf->add8x8_idct_dc  = x264_add8x8_idct_dc_sse2;
            dctf->sub8x16_dct_dc  = x264_sub8x16_dct_dc_sse2;
            dctf->add16x16_idct_dc= x264_add16x16_idct_dc_sse2;
        }
        if( cpu&X264_CPU_SSE4 )
        {
            dctf->sub8x8_dct8     = x264_sub8x8_dct8_sse4;
            dctf->sub16x16_dct8   = x264_sub16x16_dct8_sse4;
        }
        if( cpu&X264_CPU_AVX )
        {
            dctf->add4x4_idct     = x264_add4x4_idct_avx;
            dctf->dct4x4dc        = x264_dct4x4dc_avx;
            dctf->idct4x4dc       = x264_idct4x4dc_avx;
            dctf->sub8x8_dct8     = x264_sub8x8_dct8_avx;
            dctf->sub16x16_dct8   = x264_sub16x16_dct8_avx;
            dctf->add8x8_idct     = x264_add8x8_idct_avx;
            dctf->add16x16_idct   = x264_add16x16_idct_avx;
            dctf->add8x8_idct8    = x264_add8x8_idct8_avx;
            dctf->add16x16_idct8  = x264_add16x16_idct8_avx;
            dctf->add8x8_idct_dc  = x264_add8x8_idct_dc_avx;
            dctf->sub8x16_dct_dc  = x264_sub8x16_dct_dc_avx;
            dctf->add16x16_idct_dc= x264_add16x16_idct_dc_avx;
        }
    #endif // HAVE_MMX
    #else // !HIGH_BIT_DEPTH
        //MMX版本
    #if HAVE_MMX
        if( cpu&X264_CPU_MMX )
        {
            dctf->sub4x4_dct    = x264_sub4x4_dct_mmx;
            dctf->add4x4_idct   = x264_add4x4_idct_mmx;
            dctf->idct4x4dc     = x264_idct4x4dc_mmx;
            dctf->sub8x8_dct_dc = x264_sub8x8_dct_dc_mmx2;
        //此处省略大量的X86、ARM等平台的汇编函数初始化代码
    }
    //8x8块：分解成4个4x4DCT变换，调用4次sub4x4_dct()
    //返回dct[4][16]
    static void sub8x8_dct( dctcoef dct[4][16], pixel *pix1, pixel *pix2 )
    {
        /*
        * 8x8 宏块被划分为4个4x4子块
        *
        * +---+---+
        * | 0 | 1 |
        * +---+---+
        * | 2 | 3 |
        * +---+---+
        *
        */
        sub4x4_dct( dct[0], &pix1[0], &pix2[0] );
        sub4x4_dct( dct[1], &pix1[4], &pix2[4] );
        sub4x4_dct( dct[2], &pix1[4*FENC_STRIDE+0], &pix2[4*FDEC_STRIDE+0] );
        sub4x4_dct( dct[3], &pix1[4*FENC_STRIDE+4], &pix2[4*FDEC_STRIDE+4] );
    }
    //Hadamard变换
    static void dct4x4dc( dctcoef d[16] )
    {
        dctcoef tmp[16];
    
        //蝶形算法：横向的4个像素
        for( int i = 0; i < 4; i++ )
        {
    
            int s01 = d[i*4+0] + d[i*4+1];
            int d01 = d[i*4+0] - d[i*4+1];
            int s23 = d[i*4+2] + d[i*4+3];
            int d23 = d[i*4+2] - d[i*4+3];
    
            tmp[0*4+i] = s01 + s23;
            tmp[1*4+i] = s01 - s23;
            tmp[2*4+i] = d01 - d23;
            tmp[3*4+i] = d01 + d23;
        }
        //蝶形算法：纵向
        for( int i = 0; i < 4; i++ )
        {
            int s01 = tmp[i*4+0] + tmp[i*4+1];
            int d01 = tmp[i*4+0] - tmp[i*4+1];
            int s23 = tmp[i*4+2] + tmp[i*4+3];
            int d23 = tmp[i*4+2] - tmp[i*4+3];
    
            d[i*4+0] = ( s01 + s23 + 1 ) >> 1;
            d[i*4+1] = ( s01 - s23 + 1 ) >> 1;
            d[i*4+2] = ( d01 - d23 + 1 ) >> 1;
            d[i*4+3] = ( d01 + d23 + 1 ) >> 1;
        }
    }
    ```

- **量化相关源码**
    ```C
    //量化
    void x264_quant_init( x264_t *h, int cpu, x264_quant_function_t *pf )
    {
        //这个好像是针对8x8DCT的
        pf->quant_8x8 = quant_8x8;
    
        //量化4x4=16个
        pf->quant_4x4 = quant_4x4;
        //注意：处理4个4x4的块
        pf->quant_4x4x4 = quant_4x4x4;
        //Intra16x16中，16个DC系数Hadamard变换后对的它们量化
        pf->quant_4x4_dc = quant_4x4_dc;
        pf->quant_2x2_dc = quant_2x2_dc;
        //反量化4x4=16个
        pf->dequant_4x4 = dequant_4x4;
        pf->dequant_4x4_dc = dequant_4x4_dc;
        pf->dequant_8x8 = dequant_8x8;
    
        pf->idct_dequant_2x4_dc = idct_dequant_2x4_dc;
        pf->idct_dequant_2x4_dconly = idct_dequant_2x4_dconly;
    
        pf->optimize_chroma_2x2_dc = optimize_chroma_2x2_dc;
        pf->optimize_chroma_2x4_dc = optimize_chroma_2x4_dc;
    
        pf->denoise_dct = x264_denoise_dct;
        pf->decimate_score15 = x264_decimate_score15;
        pf->decimate_score16 = x264_decimate_score16;
        pf->decimate_score64 = x264_decimate_score64;
    
        pf->coeff_last4 = x264_coeff_last4;
        pf->coeff_last8 = x264_coeff_last8;
        pf->coeff_last[  DCT_LUMA_AC] = x264_coeff_last15;
        pf->coeff_last[ DCT_LUMA_4x4] = x264_coeff_last16;
        pf->coeff_last[ DCT_LUMA_8x8] = x264_coeff_last64;
        pf->coeff_level_run4 = x264_coeff_level_run4;
        pf->coeff_level_run8 = x264_coeff_level_run8;
        pf->coeff_level_run[  DCT_LUMA_AC] = x264_coeff_level_run15;
        pf->coeff_level_run[ DCT_LUMA_4x4] = x264_coeff_level_run16;
    
    #if HIGH_BIT_DEPTH
    #if HAVE_MMX
        INIT_TRELLIS( sse2 );
        if( cpu&X264_CPU_MMX2 )
        {
    #if ARCH_X86
            pf->denoise_dct = x264_denoise_dct_mmx;
            pf->decimate_score15 = x264_decimate_score15_mmx2;
            pf->decimate_score16 = x264_decimate_score16_mmx2;
            pf->decimate_score64 = x264_decimate_score64_mmx2;
            pf->coeff_last8 = x264_coeff_last8_mmx2;
            pf->coeff_last[  DCT_LUMA_AC] = x264_coeff_last15_mmx2;
            pf->coeff_last[ DCT_LUMA_4x4] = x264_coeff_last16_mmx2;
            pf->coeff_last[ DCT_LUMA_8x8] = x264_coeff_last64_mmx2;
            pf->coeff_level_run8 = x264_coeff_level_run8_mmx2;
            pf->coeff_level_run[  DCT_LUMA_AC] = x264_coeff_level_run15_mmx2;
            pf->coeff_level_run[ DCT_LUMA_4x4] = x264_coeff_level_run16_mmx2;
    #endif
            pf->coeff_last4 = x264_coeff_last4_mmx2;
            pf->coeff_level_run4 = x264_coeff_level_run4_mmx2;
            if( cpu&X264_CPU_LZCNT )
                pf->coeff_level_run4 = x264_coeff_level_run4_mmx2_lzcnt;
        }
        //此处省略大量的X86、ARM等平台的汇编函数初始化代码
    }

    typedef struct
    {
        int (*quant_8x8)  ( dctcoef dct[64], udctcoef mf[64], udctcoef bias[64] );
        int (*quant_4x4)  ( dctcoef dct[16], udctcoef mf[16], udctcoef bias[16] );
        int (*quant_4x4x4)( dctcoef dct[4][16], udctcoef mf[16], udctcoef bias[16] );
        int (*quant_4x4_dc)( dctcoef dct[16], int mf, int bias );
        int (*quant_2x2_dc)( dctcoef dct[4], int mf, int bias );
    
        void (*dequant_8x8)( dctcoef dct[64], int dequant_mf[6][64], int i_qp );
        void (*dequant_4x4)( dctcoef dct[16], int dequant_mf[6][16], int i_qp );
        void (*dequant_4x4_dc)( dctcoef dct[16], int dequant_mf[6][16], int i_qp );
    
        void (*idct_dequant_2x4_dc)( dctcoef dct[8], dctcoef dct4x4[8][16], int dequant_mf[6][16], int i_qp );
        void (*idct_dequant_2x4_dconly)( dctcoef dct[8], int dequant_mf[6][16], int i_qp );
    
        int (*optimize_chroma_2x2_dc)( dctcoef dct[4], int dequant_mf );
        int (*optimize_chroma_2x4_dc)( dctcoef dct[8], int dequant_mf );
    
        void (*denoise_dct)( dctcoef *dct, uint32_t *sum, udctcoef *offset, int size );
    
        int (*decimate_score15)( dctcoef *dct );
        int (*decimate_score16)( dctcoef *dct );
        int (*decimate_score64)( dctcoef *dct );
        int (*coeff_last[14])( dctcoef *dct );
        int (*coeff_last4)( dctcoef *dct );
        int (*coeff_last8)( dctcoef *dct );
        int (*coeff_level_run[13])( dctcoef *dct, x264_run_level_t *runlevel );
        int (*coeff_level_run4)( dctcoef *dct, x264_run_level_t *runlevel );
        int (*coeff_level_run8)( dctcoef *dct, x264_run_level_t *runlevel );
    
    #define TRELLIS_PARAMS const int *unquant_mf, const uint8_t *zigzag, int lambda2,\
                        int last_nnz, dctcoef *coefs, dctcoef *quant_coefs, dctcoef *dct,\
                        uint8_t *cabac_state_sig, uint8_t *cabac_state_last,\
                        uint64_t level_state0, uint16_t level_state1
        int (*trellis_cabac_4x4)( TRELLIS_PARAMS, int b_ac );
        int (*trellis_cabac_8x8)( TRELLIS_PARAMS, int b_interlaced );
        int (*trellis_cabac_4x4_psy)( TRELLIS_PARAMS, int b_ac, dctcoef *fenc_dct, int psy_trellis );
        int (*trellis_cabac_8x8_psy)( TRELLIS_PARAMS, int b_interlaced, dctcoef *fenc_dct, int psy_trellis );
        int (*trellis_cabac_dc)( TRELLIS_PARAMS, int num_coefs );
        int (*trellis_cabac_chroma_422_dc)( TRELLIS_PARAMS );
    } x264_quant_function_t;
    
    //4x4量化
    //输入输出都是dct[16]
    //quant_4x4()用于对4x4的DCT残差矩阵进行量化。该函数的定义位于common\quant.c，如下所示。
    static int quant_4x4( dctcoef dct[16], udctcoef mf[16], udctcoef bias[16] )
    {
        int nz = 0;
        //循环16个元素
        for( int i = 0; i < 16; i++ )
            QUANT_ONE( dct[i], mf[i], bias[i] );
        return !!nz;
    }
    
    //量化1个元素
    #define QUANT_ONE( coef, mf, f ) \
    { \
        if( (coef) > 0 ) \
            (coef) = (f + (coef)) * (mf) >> 16; \
        else \
            (coef) = - ((f - (coef)) * (mf) >> 16); \
        nz |= (coef); \
    }
    
    
    ```





