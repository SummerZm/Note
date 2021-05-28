## **Opemcu videoMixer framework**

### VideoMixConfigurator 类
- VMPcfgLayout类：规定视频会议画面如何合成
	1.  VMPcfgOption类
		```sh
		VMPBlock类
		```
	2.  VMPcfgSplitOption类

### MCUVideoMixer类
- VideoFrameStoreList类 : From RTP

- VideoMixPostion类 : 位置成员列表VMPList 
	1. MCUBufferYUV类 : 存放从RTP读取到的视频数据
	2. 将读出来的视频进行bit操作合成新界面

- ReadFrame(....) : From FrameStort 
- MCUsubtitles类  : 水印字幕；依赖于第三方库freetype
- WriteFrame(...) : To Client
- ConferenceMember: 会议成员列表
