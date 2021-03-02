## **[PS流简介](https://blog.csdn.net/weixin_43608153/article/details/90707272)**
> **udp发送，发送数据要封装为rtp格式，rtp负载即为PS流**

### **PS流的组成**
> **PS流由众多PS包构成。PS包基本构成形式：固定包头 + 系统头 + 映射头 + PES头 + 负载**

- **A. PS Header**
1. 包起始码字段 pack_start_code：值为’0000 0000 0000 0000 0000 0001 1011 1010’ (0x000001BA)的位串，用来标志一个包的开始。
2. 系统时钟参考字段 system_clock_reference_base 和 system_clock_reference_extenstion
3. 标记位字段 marker_bit：1位字段，固定值 ‘1’。
4. 节目复合速率字段 program_mux_rate：一个22位整数，规定P-STD在包含该字段的包期间接收节目流的速率。其值以50字节/秒为单位。不允许取0值。
5. 包填充长度字段 pack_stuffing_length：3位整数，规定该字段后填充字节的个数。
6. 填充字节字段 stuffing_byte
7. 8位字段，取值恒为’1111 1111’

- **B. SYS Header**

|字段|含义|备注|
|:--|:--|:--| 
|system_header_start_code| 系统标题起始码字段|32 位字段，取值’0000 0000 0000 0000 0000 0001 1011 1011’ (0x000001BB)的位串，指出系统标题的开始|
|header_length| 标题长度字段|16位字段。指出该字段后的系统标题的字节长度。在本规范将来的扩充中可能扩展该字段|
|marker_bit| 占位1bit|固定值为 1|
|rate_bound|速率界限字段|22位字段，取值不小于编码在节目流的任何包中的program_mux_rate字段的最大值。该字段可被解码器用于估计是否有能力对整个流解码|
|marker_bit|占位1bit|固定值为 1|
|audio_bound|音频界限字段|6位字段，取值是在从0到32的闭区间中的整数，且不小于节目流中解码过程同时活动的GB/T XXXX.3和GB/T AAAA.3音频流的最大数目|
|fixed_flag|固定标志字段|1位标志位。置’1’时表示比特率恒定的操作；置’0’时，表示操作的比特率可变|
|CSPS_flag |CSPS标志字段|1位字段。置’1’时，节目流符合标准|
|system_audio_lock_flag| 系统音频锁定标志字段|1位字段。表示在系统目标解码器的音频采样率和system_clock_frequency之间存在规定的比率|
|system_video_lock_flag| 系统视频锁定标志字段|1位字段。表示在系统目标解码器的视频帧速率和system_clock_frequency之间存在规定的比率|
|marker_bit|占位1bit|固定值为 1|
|vedio_bound| 视频界限字段|5位字段，取值是在从0到16的闭区间中的整数且不小于节目流中解码过程同时活动的GB/T XXXX.2和GB/T AAAA.2流的最大数目|
|packet_rate_restriction_flag|分组速率限制标志字段|1位标志位。若CSPS标识为’1’，则该字段表示2.7.9中规定的哪个限制适用于分组速率。若CSPS标识为’0’，则该字段的含义未定义。|
|reserved_bits|保留位字段|7位字段。固定值应为’111 1111’定|
|stream_id |流标识字段|8位字段。指示其后的P-STD_buffer_bound_scale和P-STD_buffer_size_bound字段所涉及的流的编码和基本流号码。<br/>若取值’1011 1000’，则其后的P-STD_buffer_bound_scale和P-STD_buffer_size_bound字段指节目流中所有的音频流；<br/>若取值’1011 1001’，则其后的P-STD_buffer_bound_scale和P-STD_buffer_size_bound字段指节目流中所有的视频流。|
|P-STD_buffer_bound_scale|P-STD缓冲区界限比例字段|1位字段。表示用于解释后续P-STD_buffer_size_bound字段的比例系数。<br/>若前面的stream_id表示一个音频流，则该字段值为’0’。<br/>若表示一个视频流，则该字段值为’1’。对于所有其它的流类型，该字段值可以为’0’也可以为’1’。|
|P-STD_buffer_size_bound P-STD|缓冲区大小界限字段|13位无符号整数，取值不小于节目流中流n的所有分组的P-STD缓冲区大小BSn的最大值。<br/>若P-STD_buffer_bound_scale的值为’0’，则该字段以128字节为单位来度量缓冲区大小的边界。|

- **C. PSM Header**

