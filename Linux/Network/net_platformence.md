## <b>网络性能优化</b> ##

### <b>应用程序优化</b> ###
- I/O模型优化
    1. I/O 多路复用技术 epoll，主要用来取代 select 和 poll。
    2. I/O（Asynchronous I/O，AIO）。AIO 允许应用程序同时发起很多 I/O 操作。

- 工作模型
    1. 主进程 + 多个 worker 子进程。其中，主进程负责管理网络连接，而子进程负责实际的业务处理。
    2. 监听到相同端口的多进程模型。需要开启 SO_REUSEPORT 选项，由内核负责，把请求负载均衡到这些监听进程中去。

- 其他
    1. 使用长连接取代短连接
    2. 使用内存等方式，来缓存不常变化的数据
    3. 使用 Protocol Buffer 等序列化的方式，压缩网络 I/O 的数据量，可以提高应用程序的吞吐。
    4. 使用 DNS 缓存、预取、HTTPDNS 等方式，减少 DNS 解析的延迟，也可以提升网络 I/O 的整体速度。

- 套接字
    1. 增大每个套接字的缓冲区大小 net.core.optmem_max
    2. 增大套接字接收缓冲区大小 net.core.rmem_max 和发送缓冲区大小 net.core.wmem_max
    3. 增大 TCP 接收缓冲区大小 net.ipv4.tcp_rmem 和发送缓冲区大小 net.ipv4.tcp_wmem
    4. 参考数值：
        ![4.png](./4.png)
        ```
        - TCP 连接设置 TCP_NODELAY 可以禁用 Nagle 算法；
        - SO_SNDBUF 和 SO_RCVBUF可以分别调整套接字发送缓冲区和接收缓冲区的大小。
        - 为 TCP 连接开启 TCP_CORK 可以让小包聚合成大包后再发送（注意会阻塞小包的发送）；
        - tcp_rmem/tcp_wmem 的三个数值分别是min，default，max，系统会根据这些设置，自动调整TCP接收/发送缓冲区的大小。
        - udp_mem 的三个数值分别是min，pressure，max，系统会根据这些设置，自动调整 UDP 发送缓冲区的大小。
        ```

### <b>网络传输层优化</b> ###
- 大量 TIME_WAIT 问题分析
    1. TIME_WAIT是主动关闭连接的一方保持的状态，进入TIME_WAIT的状态，保持这个状态2MSL（max segment lifetime）时间之后，彻底关闭回收资源。
    2. 对方连接的异常，或自己没有迅速回收资源。
    3. 为啥还要保持资源一段时间呢？
        ```sh
        # 1. 防止上一次连接中的包，迷路后重新出现，影响新连接（经过2MSL，上一次连接中所有的重复包都会消失）
        # 2. 可靠的关闭TCP连接：在主动关闭方发送的最后一个ack(fin) 有可能丢失，这时被动方会重新发fin,
        #    如果这时主动方处于 CLOSED 状态 ，就会响应 rst 而不是 ack。
        #    所以主动方要处于 TIME_WAIT 状态，而不能是 CLOSED 
        # 3. TIME_WAIT会2MSL定时的回收资源，不存在挂死现象
        ```

- 大量 CLOSE_WAIT 问题分析
    1. 服务器保持了大量CLOSE_WAIT状态： 原因: 在对方关闭连接之后服务器程序死锁自己没有进一步发出ack信号;CPU太忙

- TCP 四次挥手过程
    1. 主动关闭连接的一方 – 也就是主动调用socket的close操作的一方，最终会进入TIME_WAIT状态.**[等待最后一个ACK]**
    2. 被动关闭连接的一方，有一个中间状态，即CLOSE_WAIT。**[因为协议层在等待上层的应用程序，主动调用close操作后才主动关闭这条连接]**
    3. TIME_WAIT会默认等待2MSL时间后，才最终进入CLOSED状态
    4. 在一个连接没有进入CLOSED状态之前，这个连接是不能被重用的
    5. 状态转移：
        ```sh
        # close()[Client]
        #   ------ FIN ------->
        #   FIN_WAIT1   CLOSE_WAIT
        #   <----- ACK -------
        #   FIN_WAIT2
        #               close() [Server]
        #   <------ FIN ------
        #   TIME_WAIT   LAST_ACK
        #   ------ ACK ------->
        #   CLOSED      CLOSED

        # 客户端TCP状态迁移:
        # CLOSED -> SYN_SENT -> ESTABLISHED -> FIN_WAIT_1 -> FIN_WAIT_2 -> TIME_WAIT -> CLOSED
        # 服务器TCP状态迁移:
        # CLOSED -> LISTEN -> SYN收到 -> ESTABLISHED -> CLOSE_WAIT -> LAST_ACK -> CLOSED
        ```
    
