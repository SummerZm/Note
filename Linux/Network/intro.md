## <b>网络基础知识</b> ##

### <b>性能指标</b> ###
1. 带宽: 表示链路的最大传输速率，单位通常为 b/s （比特 / 秒），由网卡决定。
2. 吞吐量: 表示单位时间内成功传输的数据量，单位通常为 b/s（比特 / 秒）或者 B/s（字节 / 秒）。吞吐量受带宽限制，而吞吐量 / 带宽，也就是该网络的使用率。
3. 延时: 表示从网络请求发出后，一直到收到远端响应，所需要的时间延迟。在不同场景中，这一指标可能会有不同含义, TCP三次握手延时或ping数据包往返所需的时间（比如 RTT）。
4. PPS: Packet Per Second（包 / 秒）的缩写，表示以网络包为单位的传输速率。PPS 通常用来评估网络的转发能力。
    - 基于 Linux 服务器的转发，很容易受到网络包大小的影响。
    - 交换机通常不会受到太大影响，即交换机可以线性转发。
5. 其他：网络的可用性（网络能否正常通信）、并发连接数（TCP 连接数量）、丢包率（丢包百分比）、重传率（重新传输的网络包比例）等也是常用的性能指标。

### <b>工具参数</b> ###
1. 查看网口状态
    ```
    ens33   Link encap:Ethernet  HWaddr 00:0c:29:27:be:9f  
            inet addr:192.168.2.214  Bcast:192.168.2.255  Mask:255.255.255.0
            inet6 addr: fe80::3e5e:f3f:4b3e:4bb9/64 Scope:Link
            UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
            RX packets:253 errors:0 dropped:0 overruns:0 frame:0
            TX packets:212 errors:0 dropped:0 overruns:0 carrier:0
            collisions:0 txqueuelen:1000 
            RX bytes:195191 (195.1 KB)  TX bytes:21049 (21.0 KB)
    
    1. 网络接口的状态标志。ifconfig输出中的RUNNING ，或 ip 输出中的 LOWER_UP ，都表示物理网络是连通的.
    2. MTU 的大小。MTU 默认大小是 1500，根据网络架构的不同（比如是否使用了 VXLAN 等叠加网络），你可能需要调大或者调小 MTU 的数值。
    3. 网络接口的 IP 地址、子网以及 MAC 地址。这些都是保障网络功能正常工作所必需的，你需要确保配置正确。
    4. 网络收发的字节数、包数、错误数以及丢包情况.
        errors 表示发生错误的数据包数，比如校验错误、帧同步错误等；
        dropped 表示丢弃的数据包数，即数据包已经收到了 Ring Buffer，但因为内存不足等原因丢包；
        overruns 表示超限数据包数，即网络 I/O 速度过快，导致 Ring Buffer 中的数据包来不及处理（队列满）而导致的丢包；
        carrier 表示发生 carrirer 错误的数据包数，比如双工模式不匹配、物理电缆出现问题等；
        collisions 表示碰撞数据包数。
    ```
2. 查看网络连接状态
    ```
    # head -n 3 表示只显示前面3行
    # -l 表示只显示监听套接字
    # -n 表示显示数字地址和端口(而不是名字)
    # -p 表示显示进程信息
    $ netstat -nlp | head -n 3
    Active Internet connections (only servers)
    Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
    tcp        0      0 127.0.0.53:53           0.0.0.0:*               LISTEN      840/systemd-resolve

    # -l 表示只显示监听套接字
    # -t 表示只显示 TCP 套接字
    # -n 表示显示数字地址和端口(而不是名字)
    # -p 表示显示进程信息
    $ ss -ltnp | head -n 3
    State    Recv-Q    Send-Q        Local Address:Port        Peer Address:Port
    LISTEN   0         128           127.0.0.53%lo:53               0.0.0.0:*        users:(("systemd-resolve",pid=840,fd=13))
    LISTEN   0         128                 0.0.0.0:22               0.0.0.0:*        users:(("sshd",pid=1459,fd=3))

    1. 接收队列（Recv-Q）和发送队列（Send-Q）通常应该是 0。当你发现它们不是0时，说明有网络包的堆积发生。

    2. 当套接字处于连接状态（Established）时:
        Recv-Q 表示套接字缓冲还没有被应用程序取走的字节数（即接收队列长度）。
        Send-Q 表示还没有被远端主机确认的字节数（即发送队列长度）。
    3. 套接字处于监听状态（Listening）时:
        Recv-Q 表示全连接队列的长度。
        Send-Q 表示全连接队列的最大长度。

    3. 全连接: 完成了TCP三次握手，然后就会把这个连接挪到全连接队列中。这些全连接中的套接字，还需要被 accept() 系统调用取走，服务器才可以开始真正处理客户端的请求。
    4. 半连接: 还没有完成 TCP 三次握手的连接，连接只进行了一半。服务器收到了客户端的 SYN 包后，就会把这个连接放到半连接队列中，然后再向客户端发送 SYN+ACK 包。

    netstat -s
    Tcp:
        3244906 active connection openings
        23143 passive connection openings
        115732 failed connection attempts
        2964 connection resets received
        1 connections established
        13025010 segments received
        17606946 segments sent out
        44438 segments retransmitted
        42 bad segments received
        5315 resets sent
        InCsumErrors: 42
    ...
    $ ss -s
    Total: 186 (kernel 1446)
    TCP:   4 (estab 1, closed 0, orphaned 0, synrecv 0, timewait 0/0), ports 0
    Transport Total     IP        IPv6
    *    1446      -         -
    RAW    2         1         1
    UDP    2         2         0
    TCP    4         3         1
    ...
    ```
3. 如何查看系统当前的网络吞吐量和PPS
    ```sh
    给 sar 增加 -n 参数就可以查看网络的统计信息，比如网络接口（DEV）、网络接口错误（EDEV）、TCP、UDP、ICMP 等等。执行下面的命令，你就可以得到网络接口统计信息：
    # 数字1表示每隔1秒输出一组数据
    $ sar -n DEV 1
    Linux 4.15.0-1035-azure (ubuntu)   01/06/19   _x86_64_  (2 CPU)
    13:21:40        IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s   %ifutil
    13:21:41         eth0     18.00     20.00      5.79      4.25      0.00      0.00      0.00      0.00
    13:21:41      docker0      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
    13:21:41           lo      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00

    rxpck/s 和 txpck/s:  分别是接收和发送的 PPS，单位为包 / 秒。
    rxkB/s 和 txkB/s:  分别是接收和发送的吞吐量，单位是 KB/ 秒。
    rxcmp/s 和 txcmp/s: 分别是接收和发送的压缩数据包数，单位是包 / 秒。
    %ifutil 是网络接口的使用率，即半双工模式下为 (rxkB/s+txkB/s)/Bandwidth，而全双工模式下为 max(rxkB/s, txkB/s)/Bandwidth。
    Speed: 1000Mb/s
    ```