|字段|含义|备注|
|:--|:--|:--|
|packet_start_code_prefix|分组起始码前缀字段|24位码。它和跟随其后的map_stream_id共同组成一个分组起始码以标志分组的开始。该字段是值为’0000 0000 0000 0000 0000 0001’ (0x000001)的位串|
|map_stream_id|映射流标识字段|8位字段，值为0xBC|
|program_stream_map_length|节目流映射长度字段|16位字段。指示紧跟在该字段后的program_stream_map中的字节数|
|current_next_indicator|当前下一个指示符字段|1位字段。置’1’时表示传送的节目流映射当前是可用的。置’0’时表示传送的节目流映射还不可用，但它将是下一个生效的表|
|reserved|保留字段|2位|
|program_stream_map_version|节目流映射版本字段|5位字段，表示整个节目流映射的版本号。一旦节目流映射的定义发生变化，该字段将递增1，并对32取模。<br/>在current_next_indicator为’1’时，该字段应该是当前适用的节目流映射的版本号；<br/>在current_next_indicator为’0’时，该字段应该是下一个适用的节目流映射的版本号。|
|reserved|保留字段|7位|
|marker_bit|标志位|1位字段，取值为’1’|
|program_stream_info_length|节目流信息长度字段|16位字段，指出紧跟在该字段后的描述符的总长度|
|elementary_stream_map_length|基本流映射长度字段|16位字段，指出在该节目流映射中的所有基本流信息的字节长度。它只包括stream_type、elementary_stream_id和elementary_stream_info_length字段|
|stream_type|流类型字段|MPEG-4 视频流： 0x10；H.264 视频流： 0x1B；SVAC 视频流： 0x80；G.711 音频流： 0x90；<br/>G.722.1 音频流： 0x92；G.723.1 音频流： 0x93；G.729 音频流： 0x99；SVAC音频流： 0x9B|
|elementary_stream_id|基本流标识字段|8位字段，指出该基本流所在PES分组的PES分组标题中stream_id字段的值。（这个字段的定义，其中0x(C0DF)指音频，0x(E0EF)为视频）|
|elementary_stream_info_length|基本流信息长度字段|16位字段，指出紧跟在该字段后的描述符的字节长度。(即这个类型的流描述长度。这个后面的字段后面的指定长度不在elementary_stream_map_length指定的范围类。)|
|CRC_32|CRC|32位字段|
||||

- **D. PES Header**

|字段|含义|备注|
|:--|:--|:--|
|packet_start_code_prefix|分组起始码前缀字段|24位代码，它和后面的stream_id构成了标识分组开始的分组起始码。它是一个值为’0000 0000 0000 0000 0000 0001’ (0x000001)的位串。|
|stream_id|流标识字段|在节目流中，它规定了基本流的号码和类型|
|PES_packet_length|PES分组长度字段|16位字段，指出了PES分组中跟在该字段后的字节数目|
|PES_scrambling_control|PES加扰控制字段|2位字段，表示PES分组有效负载的加扰方式。当加扰发生在PES层，PES分组标题，如果有可选字段的话也包括在内，有一些字段不应被加扰|
|PES_priority|PES优先级字段|1位字段，指示PES分组中有效负载的优先级。'1’表示PES分组中有效负载的优先级高于该字段为’0’的PES分组有效负载。|
|data_alignment_indicator|数据对齐指示符字段|1位标志。置’1’时表示PES分组标题后紧跟着在data_alignment_indicator所指出的视频起始码或音频同步字，如果有data_alignment_indicator描述符的话。<br/>若其值为’1’且无该描述符，则需要和alignment_type '01’所表示的对齐。<br/>当值为’0’时，没有定义是否有任何此种的对齐。|
|copyright|版权字段|1位字段。置’1’时表示相关PES分组有效负载的材料受到版权保护。当值为’0’时，没有定义该材料是否受到版权保护|
|original_or_copy|原始或拷贝字段|1位字段。置’1’时表示相关PES分组有效负载的内容是原始的；值为’0’表示相关PES分组有效负载的内容是一份拷贝。|
|PTS_DTS_flags|PTS DTS标志字段|2位字段。当值为’10’时，PTS字段应出现在PES分组标题中；当值为’11’时，PTS字段和DTS字段都应出现在PES分组标题中；当值为’00’时，PTS字段和DTS字段都不出现在PES分组标题中。值’01’是不允许的。|
|ESCR_flag|ESCR标志字段|1位标志。置’1’时表示ESCR基础和扩展字段出现在PES分组标题中；值为’0’表示没有ESCR字段。|
|ES_rate_flag|ES速率标志字段|1位标志。置’1’时表示ES_rate字段出现在PES分组标题中；值为’0’表示没有ES_rate字段。|
|DSM_trick_mode_flag|DSM特技方式标志字段|1位标志。置’1’时表示有8位特技方式字段；值为’0’表示没有该字段。|
|additional_copy_info_flag|附加版权信息标志字段|1位标志。置’1’时表示有附加拷贝信息字段；值为’0’表示没有该字段。|
|PES_CRC_flag|PES CRC标志字段|1位标志。置’1’时表示CRC字段出现在PES分组标题中；值为’0’表示没有该字段。|
|PES_extension_flag|PES扩展标志字段|1位标志。置’1’时表示PES分组标题中有扩展字段；值为’0’表示没有该字段。|
|PES_header_data_length|PES标题数据长度字段|8位字段。指出包含在PES分组标题中的可选字段和任何填充字节所占用的总字节数。该字段之前的字节指出了有无可选字段。|
||||

- **PES packet**
1. 数据包负载