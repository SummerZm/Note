## **复杂网络下的视频流媒传输**

### **问题背景**
- **多媒体会话信令协议**
1. **协议**： sip 、 rtsp 、 h.323 
2. **目的**： 通过建立 p2p(peer to peer) 媒体流以减小时延
3. **方式**： 建立一个在信息中携带 ip 地址的分组流
4. **问题**： 由于协议自身设计上的原因，使得媒体流无法直接穿透网络地址转换 / 防火墙 (nat/fw) 。
5. **方案**： 
    ```sh
    # 复杂的网络类型
    # 完全圆锥型 nat(full cone nat) 
    # 地址限制圆锥型 nat (address restricted cone nat) 
    # 端口限制圆锥型 nat (port restricted cone nat) 
    # 对称型 nat (symmetric nat) 
    # 前三种 nat ，映射与目的地址无关，只要源地址相同，映射就相同，而对称型 nat 的映射则同时关联源地址和目的地址，所以穿透问题最为复杂。


    # 穿透解决方案
    # algs(application layer gateways) 
    # middlebox control protocol 
    # stun (simple traversal of udp through nat) 
    # turn(traversal using relay nat) 
    # rsip(realm specific ip)
    # symmetric rtp 
    ```

- **ice技术**  
1. **ice(interactive connectivity establishment)简介**
    ```sh
    # 交互式连通建立方式 ice(interactive connectivity establishment) 并非一种新的协议，它不需要对 stun 、 turn 或 rsip 进行扩展就可适用于各种 nat 。 
    # ice 是通过综合运用上面某几种协议，使之在最适合的情况下工作，以弥补单独使用其中任何一种所带来的固有缺陷。
    # 对于 sip 来说， ice 只需要定义一些 sdp(session description protocol) 附加属性即可，对于别的多媒体信令协议也需要制定一些相应的机制来实现。
    ```

2. **ICE技术算法流程**
    ```sh
    # 1. 收集传输地址
    #       本地传输地址 (local transport address) : 通常由主机上一个物理 ( 或虚拟 ) 接口绑定一个端口而获得
    #       来源传输地址 (derived transport address) : 从 unsaf(unilateral self-address fixing)服务器，例如 stun 、 turn 或 teredo, 上获得一组来源传输地址。
    #
    # 2. 启动stun 【重点不好理解】
    #
    # 3. 确定传输地址的优先级
    #       优先级反映了 ua 在该地址上接收媒体流的优先级别，取值范围在 0 到 1 之间，通常优先级按照被传输媒体流量来确定。
    #       流量小者优先，而且对于相同流量者的 ipv6 地址比 ipv4 地址具有更高优先级。
    #       因此物理接口产生的本地 ipv6 传输地址具有最高的优先级，然后是本地 ipv4 传输地址，然后是 stun 、 rsip 、 teredo 来源地址，最后是通过 vpn 接口获得的本地传输地址。
    #
    # 4. 构建初始化信息
    #       初始化消息由一系列媒体流组成，每个媒体流都有一个缺省地址和候选地址列表。
    #       缺省地址通常被 initiate 消息映射到 sip 信令消息传递地址上，而候选地址列表用于提供一些额外的地址。
    #       对于每个媒体流来说，任意 peer 之间实现最大连通可能性的传输地址是由公网上转发服务器 ( 如 turn) 提供的地址，通常这也是优先级最低的传输地址。
    #       客户端将可用的传输地址编成一个候选地址列表 ( 包括一个缺省地址 ) ，并且为每个候选元素分配一个会话中唯一的标识符。
    #       该标识符以及上述的优先级都被编码在候选元素的 id 属性中。一旦初始化信息生成后即可被发送。
    #
    # 5. 响应处理：连通性检查和地址收集
    # 6. 接受信息处理
    # 7. 附加ice过程
    # 8. ice到sip的映射
    ```

3. **ICE方式的优势**
    ```sh
    # 传统的 stun 脆弱点
    # A. 需要客户端自己去判断所在 nat 类型，这实际上不是一个可取的做法。
    # B. stun 、 turn 等机制都完全依赖于一个附加的服务器，而 ice 利用服务器分配单边地址的同时，还允许客户端直接相连。
    # C. 传统的 stun 最大的缺陷在于它不能保证在所有网络拓扑结构中都正常工作，最典型的问题就是 symmetric nat 。
    # D. 对于 turn 或类似转发方式工作的协议来说，由于服务器的负担过重，很容易出现丢包或者延迟情况。
    ```
- **相关链接**  
[**WebRTC ICE**](https://blog.csdn.net/fireroll/article/details/50780863)









