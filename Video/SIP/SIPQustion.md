## **SIP通话中常见的问题**
> **每个SIP消息的生成都和终端的配置，服务器的支持能力，网络环境有着非常紧密的联系**

### **1. 不能注册或呼叫到SIP服务器端**
- **不能注册，完全没有和SIP服务器连接**
    1. linux排查工具 sngrep； 实时检查SIP消息.
    ```sh
    # centos 安装sngrep 
    vim /etc/yum.repos.d/irontec.repo
    [irontec]
    name=Irontec RPMs repository
    baseurl=http://packages.irontec.com/centos/$releasever/$basearch/
    rpm --import http://packages.irontec.com/public.key
    yum install sngrep

    # debian 安装sngrep 
    echo "deb http://packages.irontec.com/debian jessie main" >> /etc/apt/sources.list
    wget http://packages.irontec.com/public.key -q -O - | apt-key add -
    apt-get install sngrep -y
    debian buster 即 debian10以上可以直接 apt-get install sngrep

    # [使用教程](https://www.yuque.com/wangdd/opensips/sngrep)
    ```

    2. SIP终端发出了注册消息，但是SIP服务器端没有返回的消息.
    3. 客户端收到错误消息，收到太多401/407 Unauthorized.
    4. 客户端收到403 Forbidden 消息.
    ```sh
    # 注册过于频繁，服务器端可能过滤了此地址。需要用户配合服务器端地址进行进一步检查。这里
    # 也可能是系统防火墙或者其他的配置禁止了注册消息。
    # 呼叫时出现403的话，则可能是另外的原因，例如可能欠费，
    # 可能呼叫了服务器端禁止的号码码位等其他因素。
    ```

### **2. 30秒挂断呼叫的黄金法则**
- **最主要的原因是SIP终端没有收到ACK消息**
    ```sh
    # SIP终端发送了 200 OK以后就开始了媒体的创建，RTP语音流开始启动。
    # 事实上，SIP终端可能还没有收到ACK消息，因此在30秒左右，没有收到消息的一方就发送了一个BYE消息。
    ```
- **返回时因为NAT问题导致ACK没有办法返回到相应的终端**
> **NAT问题ACK返回的路径地址发生了改变，所以SIP终端没有收到ACK消息。**
    ```sh
    # Contact header 错误   
    # 客户端没有支持router header
    # 网关在NAT后
    # Contact header 的地址在NAT后

    # 用户排查NAT等网络配置问题 
    ```

### **3. 咬线或摘机状态**
- **现象：SIP终端不能挂机或者处于摘机状态**
- **原因：在SIP终端中的表现形式为终端没有发送BYE消息或者发送了错误的BYE消息内容**
- **具体原因**
    ```sh
    # 没有发送BYE消息
    # 发送到BYE消息携带了错误的to-tag
    # 发送了格式不规范的BYE消息，例如格式错误，sequences错误或者时间戳错误。
    # 发送的BYE消息中携带的是错误的路由信息
    ```
- **问题解决**
    ```sh
    # 从终端角度看待处理问题
    # 终端的配置问题
    # 终端的质量问题
    # 如果是FXO或者FXS的话，是否出现了制式不匹配的问题导致咬线或者摘机的问题。

    # 从服务器端处理的话，可以通过两种办法来通过服务器端对其进行强制挂机处理。这四种服务器端的检测方式是：
    # A. 开启RTP超时检测来检测终端的RTP流是否仍然活动
    # B. 开启SIP的KeepAlive 检测SIP 会话状态
    # C. 使用Proxy中的dialog中的OPTION来检测SIP终端响应状态，SIP终端发送 200 OK到proxy来检测终端的状态。
    # D. 使用SIP session timer 定时器来进行周期检测，SIP终端一直在周期内刷新自己的状态。
    #    如果SIP终端来定时器的时间范围内，则表示终端参与活动状态；否则，则对其发送BYE消息，强行挂机。
    ```

### **4. 单通或无语音**
- **INVITE和200 OK中的SDP的地址不通**
    1. 如果INVITE中的地址不能和200 OK中的SDP地址不能连通的话，可能导致单通的问题。
    2. 有时也存在NAT问题，需要用户针对性地进行排查。
- **网络防火墙过滤了UDP端口**
    1. 网络的防火墙过滤了RTP端口。
    2. 用户必须开启路由器的端口转发策略，媒体服务器的端口范围不同，可能rtp的端口设置不同。
