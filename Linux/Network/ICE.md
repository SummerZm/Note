## **网络穿透：ICE协议**

### **ICE 功能简介**
- 接受客户端的请求，并且把客户端的公网IP、Port封装到ICE Candidate中。通过一个复杂的机制，得到客户端的NAT类型
- 完成了这些STUN Server就会这些基本信息发送回客户端，然后根据NAT类型，来判断是否需要TURN服务器协调进行下一步工作。

### **STUN和TURN的实现**
- **STUN是如何判断NAT的类型？**
    1. **判断客户端是否在NAT后**
        ```sh
        # C: Client     S: Server
        # C向S的IP1的pot1端口发送一个UDP包。S收到这个包后，会把它收到包的源IP和port写到UDP包中，然后把此包通过IP1和port1发还给C
        # 结果1：【没收到回复包】网络问题，被防火墙拦下，STUN服务不在，弄错端口
        # 结果2：【收到回复包】  把此UDP中的IP和自己的IP做比较
        #        A. 【一样】  客户端自身位于公网  
        #        B. 【不一样】客户端自身位于NAT后   --- 【打洞成功,称为STEP1洞】
        ```

    2. **判断是否处于Full Cone Nat下 【来者不拒】**
        ```sh
        # C向S发送一个UDP包，请求S通过另外一个IP2,向C返回一个UDP数据包 --- 【换一个IP给STEP1洞回数据】
        # 结果1： 【收到】   来者不拒，是Full Cone Nat 
        # 结果2： 【没收到】 不是Full Cone Nat 
        ```

    3. **判断是否处于对称NAT下**
        ```sh
        # C向S的IP2的port2发送一个数据包，S收到数据包后，把它收到包的源IP和port写到UDP包中，然后通过自己的IP2和port2把此包发还给C。
        # 结果：
        #   C肯定能收到这个回应UDP包。     --- 【打洞成功,称为STEP3洞】
        #   如果这个port和step1中的port一样，那么可以肯定这个NAT是个CONE NAT，否则是对称NAT。
        # 
        # 依据： 
        #   根据对称NAT的规则，当目的地址的IP和port有任何一个改变，那么NAT都会重新分配一个port使用，
        #   在step3中，和step1对应，我们改变了IP和port。 --  【对比STEP1洞和STEP3洞 - 即NAT上的端口不同】
        #   因此，如果是对称NAT,那这两个port肯定是不同的。
        ```

    4. **判断是处于Restrict Cone NAT还是Port Restrict NAT之下**
        ```sh
        # C向S的IP2的一个端口PD发送一个数据请求包，要求S用IP2和不同于PD的port返回一个数据包给C。 --- 【换一个端口给STEP3洞回数据】
        # 结果1: 【收到】   只要IP相同，即使port不同，NAT也允许UDP包通过。是Restrict Cone NAT。
        # 结果2：【没收到】  Port Restrict NAT.
        ```
    
- **STUN如何协调NAT打洞穿透 【背景：A处于NAT后】【存疑中】**
    1. **Full Cone Nat**
        ![./image/FullConeNat.webp](./image/FullConeNat.webp)
    2. **Restrict Cone Nat 和 Port Restrict Nat 【注：第5步方向反了】**
        ![./image/RestrictConeNat.webp](./image/RestrictConeNat.webp)
    3. **对称Nat**
        ![./image/PeerNat.webp](./image/PeerNat.webp)
    4. NAT打的洞也是具有时效性的，如果NAT超时了，那么还是需要重新打洞的。


### **NAT打洞相关知识**
- **client/server网络编程**
    ```sh
    # A. 在cs模型的网络编程中，服务器都架设在公网，服务器端不用打洞
    # B. client向服务器发送请求时，client的打洞和服务器响应的穿越是自动实现的。
    # C. cs模型中，打洞是单方向的，即只需客户端打洞。
    ```

