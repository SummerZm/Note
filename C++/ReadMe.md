
## **C++学习路标**
### **1. 基础C++**
- **C++98/03 标准中常用 C++ 语言特性和 stl 库**
    1. 指针和引用的概念
    2. 指针与内存关系
    3. 程序编译过程，静态链接库和动态链接库
    4. static、const、#define的用法和区别
    5. C和C++区别
    6. 内存中的栈和堆分配
    7. C++ 面向对象语法
    8. 访问限定符 public/protected/private
    9. 构造函数/析构函数/拷贝构造
    10. 多态
    11. 虚函数与纯虚函数、虚函数实现机制、虚函数表 继承原理、虚继承、菱形继承
    12. new/delete和malloc/free
    13. 重载、重写和覆盖
    14. 类型转换方式
    15. RAII 与 pimpl 惯用法
    16. 内存溢出和内存泄漏
    17. STL标准模板库
    18. 迭代器、空间配置器理解
    19. 常用容器特点、用法以及底层实现vector、list、deque、set、map、unorderedmap

- **推荐书籍**
    1. **《深度探索 C++ 对象模型》**
    2. **《C++ API 设计》**
    3. **《大规模 C++ 程序设计》**

### **2. 现代C++**
> **常用的 C++ 编码技巧，推荐阅读《提高C++性能的编程技术》**
- **Modern C++ 指的是 C++11 C++11、C++14、C++17、C++20.（同样包括但不局限于）**
    1. 统一的类成员初始化语法与 std::initializer_list
    2. 注解标签（attributes）
    3. final/override/=default/=delete 语法
    4. auto 关键字
    5. Range-based 循环语法
    6. 结构化绑定
    7. stl 容器新增的实用方法
    8. std::thread
    9. 线程局部存储 thread_local
    10. 线程同步原语 std::mutex、std::condition_variable 等
    11. 原子操作类
    12. 智能指针类
    13. std::bind/std::function

- **C++11 的改动**
    1. 废弃了C++98/03标准中一些不实用的语法和库（如std::auto_ptr）。
    2. 改进或者增强了C++ 98/03标准中一些语法元素的用法（如auto 关键字、统一类的初始化列表语法）。
    3. 新增了一些其他编程语言早已经支持的关键字和语法（如final关键字、=default语法 、=delete语法）。
    4. 开始在语法和自带的标准库的层面上增加对操作系统的功能的支持（如线程库、时间库）。
    5. C++语言本身是无法支持的，必须依赖使用原生的操作系统的API函数，导致开发者需要编写大量平台相关的代码。
        所以，与之前的 C++ 版本相比，C++11 更像一门新的语言。

- **C++14 的改动**
    1. C++14 是一个变动比较小的版本，主要是对 C++11 的新特性做了一些完善或者修正一些小的错误。
        ```sh
        #
        # 解决的问题1：
        #  对于 C++11 新增的智能指针类 std::shared_ptr、std::unique_ptr，C++11 新增了std::make_shared() 方法创建一个 std::shared_ptr 对象，
        #  却没有提供相应的 std::make_unique() 方法创建一个 std::unique_ptr 对象，直到 C++14 才被添加进来。
        #
        # 解决的问题2：
        #  std::unique_lock 在 C++11 引入，std::shared_lock 在 C++14 才引入。
        #
        ```

- **C++17 的改动**
    1. C++17 相对于 C++11 来说，是一个改动内容不多不少的版本：

- **C++20 的改动**
    1. C++20 进一步弥补 C++ 这门语言的“功能不完备性”。
    2. 所谓“功能不完备性”，即与 Java/Go 等语言相比，C++程序中的一些功能（如文件操作、协程等）需要直接调用操作系统的 API。
    3. 不像其他语言自带 SDK，这样对于要求多平台运行的程序，必须使用宏定义分别编写一套。
    4. 例如实现一个读写锁，不同平台需要调用不同的操作系统 API。

### **3. C++大工程设计**
> **如何结合 C++ 语言特性设计出优雅而鲁棒的 C++ API、如何优雅地组织和管理一个大型 C++ 工程**
- **基础C++相关工程书籍**     
    1. **《C++ API 设计》**
    2. **《大规模 C++ 程序设计》**

- **现代C++相关工程书籍**
    1. 《深入理解 C++11：C++11 新特性解析与应用》
    2. 《深入应用 C++11：代码优化与工程级应用》
    3. 《C++17 完全指南》
    4. 《Cpp 17 in Detail》

### **4. C++编译器支持情况**
- **MSVC 编译器**
    1. 支持 C++11 新标准的最低版本是VC++12（随Visual Studio 2013一起发布，Visual Studio 以下简称 VS）
    2. VS 2015 支持部分 C++14 特性，VS 2017 完美支持 C++14 和部分 C++17 特性
    3. VS 2019 完美支持 C++17 大多数语言特性

- **gcc/g++ 编译器**
    1. 支持 C++11 新标准的最低版本是 gcc/g++ 4.8
    2. 支持 C++14 是 gcc/g++ 4.9，gcc/g++ 7.3 完美支持 C++17 大多数语言特性
    3. 各个编译器对于 C++11/14/17 语法特性和标准库支持情况，完整列表可参考：
        https://en.cppreference.com/w/cpp/compiler_support。

### **现代 C++ 项目**
- Filezilla：是一款开源的 FTP 软件，其源码下载地址如下：链接: https://pan.baidu.com/s/1Efr8mEzRLgFqFvmRfX2z4Q 密码: rufj
- uWebSocket 网络库：最新版使用了大量 C++17 的语法，美中不足的是这个库代码存在不少 bug。https://github.com/uNetworking/uWebSockets
- TeamTalk 的 PC 端：蘑菇街开源的一款用于企业内部的即时通信工具。https://github.com/balloonwj/TeamTalk/tree/master/win-client
- Flamingo IM https://github.com/balloonwj/flamingo

[原网址](https://mp.weixin.qq.com/s/pAdzli-mhih9OXPUw8_TUA)


