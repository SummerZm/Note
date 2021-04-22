## **C++中New操作符的实现**
> [相关链接](!https://blog.csdn.net/weixin_30905981/article/details/99029403?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_title-1&spm=1001.2101.3001.4242)


### **A. New 简介** 
- **“new”是C++的一个关键字，同时也是操作符**
- **new的过程**
    ```sh
    # 当我们使用关键字new在堆上动态创建一个对象时
    # A. 获得一块内存空间、调用构造函数、返回正确的指针。
    # B, 如果创建的是简单类型的变量，那么第二步会被省略。
    ```

### **B. New 的三种形态**
- **new operator (new expression)【正常流程】**
    ```sh
    # new operator就是我们平时所使用的new，其行为就是前面所说的三个步骤，我们不能更改它。
    # 但具体到某一步骤中的行为，如果它不满足我们的具体要求时，我们是有可能更改它的。
    # 三个步骤中最后一步只是简单的做一个指针的类型转换。
    ```
- **operator new【重载分配内存的方式】**
    ```sh
    # new operator的第一步分配内存【通过调用operator new来完成】
    # operator new默认情况下首先调用分配内存的代码，尝试得到一段堆上的空间，如果成功就返回，
    # 如果失败，则转而去调用一个new_hander，然后继续重复前面过程。
    # 如果我们对这个过程不满意，就可以重载operator new，来设置我们希望的行为。例如：

    class A
    {
    public:
    void* operator new(size_t size)
    {
        printf("operator new called\n");
        return ::operator new(size);
    }
    };
    A* a = new A();

    # 这里通过::operator new调用了原有的全局的new，实现了在分配内存之前输出一句话。
    # 全局的operator new也是可以重载的，但这样一来就不能再递归的使用new来分配内存，而只能使用malloc了：

    void* operator new(size_t size)
    {
    printf("global new\n");
        return malloc(size);
    }
    
    # 如果重载了operator new，就应该也相应的重载operator delete，这是良好的编程习惯。
    ```

- **placement new【在分配的内存上构造对象】**
    ```sh
    # new的第三种形态——placement new是用来实现定位构造的，因此可以实现new operator三步操作中的第二步。
    # 功能：在取得了一块可以容纳指定类型对象的内存后，在这块内存上构造一个对象。

    #include <new.h>
    void main()
    {
        char s[sizeof(A)];
        A* p = (A*)s;
        new(p) A(3); //p->A::A(3);  //  在内存P上构建对象
        p->Say();
    }

    # 对头文件<new>或<new.h>的引用是必须的，这样才可以使用placement new。
    # 这里“new(p) A(3)”这种奇怪的写法便是placement new了。
    # 它实现了在指定内存地址上用指定类型的构造函数来构造一个对象的功能，后面A(3)就是对构造函数的显式调用。
    
    # 指定的地址既可以是栈，又可以是堆，placement对此不加区分。
    # 除非特别必要，不要直接使用placement new ，这毕竟不是用来构造对象的正式写法，只不过是new operator的一个步骤而已。
    # 使用new operator地编译器会自动生成对placement new的调用的代码，因此也会相应的生成使用delete时调用析构函数的代码。
    # 如果是像上面那样在栈上使用了placement new，则必须手工调用析构函数，这也是显式调用析构函数的唯一情况。

    p->~A();

    # 当我们觉得默认的new operator对内存的管理不能满足我们的需要，而希望自己手工的管理内存时，placement new就有用了。
    # STL中的allocator就使用了这种方式，借助placement new来实现更灵活有效的内存管理。
    ```