- 优化与 TIME_WAIT 状态相关的内核选项 -- [/etc/sysctl.conf]
    1. 增大处于 TIME_WAIT 状态的连接数量 net.ipv4.tcp_max_tw_buckets 
    2. 并增大连接跟踪表的大小 net.netfilter.nf_conntrack_max。
    3. 减小 net.ipv4.tcp_fin_timeout 和 net.netfilter.nf_conntrack_tcp_timeout_time_wait ，让系统尽快释放它们所占用的资源。
    4. 增大本地端口的范围 net.ipv4.ip_local_port_range 。这样就可以支持更多连接，提高整体的并发能力。
    5. fs.nr_open 和 fs.file-max ，分别增大进程和系统的最大文件描述符数；或在应用程序的 systemd 配置文件中，配置 LimitNOFILE ，设置应用程序的最大文件描述符数。
    6. 开启端口复用 net.ipv4.tcp_tw_reuse。这样，被 TIME_WAIT 状态占用的端口，还能用到新建的连接中。
    7. 开启tw_recylce和tw_reuse功能, 一定需要timestamps的支持。
    8. 1万条TIME_WAIT的连接，也就多消耗1M左右的内存，对现代的很多服务器，已经不算什么了。至于CPU，能减少它当然更好，但是不至于因为1万多个hash item就担忧。
        ```sh
        # net.ipv4.tcp_timestamps 【快捷回收基础选项】
        # RFC 1323 在 TCP Reliability一节里，引入了timestamp的TCP option，两个4字节的时间戳字段.
        # 其中第一个4字节字段用来保存发送该数据包的时间
        # 第二个4字节字段用来保存最近一次接收对方发送到数据的时间。
        # tcp_tw_reuse 和 tcp_tw_recycle就依赖这些时间字段。

        # net.ipv4.tcp_tw_reuse 【客户端快速回收选项】
        # 这个参数是reuse TIME_WAIT状态的连接。一条socket连接是个五元组
        # 应用场景：某一方，需要不断的通过“短连接“连接其他服务器，总是自己先关闭连接(TIME_WAIT在自己这方)，关闭后又不断的重新连接对方。
        # 
        # 当连接被复用了之后，延迟或者重发的数据包到达，新的连接怎么判断，到达的数据是属于复用后的连接，还是复用前的连接呢？
        # 复用连接后，这条连接的时间被更新为当前的时间，当延迟的数据达到，延迟数据的时间是小于新连接的时间，所以，内核可以通过时间判断出，延迟的数据可以安全的丢弃掉了。
        #
        # 要求连接双方同时对timestamps的支持。仅仅影响outbound连接，即做为客户端的角色，连接服务端[connect(dest_ip, dest_port)]时复用TIME_WAIT的socket。

        # net.ipv4.tcp_tw_recycle 【服务器端快速回收选项】
        # 销毁掉 TIME_WAIT。当开启了这个配置后，内核会快速的回收处于TIME_WAIT状态的socket连接。
        # 多快？不再是2MSL，而是一个RTO（retransmission timeout，数据包重传的timeout时间）的时间，这个时间根据RTT动态计算出来，但是远小于2MSL。
        #
        # 在启用该配置，当一个socket连接进入TIME_WAIT状态后，内核里会记录包括该socket连接对应的五元组中的对方IP等在内的一些统计数据，包括从该对方IP所接收到的最近的一次数据包时间。
        # 当有新的数据包到达，只要时间晚于内核记录的这个时间，数据包都会被统统的丢掉。【保障丢失重传或者延迟的数据包，不会被新的连接错误的接收】
        #
        # 要求连接双方对timestamps的支持。同时，这个配置，主要影响到了inbound的连接（对outbound的连接也有影响，但是不是复用）
        # 即做为服务端角色，客户端连进来，服务端主动关闭了连接，TIME_WAIT状态的socket处于服务端，服务端快速的回收该状态的连接。
        
        # 问题：如果客户端处于NAT的网络(多个客户端，同一个IP出口的网络环境)，如果配置了tw_recycle，就可能在一个RTO的时间内，只能有一个客户端和自己连接成功
        # (不同的客户端发包的时间不一致，造成服务端直接把数据包丢弃掉)。
        ```

