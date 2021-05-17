## SIP 协议简介
> [SIP协议的技术解析](!https://forum.huawei.com/enterprise/zh/thread-400579-1-1.html)

### **一、概述**
- SIP是一个应用层控制协议，用于创建，更改，和终止会话.
- SIP的能力：用户定位，通信能力协商，用户意愿交互，建立呼叫，呼叫处理和控制.
- SIP创建会话的方式：单播联网，组播方式，MCU(Multipoint Control Unit).
- SIP消息格式：
    1. SIP:user:password@host:port;transport-param|user-param|method-param|ttl-param|maddr-param|other-param 
    2. transport-param: TCP/UDP
    3. maddr-param: 服务器地址

- 相关概念
    1. 呼叫：一个会议被同一个发起者邀请加入的所有成员组成
    2. 呼叫分支：由Call-ID, To, From 三个参数共同决定
    3. 事务：从客户端发起第一个请求消息到服务端回应这个请求的最后一条响应期间的所有消息。一般由一个呼叫分支中的Cseq顺序号来标识。有例外。
        ```sh
        # 一个呼叫一般包含三个事务。呼叫启动包含两个操作：邀请(INVITE)和证实(ACK)；呼叫终结包含一个操作：再见(BYE)
        ```

### **二、 消息类型**
- **客户端发给服务端的请求**
    |请求消息|消息含义|
    |--:|:--|
    |INVITE|发起会话请求，邀请用户加入一个会话|
    |ACK|证实收到INVITE请求的最终响应|
    |BYE|释放已建立的呼叫|
    |CANCEL|释放尚未建立成功的呼叫|
    |REGISTER|向SIP网络服务器登记用户位置信息|
    |OPTIONS|查询服务器的能力信息|
    |||

- **服务端发给客户端的请求**
    1. 响应消息是服务器向客户端反馈对应请求的处理结果的消息
    2. 响应消息分临时响应和最终响应。1XX响应是临时响应，其他响应都是最终响应

        |响应状态码|消息功能|
        |--:|:--|
        |100|试呼叫|
        |180|振铃|
        |181|呼叫正在前转|
        |182|排队|
        |183|可靠临时响应|
        |200|OK|
        |||

### **三、消息结构**
- **请求消息和响应消息**：起始行，消息头字段，标志头字段结束的空行和消息体字段。
    1. **起始行:** 请求消息是请求行；响应消息是状态行。
    2. **消息头字段:** 通用头，请求头，响应头，实体头。(一个SIP消息可以有多个头域)
        ```sh
        # 通用头字段
            Accept | Accept-Encoding | Accept-Language | Call-ID | Contact |
            Cseq |Date | Encryption | From | Record-Route | Require |
            Supported | Timestamp | To | User-Agent | Via
        
        # 请求头字段
            Authorization | Contact | Hide | Max-Forwards| Organization |
            Priority | Proxy-Authorization | Proxy-Require | Route |
            Require | Response-Key | Subject

        # 响应头字段
            Proxy-Authenticate | Retry-After | Server | Unsupported | Warning | WWW-Authenticate

        # 实体头字段
            Allow | Content-Encoding | Content-Length | Content-Type | Expires
        ```
    3. **消息体:** 可以采用SDP(Session Description Protocol) 来描述本次会话的具体实现方式

- **请求消息详解**
    |头字段|格式|备注|
    |--:|:--|:--|
    |**Call-ID**|Call-ID：local-id@host|1. host:为主机域名或IP地址; Call-ID字符需区分大小写|
    |**From**|From: display-name <SIP-URL>;tag=xxxx|1. display-name:用户界面上显示的字符</br>2. tag:区分当两个共享同一SIP地址的用户实例用相同的Call-ID发起的呼叫邀请|
    |**To**|To: display-name ;tag=xxxx|1. 所有请求和响应消息必须包含此字段|
    |**Cseq**|命令名称和一个十进制序号组成，在Call-ID范围内唯一确定|1. 重发请求的序号保持不变</br> 2. 服务器端将请求消息中的Cseq值复制到响应消息中，用于将请求消息和其触发的响应消息相对应</br> 3. ACK和CANCEL消息的Cseq序号和对应的INVITE请求消息的相同</br>4. 服务器会记忆相同Call-ID的INVITE请求的最高序号，收到序号低于此值的INVITE请求将在给出响应后予以丢弃|
    |**Via**|Via：sent-protocol sent-by;via-params comment</br>sent-protocol=protocol-name / protocol-version / transport </br> via-params=via-hidden / via-ttl / via-maddr / via-received / via-branch</br>received: 内网地址经NAT映射后真正转发SIP消息的地址</br>示例：Via: SIP/2.0/UDP 191.169.1.116:5061;ttl=16;maddr=191.169.10.20;branch=a7c6a8dlze|1. 防止请求消息传送产生环路</br> 2. 保响应消息和请求消息选择同样的路径,以保证通过防火墙或满足其它特定的选路要求</br> 3. 若代理服务器向多播地址发送请求，则必须在其Via字段中加入“maddr”参数，此参数指明多播地址</br>4. 发起请求消息的客户端必须将其自身的主机名或网络地址插入请求消息的Via字段 </br>5. 第1个Via字段应该指示正在处理本消息的代理服务器或客户端。如果不是，丢弃该消息，否则，删除该Via字段</br>|
    |**Contact**|Contact：name-addr；q=value；action= “proxy” / “redirect”；expires=value；extension-attribute</br>|1. 给出其后和用户直接通信的地址，而不必借助Via字段经由一系列代理服务器返回</br>2. 地址参数name-addr的表示形式和To，From字段的相同</br>3. q参数的取值范围为[0，1]，指示给定位置的相对优先级</br>4. action参数仅用于REGISTER请求，它表明希望服务器对其后到该客户的请求进行代理服务还是重定向服务|
    |**Max-Forwards**|Max-Forwards:十进制整数|1. 该字段用于限定一个请求消息到达其目的地址前所允许被转发的次数|
    |**Allow**|Allow: INVITE, ACK, OPTIONS, CANCEL, BYE|1. 代理服务器支持的所有请求消息类型列表|
    ||||
    ```sh
    # 请求消息示例
    INVITE sip:66500002@191.169.1.110 SIP/2.0
    From: <sip:44510000@191.169.1.116>;tag=1ccb6df3
    To: <sip:66500002@191.169.1.110>
    CSeq: 1 INVITE
    Call-ID: 20973e49f7c52937fc6be224f9e52543@sx3000
    Via: SIP/2.0/UDP 191.169.1.116:5061;branch=z9hG4bkbc427dad6
    Contact: <sip:44510000@191.169.1.116:5061>
    Max-Forwards:70
    Content-Length:230
    Content-Type: application/sdp
    v: 0
    o: HuaweiSoftX3000 1073741831 1073741831 IN IP4 191.169.1.116
    s: Sip Call
    c: IN IP4 191.169.1.95
    t: 0 0
    m: audio 30000 RTP/AVP 8 0 4 18
    a: rtpmap:8 PCMA/8000
    a: rtpmap 0 PCMU/8000
    a: rtpmap 4 G723/8000
    a: rtpmap 18 G729/8000

    # 解释如下
    第一行：请求行。Method标记为INVITE请求消息；对端用户的URI为“sip:66500002@191.169.1.110”；SIP版本号为2.0。
    第二行：From字段。指明请求发起方的地址为“<sip:44510000@191.169.1.116>”。“tag”为“1ccb6df3”，用于共享同一SIP地址的不同用户用相同的Call-ID发起呼叫邀请时，对用户进行区分。
    第三行：To字段。指明请求接收方的地址为“<sip:66500002@191.169.1.110>”。
    第四行：Cseq字段。
    第五行：Call-ID字段。该字段唯一标识一个特定的邀请，全局唯一。
    第六行：Via字段。该字段用于指示该请求历经的路径。“SIP/2.0/UDP”表示发送的协议，协议名为“SIP”，协议版本为“2.0”，传输层为“UDP”；“191.169.1.116:5061”表示发送方CSOFTX3000 IP地址为191.169.1.116，端口号为5061；“branch=z9hG4bkbc427dad6”为分支参数，CSOFTX3000并行分发请求时标记各个分支。
    第七行：Contact字段。指示其后的请求（如BYE请求）可以直接发往<sip:44510000@191.169.1.116:5061>，而不必借助Via字段。
    第八行：Max-Forwards字段。表示该请求到达其目的地址所允许经过的中转站的最大值为70。
    第九行：Content-Length字段。指示消息体长度。
    第十行：Content-Type字段，表示消息中携带的消息体是单消息体且为SDP。
    第十一行：空行，表示消息头结束，下面为SDP描述的消息体。

    # 消息体部分的简要解释如下，详细的解释可以参阅SDP相关的资料。
    v=<协议版本>
    o=<用户名><会话标识><版本><网络类型><地址类型><地址>
    s=<会话名>
    c=<网络类型><地址类型><连接地址>
    t=<起始时间><终止时间>
    m=<媒体><端口><传送层><格式列表>
    a =rtpmap:<净荷类型><编码名><编码>
    ```

### **四、SIP-T协议简介**
- **SIP-T**（Session Initiation Protocol for Telephones）是SIP协议的扩展
- SIP消息中携带**ISUP信令**的机制，实现PSTN和SIP网络互通，包括三种呼叫模型：PSTN-IP、IP-PSTN、PSTN-IP-PSTN。
- SIP-T协议采用SIP的消息结构和消息流程。SIP-T主要采用了**封装**和**映射**两大技术。
    1. **封装**: 在SIP消息体中携带ISUP消息。包括两种情况：
        ```sh
        # A. SIP消息本身不携SDP，ISUP消息封装在SIP消息体中，消息体类型为Application/SDP
        # B. SIP消息本身携带SDP，则SIP消息需要包含多个消息体，封装了ISUP的消息体类型为Application/ISUP
        ```
    2. **映射**: ISUP-SIP消息映射，以及ISUP消息参数与SIP消息头域之间的映射。
        ```sh
        # ISUP-SIP消息映射
        # 在网关处必须能够根据收到的每一条SIP消息产生一条对应的ISUP消息，反之每一条ISUP消息也都能对应生成一条SIP消息
        IAM = INVITE
        ACM = 180 RINGING
        ANM = 200 OK
        REL = BYE
        RLC = 200 OK for BYE
        ```

































