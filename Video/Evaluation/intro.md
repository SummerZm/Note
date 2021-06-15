## **音视频质量评价方法**

### **A. 音视频质量评价背景**
- 视频编码器【编码标准】性能评测
- 视频质量检测
    ```sh
    # 主要用于实时监测分析视频流。例如监测CMMB，数字电视，IPTV，互联网视频等这些信号的质量。
    # 当这些信号出现异常的时候，可以反馈给服务提供商，使之及时采取相应的措施，修复视频传输过程中出现的问题。
    ```

### **B. 音视频质量评级分类**
- 主观音视频质量评价 (Subjective Quality Assessment，SQA)
- 客观音视频质量评价 (Objective Quality Assessment，OQA)
    ```sh
    # 全参考(FullReference，FR)
        # A. 均方差 MSE      【原始视频和参考视频的保真度】
        # B. 峰值信噪比 PSNR 【均方差的对数表示形式】
        # C. SSIM           【模拟人体视觉器官，对视频质量的损失进行评估】
    # 部分参考(ReducedReference，RR)
    # 无参考(No Reference，NR)
    ```
    ![assessment](./image/Assessment.jpg)  
    ![MSE](./image/MSE.jpg)
    ![PSNR](./image/PSNR.jpg)
    ![SSIM](./image/SSIM.jpg)

### **C. 质量评价涉及到的其它知识**
- 1. 视频数据处理
- 2. 实验结果处理
- 3. 视频序列复杂度

### **D. 质量评价做的比较好的机构**
- 1. VQEG 视频质量专家组
- 2. MSU Video Group MSU视频组

### **E. 视频质量评价资源**
- 1. 视频测试序列库
- 2. 图像测试序列库

### **F. 质量评价的已有研究**
- 1. 编码器(编码标准)性能测评
- 2. 学术研究（压缩编码与视频质量）
- 3. 其他因素（除了压缩编码之外的因素）与视频质量


