## **UDP端口扫描简介**

### **NMAP**
- 最常见的UDP服务：DNS，SNMP，和DHCP (注册的端口是53，161/162，和67/68)
- 探测扫描办法：
    ```sh
    # UDP扫描发送空的(没有数据)UDP报头到每个目标端口
    #  1. 如果返回ICMP端口不可到达错误(类型3，代码3)， 该端口是closed(关闭的)
    #  2. 其它ICMP不可到达错误(类型3， 代码1，2，9，10，或者13)表明该端口是filtered(被过滤的)
    #  3. 用版本扫描(-sV)帮助区分真正的开放端口和被过滤的端口
    #  4. 加速UDP扫描的方法包括并发扫描更多的主机，先只对主要端口进行快速 扫描，从防火墙后面扫描，使用--host-timeout跳过慢速的 主机。
    ```
- UDP扫描面临的问题缺点：
    ```sh
    #  缺点1：开放的和被过滤的端口很少响应，让Nmap超时然后再探测，以防探测帧或者 响应丢失。
    #  缺点2：关闭的端口，许多主机在默认情况下限制ICMP端口不可到达消息。【Linux 2.4.20内核限制一秒钟只发送一条目标不可到达消息】
    #  缺点3：UDP扫描一般较慢，比TCP更困难
    ```