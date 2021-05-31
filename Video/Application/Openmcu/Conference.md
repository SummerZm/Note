## **Opemcu Conference framework**
### **Conference类**
- **ConferenceAudipConnection**: 音频处理入口
    1. ConferenceAudioConnection - ConferenceConnection
    ```sh
    # a. AudioBuffer 
    # b. AudioResampler
    # c, AudioResolution
    ```
- **MCUVideoMixerList**： 视频处理入口

- **ConferenceMember**
    1. Dial()：拨号呼叫逻辑
    2. 视频流读写：通过conference的MCUVideoMixerList操作音频流
    3. 内置conference指针，通过conferencez指针操作音频流

- **ConferenceMonitor**
    1. 数据库同步线程
    2. 语音流视频流监听
    3. 调用分屏器列表 MCUVideoMixerList 的监视函数
    4. 内置ConferenceManager指针

### **ConferenceManager**
- 从当前会议列表中增删改查一个会议
- 内置会议 ConferenceMonitor 指针。【全局只有一个遍历conferencelist, 之后内嵌遍历了每个connference的videomixermonitor.两重循环】