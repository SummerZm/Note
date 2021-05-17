## **libx264 命令行代码解析**
> **如何阅读代码，通过恰当的理解传参消灭全局变量**

### **L0: Main() 函数调用**
- parse()解析输入的命令行参数
- encode()进行编码

### **L1: parse()函数调用**
> **调用位置：main() : if( parse( argc, argv, ¶m, &opt ) < 0 )**

- x264_param_default(): 存储参数的结构体x264_param_t赋默认值
- x264_param_default_preset(): x264_param_t赋值
- help()
- print_version_info()
- x264_param_parse(): 对于长选项，调用x264_param_parse()进行处理
- select_input(): 解析输出文件格式（例如raw，flv，MP4…）
- select_output(): 解析输入文件格式（例如yuv，y4m…）

### **L1: encode()函数调用**
> **调用位置：main() : ret = encode( &param, &opt);**

- **x264_encoder_open()** : 打开H.264编码器。
- x264_encoder_parameters(): 获得当前的参数集x264_param_t，用于后续步骤中的一些配置。【传参拷贝命令行部分参数到 x264_t h 变量中】
- 调用输出格式（H.264裸流、FLV、mp4等）对应cli_output_t结构体的set_param()方法，为输出格式的封装器设定参数。其中参数源自于上一步骤得到的x264_param_t
    ```sh
    # 1. cli_output_t结构体set_param 是一个函数指针
    ```

- 如果不是在每个keyframe前面都增加SPS/PPS/SEI的话，就调用 **x264_encoder_headers()** 在整个码流前面加SPS/PPS/SEI。
    ```sh
    # 1. SPS：序列参数集
    # 2. PPS：图像参数集
    # 3. SEI：辅助增强信息
    ```

- 在循环中，调用输入格式（YUV、Y4M等）对应的cli_vid_filter_t结构体get_frame()方法，获取一帧YUV数据。一帧一帧的将YUV编码为H.264
    ```sh
    # 1. cli_vid_filter_t结构体get_param 是一个函数指针
    ```

- 调用encode_frame()编码该帧YUV数据为H.264数据，并且输出出来。
    ```sh
    # 1. 调用 **x264_encoder_encode()** 完成编码工作
    # 2. 调用输出格式对应cli_output_t结构体的write_frame()完成了输出工作。
    # 3. cli_output_t结构体的write_frame()是一个函数指针
    ```

- 调用输入格式（YUV、Y4M等）对应的cli_vid_filter_t结构体release_frame()方法，释放刚才获取的YUV数据。
- 调用print_status()输出一些统计信息。
- 编码即将结束的时候，进入另一个循环，输出编码器中缓存的视频帧：
- 不再传递新的YUV数据，直接调用encode_frame()，将编码器中缓存的剩余几帧数据编码输出出来。
- 调用print_status()输出一些统计信息。
- 调用 **x264_encoder_close()** 关闭H.264编码器。

### **重要：数据流输入输出结构体**
>  **源码中为不同的 input,output,filter 维护着一个全局对象**

- **cli_output_t**：输出格式对应的结构体。输出格式一般为H.264裸流、FLV、MP4等。
    ```c
    typedef struct
    {
        int (*open_file)( char *psz_filename, hnd_t *p_handle, video_info_t *info, cli_input_opt_t *opt );
        int (*picture_alloc)( cli_pic_t *pic, int csp, int width, int height );
        int (*read_frame)( cli_pic_t *pic, hnd_t handle, int i_frame );
        int (*release_frame)( cli_pic_t *pic, hnd_t handle );
        void (*picture_clean)( cli_pic_t *pic );
        int (*close_file)( hnd_t handle );
    } cli_input_t;
    
    extern const cli_input_t raw_input;
    extern const cli_input_t y4m_input;
    extern const cli_input_t avs_input;
    extern const cli_input_t lavf_input;
    extern const cli_input_t ffms_input;
    ```

- **cli_input_t**：输入格式对应的结构体。输入格式一般为纯YUV像素数据，Y4M格式数据等。
    ```c
    typedef struct
    {
        int (*open_file)( char *psz_filename, hnd_t *p_handle, cli_output_opt_t *opt );
        int (*set_param)( hnd_t handle, x264_param_t *p_param );
        int (*write_headers)( hnd_t handle, x264_nal_t *p_nal );
        int (*write_frame)( hnd_t handle, uint8_t *p_nal, int i_size, x264_picture_t *p_picture );
        int (*close_file)( hnd_t handle, int64_t largest_pts, int64_t second_largest_pts );
    } cli_output_t;

    extern const cli_output_t raw_output;
    extern const cli_output_t mkv_output;
    extern const cli_output_t mp4_output;
    extern const cli_output_t flv_output;
    ```

- **cli_vid_filter_t**：输入格式滤镜结构体。滤镜可以对输入数据做一些简单的处理，例如拉伸、裁剪等等（也可以不作任何处理，直接读取输入数据）。
    ```c
    struct cli_vid_filter_t
    {
        /* name of the filter */
        const char *name;
        /* help: a short message on what the filter does and how to use it.
        * this should only be implemented by filters directly accessible by the user */
        void (*help)( int longhelp );
        /* init: initializes the filter given the input clip properties and parameter to adjust them as necessary
        * with the given options provided by the user.
        * returns 0 on success, nonzero on error. */
        int (*init)( hnd_t *handle, cli_vid_filter_t *filter, video_info_t *info, x264_param_t *param, char *opt_string );
        /* get_frame: given the storage for the output frame and desired frame number, generate the frame accordingly.
        * the image data returned by get_frame should be treated as const and not be altered.
        * returns 0 on success, nonzero on error. */
        int (*get_frame)( hnd_t handle, cli_pic_t *output, int frame );
        /* release_frame: frame is done being used and is signaled for cleanup.
        * returns 0 on succeess, nonzero on error. */
        int (*release_frame)( hnd_t handle, cli_pic_t *pic, int frame );
        /* free: run filter cleanup procedures. */
        void (*free)( hnd_t handle );
        /* next registered filter, unused by filters themselves */
        cli_vid_filter_t *next;
    };
    ```


