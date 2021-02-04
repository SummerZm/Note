## **RCU [Read-Copy-Update] 相关**
> **[RCU用于保护共享数据，读者不需要获取锁就可以访问，写者需要拷贝一个副本，然后等待一个时机回写](https://blog.csdn.net/andylauren/article/details/70233241https://blog.csdn.net/andylauren/article/details/70233241)**

### **引子**
- **读写逻辑常见问题**
- **代码框架流程的间隙**
- **了解他人如何处理异步场景问题**

### **个人理解与问题**
- **A. 数据是保存内存或者cache中的，CPU读取被RCU保护的数据时，为什么不能发生上下文切换？**
- **B. 对的**

### **RCU简介**
- **A. RCU是一种内核数据保护机制**
- **B. RCU允许多个读者和多个写者同时访问被保护的数据【多个写者同时访问需要写者之间提供数据同步机制】**
- **C. 适用于读多写少应用场景**
- **D. 超级重要：读者在读取数据的时候不能被阻塞【即：读者所在CPU不能进行上下文切换】**
    ```sh
    # 1. CPU读取数据一般是从register, level one cache, level two cache，最后才是内存获取数据。
    # 2. C言语 volatile 关键词用于从内存获取数据。
    # 3. 不让读者所在CPU进行切换，应该是为了保护 cache 中的数据不被修改。
    ```
- **E. 写者在访问被RCU保护的共享数据时不需要和读者竞争任何锁，只有在有多于一个写者的情况下需要获得某种锁以与其他写者同步。**
    ```sh
    # 1. 写者是将被RCU保护数据拷贝一份[这是一个读操作], 然后在拷贝的数据上进行修改，不会修改到原始数据。
    # 2. 多写者之间同步实际上是：同步多个写者各自的拷贝数据。
    ```
- **F. 写者等待回写的时机称为grace period; CPU发生上下文切换称为quiescent state; grace period就是所有CPU都是经过一次quiescent state所需等待的时间**
    ```sh
    # 1. 读者会禁用上下文切换。所有CPUi经过一次上下文切换，说明读者已经全部退出。【CPU之间的上下文切换应该是不一致的，这样写者的回写速度很很慢】
    ```

### **RCU内核使用例程**  
- **RCU内核使用例程**
    ```C
    struct foo {
    int a;
        char b;
        long c;
    };

    DEFINE_SPINLOCK(foo_mutex);
    struct foo *gbl_foo;

    void foo_update_a(int new_a)
    {
        struct foo *new_fp;
        struct foo *old_fp;
        new_fp = kmalloc(sizeof(*new_fp), GFP_KERNEL);
        // 内核编程中经常要使用到自旋锁，因为不像用户态有虚拟内存空间保护。
        // 若此处发生了抢占，则内存数据很容易，被修改。
        spin_lock(&foo_mutex);
        old_fp = gbl_foo;
        *new_fp = *old_fp;
        new_fp->a = new_a;
        rcu_assign_pointer(gbl_foo, new_fp);
        spin_unlock(&foo_mutex);
        synchronize_rcu();
        kfree(old_fp);
    }
    ```

### **RCU API实现分析**
- **RCU API实现分析**
    ```C
    #define rcu_read_lock() __rcu_read_lock() 
    #define rcu_read_unlock() __rcu_read_unlock() 
    // 保持一个读者的RCU临界区.在该临界区内不允许发生上下文切换 
    #define __rcu_read_lock() 
    do { 
        preempt_disable();  // 禁止抢占
        __acquire(RCU);     // 编译性选择函数
        rcu_read_acquire(); // 编译性选择函数
    } while (0) 
    #define __rcu_read_unlock() 
    do { 
        rcu_read_release(); // 编译性选择函数
        __release(RCU);     // 编译性选择函数
        preempt_enable();   // 开启抢占
    } while (0)

    // 保护一个指针
    #define rcu_dereference(p) ({ 
        typeof(p) _________p1 = ACCESS_ONCE(p); // 编译时 volatile
        smp_read_barrier_depends();             // 内存屏障 
        (_________p1); 
    }) 
    // 修改一个指针
    #define rcu_assign_pointer(p, v) 
    ({ 
        if (!__builtin_constant_p(v) || ((v) != NULL))  
            smp_wmb();  // 内存屏障 - 解决内存乱序访问问题，保证cache的一致性
        (p) = (v); 
    })
    // 上面两个函数同时使用了内存屏障，可以让读者和写者同时看到指针的最新值
    ```