- TCP 网络负载简析【系统层面的解决方案】
    1. 配置 /etc/sysctl.conf下的 net.ipv4.ip_local_port_range=1024 65535 【保证服务器至少可以使用6万个随机端口】
    2. 在1的条件下，可以支持每秒1000的并发（想一想，因为TIME_WAIT状态在linui下会持续1分钟后消失，所以一分钟最多有6万，每秒1000并发）
    3. 如果端口不够用。解决办法：A.加服务器; B.配置多个IP地址; C.后端服务器需要监听更多的端口和配置更多的IP 【原理：socket的五元组】

- TCP 内核设置信息获取
    1. 内核里有保存所有连接的一个hash table，包含TIME_WAIT状态的连接，也包含其他状态的连接。不同的内核hash table的大小不同。查看方式：
        ```sh
        [root@web01 ~]# dmesg |grep --color "TCP established hash table"
        TCP established hash table entries: 524288 (order: 11, 8388608 bytes)
        ```
    2. 有一个hash table用来保存所有的bound ports，主要用于可以快速的找到一个可用的端口或者随机端口：
        ```sh
        [root@web01 ~]# dmesg |grep --color "TCP bind hash table"
        TCP bind hash table entries: 65536 (order: 8, 1048576 bytes)
        ```

- 缓解SYN FLOOD等，利用TCP协议特点进行攻击而引发的性能问题
    1. 增大 TCP 半连接的最大数量 net.ipv4.tcp_max_syn_backlog
    2. 开启 TCP SYN Cookies net.ipv4.tcp_syncookies ，来绕开半连接数量限制的问题（注意，不可与net.ipv4.tcp_max_syn_backlog同时使用）。
    3. 减少 SYN_RECV 状态的连接重传 SYN+ACK 包的次数 net.ipv4.tcp_synack_retries。

- 在长连接的场景中，通常使用 Keepalive 来检测 TCP 连接的状态，以便对端连接断开后，可以自动回收。
    1. 缩短最后一次数据包到 Keepalive 探测包的间隔时间 net.ipv4.tcp_keepalive_time
    2. 减少 Keepalive 探测失败后，一直到通知应用程序前的重试次数 net.ipv4.tcp_keepalive_probes

- UDP 优化，相对于 TCP 来说，要简单得多。这里我也总结了常见的几种优化方案。
    1. 增大套接字缓冲区大小以及 UDP 缓冲区范围；
    2. 增大本地端口号的范围；
    3. 根据 MTU 大小，调整 UDP 数据包的大小，减少或者避免分片的发生。

- 参考指标
![5.png](./5.png)

### 网络层优化 ###
- 从路由和转发的角度出发
    1. NAT 网关的服务器或者使用 Docker 容器时，开启 IP 转发，即设置 net.ipv4.ip_forward = 1。
    2. 调整数据包的生存周期 TTL，比如设置 net.ipv4.ip_default_ttl = 64。注意，增大该值会降低系统性能。
    3. 开启数据包的反向地址校验，比如设置 net.ipv4.conf.eth0.rp_filter = 1。这样可以防止 IP 欺骗，并减少伪造 IP 带来的 DDoS 问题。
        - 0：关闭反向路由校验
        - 1：开启严格的反向路由校验。对每个进来的数据包，校验其反向路由是否是最佳路由。如果不是最佳路由，则丢弃。
        - 2：开启松散的反向路由校验。对每个进来的数据包，校验其源地址是否可达，如果反向路径不通，则丢弃。

- 从分片的角度出发
    1. 最主要的是调整 MTU（Maximum Transmission Unit）的大小。
    2. MTU 的大小应该根据以太网的标准来设置。
    3. 以太网标准规定，一个网络帧最大为 1518B，那么去掉以太网头部的 18B 后，剩余的 1500 就是以太网 MTU 的大小。
    4. 使用 VXLAN、GRE 等叠加网络技术时，要注意，网络叠加会使原来的网络包变大，导致 MTU 也需要调整。
        - VXLAN在原来报文的基础上，增加了 14B 的以太网头部、 8B 的 VXLAN 头部、8B 的 UDP 头部以及 20B 的 IP 头部。换句话说，每个包比原来增大了 50B。
        - 需要把交换机、路由器等的 MTU，增大到 1550， 或者把 VXLAN 封包前（比如虚拟化环境中的虚拟网卡）的 MTU 减小为 1450。
        - 如果网络设备支持巨帧，可以把 MTU 调大为 9000，以提高网络吞吐量。