- **ALG 网关设置问题**
    1. ALG网关有时可以解决NAT问题，但是也同样会带来其他的问题。ALG可以设置其他的SIP 服务器端口。有时用户可以关闭路由器上的ALG选项解决单通问题。
    2. ALG应用级网关，也叫做应用层网关（Application Layer Gateway），由一个扩增防火墙或计算机网络应用或 NAT 平安部件组成的一类防火墙。

### **5. 收到400 bad request**
- **消息体中携带了非法的参数或者SIP服务器或者代理不能正确解析消息体格式，有可能在消息体中携带了重复的参数设置或者其他非法字符。**
    1. 两个重复的mkp参数设置
    2. 可能丢失了To以后的最后Contact 头域值。
    3. Content-Length长度的问题

- **NAT地址的无效的host问题等问题**

### **6. 收到413，513 Request Entity Too Large或Message Too Large消息**
- **问题原因**
    1. UDP包的限制是1500 bytes
    2. Proxy路由路径中添加了太多的VIA header
    3. 路由路径中添加Record-Route headers
    4. 终端配置了太多的编码选项支持

- **问题解决**
    1. 尽量减少太多的proxy路由，大家知道，每经过一个proxy路由就会增加相应的头值，最后可能出现数据包太大的问题，UDP拒绝通过。
    2. 尽量减少终端的编码选项支持，我们建议用户使用1-3种常用的编码即可。
    3. 排查一些SIP 服务器可能增加额外的自有的头包。一些商业的解决方案可能有自有的非标准的头包，如果没有必要的话，可以关闭这些选项设置。
    4. 使用拓扑隐藏方式或者B2BUA减少路由的其他开销。这种方式仅产生新的请求，不会增加Via header头域值和record header。
    5. 尽量不要使用BLF, 因为BLF会不断发送新的消息，数据包会增加。

### **7. 收到408， 480或者487 消息**
> **遇到408， 480 或者487的消息。通常情况下，这三个错误消息和SIP的定时器相关**
- **场景举例**
    ```sh
    # A. 在一个呼叫创建以后，SIP终端开始振铃
    # B. 如果SIP服务器端的超时设置首先被触发，服务器端就会返回一个408 timeout 消息。
    # C. 如果是SIP终端的超时设置被首先触发的话，客户端会发送一个480 消息。
    # D. 每次触发超时以后，对端都会发送一个Cancel,
    # E. 这里的Cancel 和INVITE是两个分别不同的事务， 他们执行各自的流程。如果SIP 用户端在一定的超时设置时间内没有人接听呼叫，就会返回一个487 消息。
    # F. 随后 OK响应的是Cancel。487响应的是INVITE。
    ```

- **可SIP终端本地设置错误，也可能导致408 超时错误。如果SIP终端收到了408 超时消息。**

### **8. 483 - Too Many Hops**
- 483 too many hops 如果用户配置了服务器，错误配置domain或不清楚domain，形成一个自己的回环网络
- 一些SIP 解决方案的厂家也支持了Loop Detection 功能，它可以支持detect 模式，超时设置和阀值，如果用户遇到类似的设备的话，可以开启这些参数做进一步的优化措施

### **9. 488 – Not Acceptable Here**
- **SIP 终端出现这个问题的话，都是因为编码不支持导致的**
    1. 如果不同的SIP终端使用了不同的语音编码的话，需要SIP媒体服务器进行编码协商，如果双方的编码统一了，才能进行正常的呼叫。
    2. 很多情况下，用户设置的编码有很大差异，如果媒体服务器编码协商失败的话，就会返回488的错误。

- **服务器端编码支持能力，用户可以和维护人员联系，检查是否支持SIP终端设置的编码**
    1. 如果不支持的话，需要关闭编码选项。很多情况下，特别是用户使用Asterisk或FreeSWITCH，为了节省网络带宽的开销，很多SIP trunk 或者IMS使用了G.729。
    2. 但是，很多Asterisk和FreeSWITCH 如果没有默认配置G.729的模块的话， 或者没有编码转换的能力，那么服务器端结合出现编码协商失败的问题，最后返回488 响应消息。

### **10. 语音质量**
- **语音回声的三个黄金法则**
1. 回声总是在听到回声的对端产生。一般来说，回声是从终端或者接入设备传入，因此必须从接入或者终端方解决回声问题。
2. 回声的问题大部分是有PSTN的接入设备导致，所以尽量使用带回声的处理接入设备，杜绝回声的产生。
3. 如果遇到回声，不使用带扬声器的终端测试，使用耳麦软电话测试。


[相关链接](https://blog.csdn.net/alwaysrun/article/details/107825090)








