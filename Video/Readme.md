## <b>音视频处理流程简介</b> ##
>  原文链接：https://blog.csdn.net/leixiaohua1020/article/details/18893769

- <b>解协议：</b> 从流媒体协议http,rtmp等丢弃信令数据，保留流媒体协议音视频数据。
    - 常见流媒体协议: http，rtmp, MMS
    - 输出：封装的音视频数据 

- <b>解封装：</b> 将已经压缩编码的音频数据和视频数据从封装格式中<b>分别</b>取出来。
    - 常见封装格式: MKV, RMVB, TS, FLV, AVI
    - 输出：编码过的视频码流和音频码流

- <b>解压缩：</b> 将视频和音频数据<b>分别</b>解压缩 
    - 将压缩过的音视频数据解压成原始数据 <b>最复杂最重要的过程</b>
    - 常见视频压缩标准: H.264, MPEG2, VC-1
    - 常见音频压缩标准: AAC, MP3, AC-3
    - 常见原始数据格式: YUV, RGB, PCM(音频)

- <b>音视频同步</b>
    - 根据前面解封装模块处理过程中获取的参数信息，同步解码出来的视频和音频数据
    - 将解码出来的音视频数据送至系统的显卡和声卡

## <b>流媒体协议概览</b> ##
> RTMFP相对较新的流媒体传输协议, 支持p2p

|流媒体协议|公司|传输层协议|应用|
|:---|:---|:---|:---|
|RTSP/RTP|IETF|TCP+UDP|IPTV领域|
|RTMP/RTMFP|Adobe Inc.|TCP/UDP|互联网直播|
|MMS|Microsfot Inc.|TCP/UDP|互联网直播+点播|
|HTTP|WWW+IETF|TCP|互联网点播|

## <b>封装格式概览</b> ##
>    AVI不支持流媒体播放

 |封装格式|机构|支持的音/视频编码|使用领域|
 |:---|:---|:---|:---|
 |AVI|<b>Microsoft Inc.</b>|几乎所有格式|BT下载影视|
 |MKV|<b>CoreCodec Inc.</b>|同上|互联网视频网站|
 |MP4|<b>MPEG</b>|MPEG-2, MPEG-4, H.264, H.263等<br/>AAC, MPEG-1 Layers I, II, III, AC-3等|互联网视频网站|
 |TS|<b>MPEG</b>|同上|IPTV，数字电视|
 |FLV|<b>Adobe Inc.</b>|Sorenson, VP6, H.264<br/>MP3, ADPCM, Linear PCM, AAC等|互联网视频网站|
 |RMVB|<b>Real Networks Inc.</b>|RealVideo 8, 9, 10<br/>AAC, Cook Codec, RealAudio Lossless|BT下载影视|

## <b>视频编码概览</b> ##
> 视频编码的主要作用是将视频像素数据（RGB，YUV等）压缩成为视频码流，从而降低视频的数据量。

|名称|推出机构|推出时间|目前使用领域|
|:---|:---|:---:|:---|
|HEVC(H.265)|<b>MPEG/ITU-T</b>|2013|研发中|
|VP9|<b>Google</b>|2013|研发中|
|VP8|<b>Google</b>|2008|不普及|
|<b>H.264</b>|<b>MPEG/ITU-T</b>|2003|<b>各个领域</b>|
|MPEG4|<b>MPEG</b>|2001|不温不火|
|MPEG2|<b>MPEG</b>|1994|数字电视|
|VC-1|<b>Microsoft Inc.</b>|2006|微软平台|


### <b>视频编码技术在整个视音频技术中应该是最复杂的技术:相关书籍 ###
-  《现代电视原理》
-  《数字电视广播原理与应用》（本科的课本）中的部分章节
-  &nbsp;&nbsp;视频不经过压缩编码的话，一部电影可能就要上百G的空间。

### <b>264编码器介绍</b> ###
   - H.264最标准的源代码，官方标准JM。但JM速度非常的慢，是无法用于实际
   - 实际中使用最多的就是x264了，性能强悍（超过了很多商业编码器），而且开源
   - Google推出的VP8属于和H.264同一时代的标准。总体而言，VP8比H.264要好
   - 我国还推出了自己的国产标准AVS，性能也不错，但目前比H.264还是要稍微逊色一点

### <b>下一代编码标准(HEVC-VP9)</b> ###
 
- HEVC在未来拥有很多大的优势，HEVC将会取代H.264
- 未来实际使用的HEVC开源编码器很有可能是x265，目前该项目还处于发展阶段
- 限制码率的视频编码标准比较（包括MPEG-2，H.263， MPEG-4，以及 H.264）
           <b> HEVC > VP9 > H.264> VP8 > MPEG4 > H.263 > MPEG2</b>

## <b>音频编码概览</b > ##
>  <b>音频编码的主要作用是将音频采样数据（PCM等）压缩成为音频码流，从而降低音频的数据量。<b>

### <b>背景与现状</b> ###
- 一般情况下音频的数据量要远小于视频的数据量，因而即使使用稍微落后的音频编码标准，而导致音频数据量有所增加，也不会对视音频的总数据量产生太大的影响。
- 高效率的音频编码在同等的码率下，可以获得更高的音质。

### <b>主要音频编码一览</b> ###
> 近年来并未推出全新的音频编码方案，音频编码技术已经基本满足人们的需要。<br>音频编码技术近期绝大部分的改动都是在AAC的基础上完成的。

|名称|推出机构|推出时间|目前使用领域|
|:---|:---|:---|:---|
|WMA|Microsoft Inc.|1999|微软平台|
|AAC|MPEG|1997|各个领域（新）|
|MP3|MPEG|1993|各个领域（旧）|
|AC-3|Dolby Inc.|1992|电影|

音频编码方案之间音质比较: AAC+ > MP3PRO > AAC> RealAudio > WMA > MP3<br/>
此外杜比数字的编码标准也比较流行，但是貌似比最新的AAC稍为逊色：AC-3技术综述

##  <b>应用方案</b> ##
### <b>直播</b>###
>  直播服务普遍采用了RTMP作为流媒体协议，FLV是RTMP使用的封装格式，使用Flash播放器。

  - H.264是当今实际应用中编码效率最高的视频编码标准
  - AAC则是当今实际应用中编码效率最高的音频编码标准
  - 封装格式，视频编码，音频编码方面，使用FLV + H.264 + AAC的组合。
  - 采用RTMP作为直播协议的好处在于其被Flash播放器支持。

### <b>点播</b> ###
> 点播服务普遍采用了HTTP作为流媒体协议，H.264作为视频编码格式，AAC作为音频编码格式。

- HTTP是基于TCP协议的应用层协议，媒体传输过程中不会出现丢包等现象，从而保证了视频的质量。
- HTTP被绝大部分的Web服务器支持，因而流媒体服务机构不必投资购买额外的流媒体服务器，从而节约了开支。
- 点播服务采用的封装格式有多种：MP4，FLV，F4V等，它们之间的区别不是很大。
- 视频播放器方面，无一例外的都使用了Flash播放器。

### <b>无插件直播</b> ###
- Flash播放器如今已经安装在全球99%的电脑上，并且与浏览器结合的很好。
- Flash和Rtmp 都是adobe Inc. 的。