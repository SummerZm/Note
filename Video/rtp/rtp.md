## **RTP 协议**

###  **RTP简介**
> **1996年的RFC1889给出RTP的规范和细节，其后在RFC3550中进行了更新【要系统性学习，直接看RFC3550规范】**

- **RTP:  实时传输协议(Real-time Transport Procotol): 一种网络传输协议，负责音视频数据的封包和传输**
- **RTCP: 实时传输控制协议(Read-time Transport Control Protocol): 流量和拥塞控制，媒体间的同步**
- **RTSP: 实时传输流协议(Real Time Streaming Procotol)**
![RTP网络模型图](./rtpNetworkModel.jpg)

### **RTP连接的建立**
- 通过RTSP、SIP或者HTTP等协议和接收端协商，确定了RTP要传输的对端IP地址和端口，传输内容格式【用SDP描述】
- 一般在协商过程中确定发送端RTP和RTCP的目的地址，偶数端口就是RTP媒体流的目的端口，偶数端口+1就是RTCP协议的目的端口

### **RTP报文结构**
![RTP报文结构](./rtpFormat.jpg)
```sh
# V:  RTP协议版本号.
# P:  填充标记(如果有填充数据，填充数据的第一个字节记录了填充数据的长度).
# X:  是否有拓展头.
# CC: CSRC个数【最多15个】
# M:  对于视频而言，表示帧结束
# PT: 有效载荷有效载荷类型(GSM音频、JPEM图像等)
# sequence number: 
#      网络不好时可用于检查丢包。网络抖动时可用于对数据重新排序;
#      音频包和视频包的sequence是分别记数的。
#
# timestamp:
#   单位是采样率到倒数，不是真实的时间，一般要根据采样率进行换算。
#   RTP报文第一个八位组的采样时刻，目的是为了接收端计算延迟、抖动和音视频同步。
#   一个视频帧的时间戳是相同的，如果视频帧数据量很大可能需要多个RTP包传输，它们的时间戳相同
#   音频帧数据小，不存在音频帧跨RTP的情况，所以不存在这个问题
# 
# SSRC: 同步信源标识符，用于标识同步信源。参加同一视频会议的两个同步信源不能有相同的SSRC。
# CSRC: 特约信源标识符
#     每个CSRC标识了包含在该RTP报文有效载荷中的所有特约信源
#     作用：混流和混音时，需要把各方的RTP同步信号源列出来
#
#
```


### **RTP头部拓展**
![RTP头部拓展图](./rtpHeader.jpg)
```sh
# 大部分音视频场景都没有启用RTP扩展部分，但是也有例外。在WebRTC中看到利用RTP扩展部分做了FEC
# 
# 扩展字段定义define by profile:16bit两字节，这个由上层的具体实现协议来决定
# 扩展头长度length:表示扩展头的长度字段，16bit即2字节，最大扩展长度1024字节
# 
```
[参考链接](https://www.huaweicloud.com/articles/79999bf7e4235d59927366e0dd1ca267.html)