- 从 ICMP 的角度出发
    1. 为了避免 ICMP 主机探测、ICMP Flood 等各种网络问题，你可以通过内核选项，来限制 ICMP 的行为。
    2. 禁止 ICMP 协议，即设置 net.ipv4.icmp_echo_ignore_all = 1。
    3. 禁止广播 ICMP，即设置 net.ipv4.icmp_echo_ignore_broadcasts = 1。

### 链路层优化 ###
> 链路层负责网络包在物理网络中的传输，比如 MAC 寻址、错误侦测以及通过网卡传输网络帧等。自然，链路层的优化，也是围绕这些基本功能进行的。

- 网卡收包后调用的中断处理程序需要消耗大量的 CPU。 将中断处理程序调度到不同的 CPU 上执行，就可以显著提高网络吞吐量。
    1. 为网卡硬中断配置 CPU 亲和性（smp_affinity）
    2. 开启 irqbalance 服务
    3. 开启 RPS（Receive Packet Steering）和 RFS（Receive Flow Steering），将应用程序和软中断的处理，调度到相同 CPU 上，增加 CPU 缓存命中率，减少网络延迟。

- 把原来在内核中通过软件处理的功能，可以卸载到网卡中，通过硬件来执行。
    1. TSO（TCP Segmentation Offload）和 UFO（UDP Fragmentation Offload）
        - 在 TCP/UDP 协议中直接发送大包；而 TCP 包的分段（按照 MSS 分段）和 UDP 的分片（按照 MTU 分片）功能，由网卡来完成 。

    2. GSO（Generic Segmentation Offload）
        - 在网卡不支持 TSO/UFO 时，将 TCP/UDP 包的分段，延迟到进入网卡前再执行。
        - 可以减少 CPU 的消耗，还可以在发生丢包时只重传分段后的包。

    3. LRO（Large Receive Offload）
        - 在接收 TCP 分段包时，由网卡将其组装合并后，再交给上层网络处理。
        - IP 转发的情况下，不能开启 LRO，因为如果多个包的头部信息不一致，LRO 合并会导致网络包的校验错误。

    4. GRO（Generic Receive Offload）：
        - GRO 修复了 LRO 的缺陷，并且更为通用，同时支持 TCP 和 UDP。

    5. RSS（Receive Side Scaling）：
        - 称为多队列接收，它基于硬件的多个接收队列，来分配网络接收进程，这样可以让多个 CPU 来处理接收到的网络包。

    6. VXLAN 卸载
        - 让网卡来完成 VXLAN 的组包功能。

- 优化网络的吞吐量。
    1. 开启网络接口的多队列功能。每个队列可以用不同的中断号，调度到不同 CPU 上执行，从而提升网络的吞吐量。
    2. 增大网络接口的缓冲区大小，以及队列长度等，提升网络传输的吞吐量（注意，这可能导致延迟增大）。
    3. 使用 Traffic Control 工具，为不同网络流量配置 QoS。

- C10M极限场景
    1. DPDK 技术，跳过内核协议栈，直接由用户态进程用轮询的方式，来处理网络请求。结合大页、CPU 绑定、内存对齐、流水线并发等多种机制，优化网络包的处理效率。
    2. XDP 技术(内核自带)，在网络包进入内核协议栈前，就对其进行处理，这样也可以实现很好的性能。

### 其他 ###
- 软中断处理，就有专门的内核线程ksoftirqd。每个 CPU 都会绑定一个 ksoftirqd 内核线程。
- 并非所有网络功能，都在软中断内核线程中处理。内核中还有很多其他机制（比如硬中断、kworker、slab 等），这些机制一起协同工作，才能保证整个网络协议栈的正常运行。
- TCP/UDP端口号只占 16 位，也就说其最大值也只有 65535。如果使用 TCP 协议，在单台机器、单个 IP 地址时，并发连接数最大也只有 65535 呢？
    1. Linux 协议栈，通过五元组来标志一个连接（即协议，源 IP、源端口、目的 IP、目的端口)。
    2. 对客户端来说，每次发起 TCP 连接请求时，都需要分配一个空闲的本地端口，去连接远端的服务器。由于这个本地端口是独占的，所以客户端最多只能发起 65535 个连接。
    3. 对服务器端来说，其通常监听在固定端口上（比如 80 端口），等待客户端的连接。
        - 根据五元组结构，我们知道，客户端的 IP 和端口都是可变的。
        - 如果不考虑 IP 地址分类以及资源限制，服务器端的理论最大连接数，可以达到 2 的 48 次方（IP 为 32 位，端口号为 16 位），远大于 65535。






