## **PTlib库类型信息简介**
> **工程的目的：如何用更少更薄的中间层解决问题？** **[什么是PTlib?](!https://blog.csdn.net/f_705/article/details/49783835?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522160670653819721942283418%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=160670653819721942283418&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_v2~rank_v28-11-49783835.pc_first_rank_v2_rank_v28&utm_term=ptlib)**

```sh
# PTlib由基类和控制台两部分组成.
# 基类部分: 主要包括网络,多线程,ASN等...,所有使用PTLib的应用程序必然用到基类部分.
# 控制台部分: 定义了一些基本的数据和设备类型,它是可选的,不过要方便移植的话还是要用到它.
```

### **A. 基本对象类**
> **学习基础类抽象方法(概念定义)，代码编写方法.【数据存储】**
- PObject - PWLib中的根类,所有的类都派生于此 
- PContainer - 抽象容器类
- PAbstractArray - 所有数组对象的基类
- PAbstractList - 所有列表对象的基类 
- PAbstractDictionary - 字典对象的基类
- PString - 字符类,在Openh323中随处可见

### **B. I/O通道类**
> **拓展各种IO的简单特性认知. [阻塞/异步]【数据读取】**
- PChannel - I/O通道的基类,下面类都派生于此
- PIndirectChannel - 可以交换的操作不同的I/O通道
- PConsoleChannel - 进入系统命令行的通道
- PPipeChannel - 在当前进程和子进程剪建立管道 
- PSerialChannel - 串口通信通道 
- PFile - 文件操作类.
- PTextFile - 文本文件操作类
- PStructuredFile - 结构文件操作类,不同操作文件类型都不尽相同
- PFilePath -对文件的路径描述的类
- PVideoChannel - 视频的输入,输出通道,采用YUV格式
- PSoundChannel - 音频的输入,输出通道,使用线性的PCM通道

### **C. 套接字类** 
> **网络报文的结构，协议设计的初衷.【数据传输】**
- PSocket - 套接字的基类,所有网络操作类派生于此 
- PIPSocket -  IP协议 层的描述类 
- PUDPSocket -  IP协议 中UDP描述类 
- PTCPSocket -  IP协议 中 TCP/IP 描述类 
- PICMPSocket - IP协议中ICMP描述类 
- PIPXSocket - IPX协议的基类
- PEthSocket - 底层的以为网接口类,可以直接操作底层数据包

### **D. 进程和线程类 【数据处理】**
> **进程/线程要考虑的问题是固定的，框架/系统本身的复杂性，代码理解上升了一个维度。要抓住关键节点.**
- PProcess - 实现程序的主线程控制类 
- PServiceProcess - 后台程序或系统进程使用的类 
- PThread - 普通线程的控制类 
- PSemaphore - 使用信号量的 线程同步 类
- PMutex - 使用互斥对象的 线程同步 类
- PCriticalSection - 使用临界区的 线程同步 类
- PSyncPoint - 使用事件的线程同步类
- PAtomicInteger - 描述了信号量及 PV操作

### **E. 其他杂类**
> **辅助管理类构建代码的时候很重要，阅读时可以不过纠结，它们一般目的明确.**
- PArgList - 命令行参数类 
- PConfig - 存储应用程序的配置内容 
- PTime - 时间和日期的抽象类 
- PTimeInterval - 以 毫秒 为单位的记时类
- PDynaLink - 动态连接类
- PRemoteConnection - 网络连接控制类
- PMail - 调用系统默认程序发邮件
- PPluginManager - 插件管理模块
- PSmartPointer - 智能指令类 
- PNotifier - 通告类,可以调用任何类的任何函数
- PSmartNotifierFunction - 智能的通告函数类,使用对象ID

### **F. HTTP协议类**
> **控制台部分, 学习文本协议有哪些解析方法**
- PHTTP - HTTP协议基类
- PURL - URL的具体描述
- PHTML - HTML的具体描述
- PHTTPServiceProcess - 后台的HTTP服务程序类

### **G. 其他协议类** 
> **学习字节流协议有哪些解析处理方法**
- PInternetProtocol - Interner文本协议基类 
- PPOP3  -  POP3 协议的基类
- PSMTMP -  SMTP协议 的基类
- PFTP -  FTP协议 的基类
- PMIMEInfo - 描述MIME信息
- PTelnetSocket - 描述TELNET协议 
- PSocksProtocol - Socks协议的基类
- PSTUNClient - 实现STUN登陆操作
- PSNMP - SNMP协议的基类
- PSSLChannel - 在OpenSSL基础上建立的 SSL协议 类
- PSASL - 在Cyrus SASL基础上建立的SAS协议L类
- PXMLRPC - 在XML和HTTP基础上建立的XMLRPC协议描述 
- PSOAPClient - 实现SOAP登陆
- PLDAPSession - 使用OpenLDAP进行LDAP登陆 
- PILSSession - 使用OpenLDAP进行ILS登陆
- XMPP::Stream - 实现XMPP(Jabber)流

### **H. 其他杂类**
> **学习如何写一个能在复杂(多进程/多线程/多过程/多状态)应用场景中使用的类？**
- PModem -操作Modem,使用AT命令和Modem串口通信
- PIpAccessControlList - 子网掩码的描述
- PRandom - 产生随机数类
- PCypher - 不同编码的解码和编码
- PWAVFile - AIFF格式的WAV文件操作类 
- PDTMFDecoder - 对PCM流进行DTMF解码 
- PMemoryFile - 让 数据存储 在内存中
- PSDLVideoDevice - 使用SDL库实现视频设备
- PXML - XML的解析器
- PVXMLChannel - VXML的解析器
- PTextToSpeech - 实现文本到语音的语音合成转换
