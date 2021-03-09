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
- 

### ***4. 类结构图**
- openmcu
	1. conferenceManager 
	2. sipendpoint
	3. registar
	4. rtspServer
	5. cfg

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

- registar 
	1. AccountThread
	2. ConnectionThread
	3. SubscriptionThread
	4. QueueThread
	5. BookThread
	6. McuVerifyLB
	7. cpuThread
	8. HeartBeatsThread










