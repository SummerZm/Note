## **PTlib框架信息**
> **如何组织粘合一个框架？需求是什么？**

### **1. 目录结构信息**
### **2. 代码流程脉络**
- **Openmcu: 编码支持以插件的形式编入项目中. [MCUpluginCodecManager()]**
- **Openmcu: 开启用户认证时，从配置文件中的读取需要验证的用户密码信息. [InitAccount()-reg.h]**
- **Openmcu: 有一个用户队列Account和已经注册的用户队列bookAccount**
- **Openmcu: 注册创建了一个Regconnection, 然后保存利用已有Connection创建SipConnection.**
- **Openmcu: 有各种monitor和manager**

### **3. 软件线程是如何跑起来的**
- ****

### ***4. 类结构图**
- openmcu
> **SIP 注册机制如何与会议连接起来？**
	1. conferenceManager 
	2. sipendpoint
	3. registar
	4. rtspServer
	5. cfg
    6. EventQueue
    7. LiveEventQueue
    8. telnetServer
    9. OTFcontrol
    10. httpd
    11. plugmgr
    12. log

================================================

- conferenceManager
    1. addVideoMixer
    2. conferenceList
    3. conferenceMintor

- conference
    1. memberList
    2. audioConnectionList
    3. videoMixerList

- ConferenceMember
    1. readAuido
    2. writeAudio
    3. writeVideo
    4. videoMixer
    5. conference
	6. rtsp成员也被模拟成member加入会议
	7. 设置成员的视角

- conferenceAudioConnection
    1. readAuido
    2. writeAudio
    3. Mix
    4. audioBufferList

-  videoMixerList [MCUSimpleVideoMixer]
    1. vmpList
    2. writeFrame
    3. readFrame
    4. AddvideoSourceLayout
	5. 设置分屏器的视频输入

================================================

- sipendpoint


- registar 
	1. AccountThread    -- accountList
	2. ConnectionThread -- connectionList
	3. SubscriptionThread -- subscriptionList
	4. QueueThread  -- regQueue
	5. BookThread   -- abookList
	6. McuVerifyLB
	7. cpuThread
	8. HeartBeatsThread
    9. aliveThread
    10. serverList
    11. isnatip [NAT]
    12. conn [MySql]

### **相关问题**
1. SIP注册逻辑如何与视频会议结合
2. Rstp,Srs如何参与视频会议






