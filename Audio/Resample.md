## **声频重采样(Audio resamlp)**

- **A. 重采样的目的**
1. **在声纹识别中，为了满足对不同采样率的要求，常需要对语音进行重采样**
2. **重采样即将原始的采样频率变换为新的采样频率以适应不同采样率的要求**

- **B. 重采样的三种方式**
1. **原模拟信号x(t)可以再生，或是已记录下来，那么可以进行重新采样**
2. **将x(n)通过数模转换D/A变成模拟信号x(t)，对x(t)经模数转换A/D在重新采样**
3. **L/M倍采样率转换算法，对采样后的数字信号x(n)，在“数字域”做采样率转换，以得到新的采样率**
4. **总结比较**
    ```sh
    # 方法一: 所处理的情况比较特殊
    # 方法二: 再一次引入A/D和D/A量化误差
    # 方法三: 最为理想
    ```
- **C. 重采样的理论基础**
1. **音频重采样主要步骤是进行插值或抽取**
    ```sh
    # a. 抽取可能产生混叠，抽取前进行抗混叠滤波.
    # b. 内插可能产生镜像, 内插后进行抗镜像滤波.
    # c. 抗混叠滤波和抗镜像滤波都是使用低通滤波器实现.
    ```
2. **重采样例子**
    ```sh
    # a. 假设已对音频信号x(n)(1≤n≤N)以频率L进行采样，现在要对他以新的频率M进行采样，L和M都是实数
    # b. M可以比L大或者小。如果M>L，那么就是上采样，反之，就为下采样
    # c. 让y(k)(1≤k≤K)表示重采样后的信号，明显满足关系K=M/L*N
    #       问题1：多少个x(n)采样点去导出一个y(k)的采样点幅值；这便涉及到窗口大小的问题？
    #       答：每个音频信号的采样仅仅决定于一些与它邻近的采样值。实际上窗是非常小的，通常仅仅包含几个采样值
    #
    #       问题2：什么插值函数比较适合与如何估计逼近精度？
    #       答：内插函数和逼近函数是数学问题
    #           任意连续函数可以被任意均匀的多项式或三角多项式表示。
    #           逼近的精度可以用泰勒级数和原函数的高阶函数来估计。
    #           所以理论上可以选择合适的多项式和三角多项式作为音频重采样的插入函数。
    ```

