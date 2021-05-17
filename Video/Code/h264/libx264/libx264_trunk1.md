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
- x264_ratecontrol_new()：初始化码率控制相关的变量。
- mbcmp_init()：决定像素比较的时候使用SAD还是SATD。











