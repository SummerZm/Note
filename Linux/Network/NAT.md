## <b>NAT 网络技术</b> ##

> NAT 技术可以重写 IP 数据包的源 IP 或者目的 IP，被普遍地用来解决公网 IP 地址短缺的问题

### <b>NAT的类型</b> ###
> 在Linux服务器中配置 NAT。 实际上就是把Linux服务器充当的是“软”路由器的角色

- 根据实现方式的不同，NAT 可以分为三类：
    1. 静态 NAT，即内网 IP 与公网 IP 是一对一的永久映射关系。
    2. 动态 NAT，即内网 IP 从公网 IP 池中，动态选择一个进行映射。
    3. 网络地址端口转换 NAPT（Network Address and Port Translation），即把内网 IP 映射到公网 IP 的不同端口上，让多个内网 IP 可以共享同一个公网 IP 地址。

- 根据转换方式的不同，可以把NAPT分为三类：
    1. 源地址转换 SNAT，即目的地址不变，只替换源 IP 或源端口。主要用于，多个内网 IP 共享同一个公网 IP ，来访问外网资源的场景。
    2. 目的地址转换 DNAT，即源 IP 保持不变，只替换目的 IP 或者目的端口。主要通过公网 IP 的不同端口号，来访问内网的多种服务，隐藏后端服务器的真实IP地址。
    3. 双向地址转换，同时使用 SNAT 和 DNAT。
        - 接收到网络包时，执行 DNAT，把目的 IP 转换为内网 IP。
        - 发送网络包时，执行 SNAT，把源 IP 替换为外部 IP。
        - 外网 IP 与内网 IP 的一对一映射关系，常用在虚拟化环境中，为虚拟机分配浮动的公网 IP 地址。

### <b>NET filter网络框架</b> ###
>  NAT 网关直接影响整个数据中心的网络出入性能，所以 NAT 网关通常需要达到或接近线性转发，PPS 是最主要的性能目标。
- NAT网关实际上是通过Net filter 框架的4张表，5条链的操作实现的
    ![1.png](./1.png)

### <b>NAT 性能问题</b> ###
1. NAT 基于 Linux 内核的连接跟踪机制来实现。
2. 分析 NAT 性能问题时，先从 conntrack 角度来分析，比如用 systemtap、perf 等，分析内核中 conntrack 的行为
    ```sh
    #perf record 和 perf report 命令
    # 记录一会（比如30s）后按Ctrl+C结束
    perf record -a -g -- sleep 30
    # 输出报告
    perf report -g graph,0
    #在 perf report 界面中，输入查找命令 / 然后，在弹出的对话框中，输入 nf_hook_slow；最后再展开调用栈,看哪个函数占用系统资源高
    ```
3. 最后，通过调整 netfilter 内核选项的参数，来进行优化。
    ```sh
        $ sysctl -a | grep conntrack
        # 表示当前连接跟踪数
        net.netfilter.nf_conntrack_count = 180
        # 表示最大连接跟踪数
        net.netfilter.nf_conntrack_max = 1000
        # 表示连接跟踪表的大小
        net.netfilter.nf_conntrack_buckets = 65536
        net.netfilter.nf_conntrack_tcp_timeout_syn_recv = 60
        net.netfilter.nf_conntrack_tcp_timeout_syn_sent = 120
        net.netfilter.nf_conntrack_tcp_timeout_time_wait = 120
    ```


