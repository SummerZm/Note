### **[Basic and Important](http://www.eecs.berkeley.edu/~rcs/research/interactive_latency.html)** ###
#### 1. 纳秒ns级 ####
|Path|CostTime|
|:--|:--|
|一个CPU周期|0.xns  【时钟频率相关[1/HZ】|
|一条指令|1ns   【几个时钟周期】|
|Read 1L Cache|1ns |
|branch misprediction|3ns 【分支预测错误】|
|Read 2L Cache|4ns|
|Metux lock/unlock|17ns|
|Read main memory|100ns 【读取主存】|
|||

#### 2. 微秒ns级 ####
|Path|CostTime|
|:--|:--|
|SSD IO|10~30 us 【SSD磁盘一次IO】|
|SSD read 1MB sequentially|59us 【SSD磁盘顺序读到1MB数据】|
|Java native function call|1~10us   【Java 本地方法调用】|
|Read 1MB from memory sequentially|3us  【内存属顺序读1MB】|
|Network transfer 2KB|1us 【网络传输2K字节】|
|||

#### 3. 毫秒ms级 ####
|Path|CostTime|
|:--|:--|
|Redis read|0.5ms 【从远程分布式缓存Redis读取一个数据】|
|Disk read 1MB sequentially|1ms 【机械磁盘顺序读取1MB数据】|
|Disk location|2ms 【机械磁盘一次寻址定位】|
|SQL query a record|10~20ms 【数据库查询一条记录（有索引）】|
|互联网：旧金山到纽约|40ms|
|互联网：旧金山到英国|81ms|
|互联网：旧金山到澳大利亚|183ms|
|||

#### 4. 分秒s级 ####
|Path|CostTime|
|:--|:--|
|TCP 包重传|1~3s|
|TCP keepalive设置|设置成ms级容易断连|
|OS虚拟化系统重启|4s|
|硬件虚拟化系统重启|40s|
|物理服务器重启|5min|
|||

- [Teach Yourself Programming in Ten Years](http://norvig.com/21-days.html#answers)
- [Latency Intro](https://www.iteye.com/blog/shihlei-2276438)
