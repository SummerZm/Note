## **flv文件格式介绍**

### **一. flv文件格式**
> **flv**由flv header 和 flv body组成.**flv body**由多个flv tag 组成. **flv tag**前面的跟着一个**preTagSize**.

#### **1. flv header 格式图** 

![flvHeader格式图](./flvHeaderFormat.jpg)

#### **2. flv body 格式图**  
    
![flvBody格式图](./flvBodyFormat.jpg)

#### **3. flv tag 格式图**

> Tag有三种类型，Audio Tag（音频Tag），Video Tag（视频Tag），script Tag（又称Metadata Tag）

![flvTag格式图](./flvTagFormat.jpg)

#### **4. flv 总体结构图**
![flv文件格式](./flvFileFormat.jpg)
![flv格式图](./flvFormat.jpg)

### **二. flvTag 三种数据类型格式**
#### **1. flvTag audio 数据格式**
![flvTag音频格式图](./flvAudioTag.jpg)

> **SoundFormat 等于 10 时， 音频数据格式为ACC**   

![flvTag Acc音频tagData格式图](./flvAccAudioTagData.jpg)

#### **2. flvTag video 数据格式** 

![flvTag视频格式图](./flvVideoTag.jpg)

> **frameType 等于 7 时,视频数据格式为H264**   

![flvTag H264视频tagData格式图](./flvH264VideoTagData.jpg)

> **视频数据H264 AVCPacketType 等于 0 【H264配置信息】**     

![flvTag H264视频tagData格式图](./flvH264VideoTagDataType0.jpg)

> **视频数据H264 AVCPacketType 等于 1**  

![flvTag H264视频tagData格式图](./flvH264VideoTagDataType1.jpg)

#### **3. flvTag script 数据格式** 
> **通常被称为MetadataTag，放一些关于FLV视频和音频的元数据信息如：duration、width、height等, 会跟在FileHeader后作为第一个Tag出现，而且只有一个。**

![flvTag Script格式图](./flvScriptTag.jpg)

```sh
# 第一个AMF包：第一个字节一般为0x02，表示字符串，第2-3个字节表示字符串的长度，一般为0x000A，后面跟的就是字符串，一般为"onMetaData"。
# 第二AMF包：第一个字节为0x08，表示数组，第2-5个字节表示数组元素个数，后面跟着就是数组的元素。
#   格式为：元素名长度（UI16） + 元素名（UI8[n]） + 元素的值(double)，最后以“009”结尾。
```

> **flvTag script类型数据第一个字节表示 AMF 的类型**

![flvTag Script AMF类型列表](./flvScriptTagType.jpg)


> **flvTag script类型数据第一个字节表示 AMF 的类型 【即上面的AMF2】**  

![flvTag Script AMF类型0x08时可选值列表](./flvScriptTagAMFType0x08OptionArray.jpg)
