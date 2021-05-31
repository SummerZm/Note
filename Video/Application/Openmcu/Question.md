## **日常处理问题合集** 
### **双屏设备回声问题**
- 问题描述：同一台设备有两个屏幕各一个SIP账号，共用一个麦克风。主席模式下，禁用了右屏的音频采集仍然能听见回声。
    1. 在服务器上禁用右屏的喇叭,没听到回声,但过一会用户成员退出。
    2. 由1可得,双屏设备的声音是来自于右屏;用户会退出原因是服务器没有往设备端发送音频数据，链接断开。
    3. 服务器端禁用音频，但实际上只是不处理音频，数据还是会上传到服务器的。

- 解决方案：设备端粉不处理/播放服务端发来的右屏数据;
- 疑问: 右屏的声音是从哪里来的呢？明明已经禁用了它的音频

### **常见逻辑问题**
> **设计数据库时类似可以多维度挖掘查询信息的数据，不要过度分表和创建主键约束, 导致业务需求被制约** 
- 数据库主键先从账号转变成序列号
1. 原因: 账号的IP地址一变就找不到原来的记录了。
2. 缺点：引入的新缺点是旧设备名称信息会被覆盖，结果旧参与会议人员记录出现错误。

### **性能问题**
- **分屏只是视频的界面布局，当多终端每个终端不同的分辨率时，需要编码对应的分辨率视频，会耗费很大性能 [解决：统一终端编码]**






