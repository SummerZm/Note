## **[TCP常见问题](!https://mp.weixin.qq.com/s/ZBk0A0mvase6Mwa5tFSaQg)**
> **从问题出发才能更好的理解实现.**

### **A. TCP是用来解决什么问题？**
1. **控制可靠，按序传输，端与端之间的流量控制。同时考虑网络的整体情况，拥塞控制。**

### **B. 为什么要TCP，IP 层实现控制不行么？**
1. IP层功能方便所有设备寻址，而通信控制的方式有多种，是可选的; 并不是所有设备需要可靠，按序的传输

### **C. 三次握手的目的？**
1. **同步发送序号【ISN安全问题】**
2. **确认双方收发能力**

### **D. 初始序列号 ISN 的取值？**
1. **ISN泄漏的严重后果**
    ```sh
    # a. 身份仿冒
    #   X首先对Client进行攻击（比如Syn Flood），导致C不可用。
    #   然后X仿冒Client的地址对Server发起连接请求。
    #   Server对Client进行回应，附带ISN。注意：这个报文X是收不到的。
    #   X可以预测ISN，可以按预测的ISN直接给S回应确认，这时Server误认为已经和Client建立了连接。
    #   X这时就可以仿冒Client的地址，发送恶意指令给Server，Server会认为这是Client下发的指令，被欺骗执行，攻击生效。
    
    # b. DoS攻击
    #   发送序列号正确的ack报文，导致很多正确的报文被丢弃，TCP连接看上起正常，但是因为很多正确的报文被丢弃，实际处于拒绝服务状态。
    #   发送序列号正确的fin报文，导致TCP连接关闭，系统处于拒绝服务状态。

    # c. 信息投毒
    #   因为X可以预测ISN，就可以在通信过程中，假冒其中一方，使用正确的序列号抢先发送非法内容，而正确的报文在到达时被认为是重复的无效报文被丢弃。
    #   这种攻击需要对双方通信协议的内容有一定的了解，塞入的报文符合原来通信规格要求，被认为是合法报文进行了处理。
    #
    ```

2. **ISN取值算法很重要吗？**
    ```sh
    # ISN 生成算法1：【RFC793】
    # ISN 每四微秒加1，当超过 2 的 32 次方之后又从 0 开始，要四个半小时左右发生 ISN 回绕。ISN 变成一个递增值，真实的实现还需要加一些随机值在里面，防止被不法份子猜到 ISN 

    # ISN 生成算法2：【RFC1948[1996年]】
    # 通过使用HASH单向加密函数处理ISN，能够使远程攻击者无从下手，但不能阻止同一网段的攻击者通过监听网络上的数据来判断ISN。

    # ISN 生成算法3：【RFC1948[1996年]】
    # 建立TCP连接的五元组区分序列号空间，每一个五元组由本地地址、本地端口、远程地址、远程端口来组成，由一个函数对这4个参数进行计算，得到唯一的序列号地址空间偏移值。
    # 注意这个函数不能被攻击者获得，否则攻击者就也可以通过计算得到ISN，ISN就在这个偏移值上增加
    ```

### **E. SYN 超时了怎么处理？**
1. client 发送 SYN 至 server 然后就挂了，此时 server 发送 SYN+ACK 就一直得不到回复，怎么办？
    ```sh
    # 在 Linux 中就是默认重试 5 次，并且就是阶梯性的重试
    # 间隔就是1s、2s、4s、8s、16s，再第五次发出之后还得等 32s 才能知道这次重试的结果
    # 所以说总共等63s 才能断开连接。
    ```

2. SYN Flood 攻击
    ```sh
    # 解法1：开启 tcp_syncookies，不用SYN 队列
    # SYN 队列满了之后 TCP 根据自己的 ip、端口、然后对方的 ip、端口，对方 SYN 的序号，时间戳等一波操作生成一个特殊的序号（即 #cookie）发回去，
    # 如果对方是正常的 client 会把这个序号发回来，然后 server 根据这个序号建连。
       
    # 解法2：调整 tcp_synack_retries 减少重试的次数
    # 解法3：设置 tcp_max_syn_backlog 增加 SYN 队列数
    # 解法4：设置tcp_abort_on_overflow SYN 队列满了直接拒绝连接。
    ```

### **F. 四次挥手的变迁过程有哪些？**
1. **四次挥手一定要经历过 FIN_WAIT_2， CLOSE_WAIT，LAST_ACK状态码？**
    ```sh
    # Client / Sever 先后关闭
    # 客户端TCP状态迁移:
    # CLOSED -> SYN_SENT -> ESTABLISHED -> FIN_WAIT_1 -> FIN_WAIT_2 -> TIME_WAIT -> CLOSED
    # 服务器TCP状态迁移:
    # CLOSED -> LISTEN -> SYN收到 -> ESTABLISHED -> CLOSE_WAIT -> LAST_ACK -> CLOSED

    # 四次挥手一定要经历过四次通信吗？
    # 如果通信两端都没有数据要发，先后关闭连接，则四次握手会退化成三次握手
    # 因为收到Fin的一方回复ACK+FIN1合并包

    # Client / Sever 同时关闭
    # 客户端TCP状态迁移:
    # CLOSED -> SYN_SENT -> ESTABLISHED -> FIN_WAIT_1 -> CLOSEING -> TIME_WAIT -> CLOSED
    # 服务器TCP状态迁移:
    # CLOSED -> LISTEN -> SYN收到 -> FIN_WAIT_1 -> CLOSEING -> TIME_WAIT -> CLOSED
    ```

### **G. TIME_WAIT问题集合**
1.  为什么要有 TIME_WAIT?
    ```sh
    # a. 等待最后1个ACK包
    # b. 等待网络上残留的网络包消亡
    ```
2.  等待 2MSL 会产生什么问题？ 大量未关闭接口，浪费资源.

3.  如何解决 2MSL 产生的问题？
    ```sh
    # A. 快速回收：【NAT的局限被弃用】  
    #   Linux: tcp_timestamps【默认打开】和 tcp_tw_recycle【4.12内核废弃了这个参数 - NAT问题】
    #   tcp_tw_recycle 开启可以将 2MSL 时间缩短至一个重传时间。需要配合 tcp_timestamps使用
    #
    # B. 重用：【用于客户端的设置对服务端负载无效】
    #   Linux: tcp_tw_reuse 用于客户端发起连接时，可以快速复用客户端自身处于TIME_WAIT 1s以上的socket资源，对服务器没有大的减负作用
    #
    # C. 误区：【SO_REUSEADDR 能解决服务端的 TIME_WAIT问题】
    #   SO_REUSEADDR 主要用在你启动服务的时候，如果此时的端口被占用了并且这个连接处于 TIME_WAIT 状态，
    #   那么你可以重用这个端口，如果不是 TIME_WAIT，那就是给你个 Address already in use。
    #   
    # D. 解决方案：
    #   a. 调小MSL时间
    #   b. 调整 tcp_max_tw_buckets 默认值180000，已经很大
    #   c. 客户端主动关闭soket, 服务端不主动关闭
    #   d. 使用IP_TRANSPARENT选项模式启动一个新的服务用于假装TIME_WAIT链接的客户端，向自己服务器发请求；
    #       服务器会回复真客户端ACK，真客户端收到这个奇怪的ACK会回复RST重置连接【自己杀死自己】 
    #
    ```






