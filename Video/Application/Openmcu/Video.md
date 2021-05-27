## **Opemcu videoMixer framework**

### VideoMixConfigurator 类
- VMPcfgLayout类
	1.  VMPcfgOption类
		```sh
		VMPBlock类
		```
	2.  VMPcfgSplitOption类

### MCUVideoMixer类
- VideoFrameStoreList类 : From RTP

- VideoMixPostion类 : 位置成员列表VMPList 
	1.	MCUBufferYUV类 : 存放从RTP读取到的视频数据

- ReadFrame(....) : From FrameStort 
- MCUsubtitles类  : 水印字幕；依赖于第三方库freetype
- WriteFrame(...) : To Client
- ConferenceMember: 会议成员列表
