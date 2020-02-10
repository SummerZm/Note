## <b>DDOS 网络攻击</b> ##
> 已知的最大流量攻击是Github遭受的DDoS攻击，其峰值流量已经达到了1.35Tbps，PPS 更是超过了1.2亿（126.9 million）

### <b>DDOS网络攻击原理</b> ###
1. 耗尽带宽。网络设备的带宽都有固定的上限。带宽耗尽后，就会发生网络拥堵，从而无法传输其他正常的网络报文。
2. 耗尽操作系统的资源。CPU、内存等物理资源，以及连接表等软件资源。一旦资源耗尽，系统就不能处理其他正常的网络连接。
3. 消耗应用程序的运行资源。应用程序通常需要跟其他的资源或系统交互。如果程序一直忙于处理无效请求，会导致请求的处理变慢，得不到响应。

### <b>DDOS网络攻击类型</b> ###
1. 流量型DDos: 在服务器外部的网络设备中，设法识别并阻断流量（当然前提是网络设备要能扛住流量攻击）。比如，购置专业的入侵检测和防御设备，配置流量清洗设备阻断恶意流量等。
2. 慢型DDoS：请求流量可能本身并不大，但响应流量却可能很大，并且应用程序内部也很可能要耗费大量资源处理。
3. SYN Flood：小包攻击，巨大的PPS会导致 Linux 内核消耗大量资源，进而导致其他网络报文的处理缓慢。

### <b>SYN 攻击优化</b> ###
1.  iptables -I INPUT -s 192.168.0.2 -p tcp -j REJECT
    ```sh
    # 限制syn并发数为每秒1次
    iptables -A INPUT -p tcp --syn -m limit --limit 1/s -j ACCEPT
    # 限制单个IP在60秒新建立的连接数为10
    iptables -I INPUT -p tcp --dport 80 --syn -m recent --name SYN_FLOOD --update --seconds 60 --hitcount 10 -j REJECT

    cat /etc/sysctl.conf
    # SYN Cookies 基于连接信息（包括源地址、源端口、目的地址、目的端口等）以及一个加密种子（如系统启动时间），计算出一个哈希值（SHA1）称为 cookie。
    # 这个 cookie 就被用作序列号，来应答 SYN+ACK 包，并释放连接状态。
    # 当客户端发送完三次握手的最后一次 ACK 后，服务器就会再次计算这个哈希值，确认是上次返回的 SYN+ACK 的返回包，才会进入 TCP 的连接状态。
    net.ipv4.tcp_syncookies = 1 
    # 减少syn包重发次数
    net.ipv4.tcp_synack_retries = 1
    # 增大半连接的容量
    net.ipv4.tcp_max_syn_backlog = 1024
    ```
2. 在 Linux 服务器中，你可以通过内核调优、DPDK、XDP 等多种方法，来增大服务器的抗攻击能力，降低 DDoS 对正常服务的影响。
3. 在应用程序中，你可以利用各级缓存、 WAF、CDN 等方式，缓解 DDoS 对应用程序的影响。