- **P2P网络编程**
    ```sh 
    # A. P2P网络编程中，通信的双方A和B既要当服务器，又要当客户端。
    # B. 打洞是双方的：A在自己的NAT上为B打一个洞，让B的数据能过来；B在自己的NAT上为A打一个洞，让A的数据能过来。
    # C. AB双方的打洞的操作得由我们自己完成，所以不得不提及穿越打洞的概念。
    ```
- **NAT打洞：命令A和B互相发一条信息，这样各自的NAT就留下了对方的洞**

### **NAT穿越相关方案**
- **ALG(应用层网关)**
    1. ALG 可以认为是 NAT 的插件，让NAT识别出特有协议，进而不限制它们
    2. ALG 的功能
        ```sh
        # A. 修改自己“认识的”协议相关字段的IP地址。内网改成相应的外网地址。
        # B. 允许自己“认识的”协议直接穿透，而不需要打洞。
        ```
    3. ALG 的应用与不足
        ```sh
        # A. 常用的应用有FTP、DNS、ICMP、SIP等。
        # B. 在VOIP中，如果使用ALG的话，可以实现通信双方的直接媒体通信，但是不足之处就是需要NAT支持SIP，这就不适合像skype类型的应用。
        ```
        
- **UPnP**
    1. 【在NAT上做手脚】：当内网的主机启动UPnP程序的时候，就会在NAT上产生 “映射端口”与UPnP程序的端口一一映射，而且不会被回收。
    2. 【缺点】：需要应用程序、操作系统以及NAT支持UPn
    3. 【案例】：从外部来的消息，只要是发送到“映射端口”的，直接就会送至内网的UPnP程序。像我们平常用到的迅雷、电骡等。

- **STUN/TURN/ICE**
    1. 使用STUN/TURN/ICE最大的好处充分利用NAT的特性，不需要操作系统支持、不需要对现有的NAT设备做改变就能实现穿越。  

### **STUN TURN ICE之间的关系**
- **STUN协议**
    1. 目的： 进行P2P通信，通过提供反射地址（Server Reflexive Address）这种能力来使双方可以进行P2P通信
    2. 缺点： 依赖NAT类型的不同，这种方式是有失败的概率的：比如双方都为对称型NAT或者一方为对称型，另一方为端口限制型。在多层NAT下，类型的探测不总是有效的
    3. 作用： 为ICE提供支持(对Binding的扩展)。

- **TURN协议**
    1. 目的:  保证通信双方百分之百能进行通信，就是在只知道反射地址而打洞失败的情况下的一种补充方案。
    2. 性质： 使用中继方式百分之百能使得双方进行通信，只不过已经不是P2P的了，而且伴随而来的是转发效率的问题。
    3. 优缺点：该协议的目的就是保证双方肯定能通信，损失效率来保证了连通性。

- **ICE协议**
    1. 目的：综合以上两种方案，通过通信双方互相发探测包，找出一种最合理，最廉价的可行路径。
    2. 方式：ICE首先探测内网地址，再探测STUN提供的反射地址，最后探测TURN协议的中继地址，反正最终目的就是探出一条路，内网地址不行用反射地址，反射地址不行，最后不得已情况下那就用中继地址。
    3. 疑问：探测内网地址的目的和方式是？

- **现况**
    1. 一般来说，目前的TURN服务器通常也实现了STUN协议，所以可以称之为TURN服务器或者是STUN 服务器。
    2. 如果说一个服务器是STUN服务器，那么该服务器可能是纯的STUN(RFC 5389)服务器，也可能是一个TURN(RFC 5766)服务器，也可能是两者都实现了的服务器。
    
### **资料索引**
- [1. ICE协议下NAT穿越的实现](https://www.jianshu.com/p/84e8c78ca61d?utm_campaign=hugo&utm_medium=reader_share&utm_content=note)  
- [2. STUN/TURN/ICE协议在P2P SIP中的应用（一）](https://www.cnblogs.com/ishang/p/3810382.html)

