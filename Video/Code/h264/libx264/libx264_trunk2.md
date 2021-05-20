## **libx264主干接口代码调用**
> [雷神](https://blog.csdn.net/leixiaohua1020/article/details/45719905)

### x264_encoder_encode()编码一帧YUV为H.264码流。它调用了下面的函数：
- x264_frame_pop_unused()
    1. 获取1个x264_frame_t类型结构体fenc。
    2. 如果frames.unused[]队列不为空，就调用x264_frame_pop()从unused[]队列取1个现成的；
    3. 否则就调用x264_frame_new()创建一个新的。

- x264_frame_copy_picture()：将输入的图像数据拷贝至fenc。
- x264_lookahead_put_frame()：将fenc放入lookahead.next.list[]队列，等待确定帧类型。
- x264_lookahead_get_frames()：通过lookahead分析帧类型。
    1. 该函数调用了x264_slicetype_decide()x264_slicetype_analyse()和x264_slicetype_frame_cost()等函数。
    2. 经过一些列分析之后，最终确定了帧类型信息，并且将帧放入frames.current[]队列。

- x264_frame_shift()：从frames.current[]队列取出1帧用于编码。
- x264_reference_update()：更新参考帧队列。
- x264_reference_reset()：如果为IDR帧，调用该函数清空参考帧列表。
- x264_reference_hierarchy_reset()：如果是非IDR的I帧、P帧、B帧（可做为参考帧），调用该函数。
- x264_reference_build_list()：创建参考帧列表list0和list1。
- x264_ratecontrol_start()：开启码率控制。
- x264_slice_init()：创建 Slice Header。
- x264_slices_write()：编码数据（最关键的步骤）。
    1. 调用了x264_slice_write()完成了编码的工作（注意“x264_slices_write()”和“x264_slice_write()”名字差了一个“s”）。
    
- x264_encoder_frame_end()：编码结束后做一些后续处理，例如记录一些统计信息。
    1. 调用了x264_encoder_encapsulate_nals()封装NALU（添加起始码）
    2. 调用x264_frame_push_unused()将fenc重新放回frames.unused[]队列
    3. 调用x264_ratecontrol_end()结束码率控制。


