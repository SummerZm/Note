## libuv简介
> 高性能网络编程这个话题已经被讨论烂了。异步，异步，还是异步。不管是 epoll 也好，kqueue 也罢，总是免不了异步这个话题。

### libev
- libev 是系统异步模型的简单封装，基本上来说，它解决了 epoll ，kqueuq 与 select 之间 API 不同的问题。
- 保证使用 livev 的 API 编写出的程序可以在大多数 *nix 平台上运行。
- libev 的缺点也是显而易见，由于基本只是封装了 Event Library，用起来有诸多不便。
    1. 比如 accept(3) 连接以后需要手动 setnonblocking 。
    2. 从 socket 读写时需要检测 EAGAIN 、EWOULDBLOCK 和 EINTER 。这也是大多数人认为异步程序难写的根本原因。

### libuv
- libuv 是 joyent 给 Node 做的一套 I/O Library 。
- 这导致了 libuv 最大的特点就是处处回调。基本上只要有可能阻塞的地方，libuv 都使用回调处理。这样做实际上大大减轻了程序员的工作量。
- 因为当回调被 call 的时候，libuv 保证你有事可做
- 这样 EAGAIN 和 EWOULDBLOCK 之类的 handle 就不是程序员的工作了，libuv 会默默的帮你搞定。

### libev vs libuv
- 读写事件上的差异 
    1. libev 在 socket 发生读写事件时，只告诉你，“XX socket 可以读/写了，自己看着办吧”。我们需要自己申请内存并调用 read(3) 或者 write(3) 来响应 I/O 事件。
    2. libuv 分读/写两个部分来描述
        ```sh
        # 接口可读时，libuv 会调用你的 allocate callback 来申请内存并将读到的内容写入。
        # 当读取完毕后，libuv 会 call 你为这个 socket 设置的回调函数，在参数中带着这个 buffer 的信息。
        # 你只需要负责处理这个 buffer 并且free 掉就OK了。
        # 因为是从 buffer 中读取数据，在你的 callback 被调用时数据已经 ready 了，所以程序员也就不用考虑阻塞的问题了【EAGAIN 和 EWOULDBLOCK】。

        # 写的处理则更显巧妙。libuv 没有 write callback ，如果你想写东西，直接 generate 一个 write request 连着要写的 buffer 一起丢给 libuv
        # libuv 会把你的 write request 加进相应 socket 的 write queue ，在 I/O 可写时按顺序写入。
        ```

    3. 没有闭包，所以确定读写上下文是 libuv 的使用者需要面对的问题。否则程序面对汹涌而来的 buffer 也不能分得清哪个是哪个的数据。
        ```sh
        # 这里隐晦的潜台词是: 闭包是解决上下文问题的一种方式, 如果，用C言语阶实现闭包功能

        # 在这一点的处理上，libuv 跟 libev 一样，都是使用了一个 void *data 来解决问题。 
        # 可以用 data 这个 member 存储任何东西，这样当 buffer 来的时候，只需要简单的把 data cast 到你需要的类型就 OK 了。 
        ```

- 其他差异
    1. libev 没有异步 DNS 解析，这一点一直广为垢病。
    2. libuv 有异步的 DNS 解析，解析结果也是通过回调的方式通知程序。

    3. libev 完全是单线程的。
    4. libuv 需要多线程库支持，因为其在内部维护了一个线程池来 handle 诸如 getaddrinfo(3) 这样的无法异步的调用。

    5. libev 不支持 IOCP ，如果需要在 Win 下运行的程序会很麻烦。
    6. libuv 支持 IOCP ，有相应脚本编译 Win 下的库。


原文：https://idndx.com/2013/01/05/comparison-between-libuv-and-libev/

