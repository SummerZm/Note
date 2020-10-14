## MediaSink 介绍
> Sink 对象主要作用是消费数据

### **一、 Sink 继承关系**

> **中间层对于一个系统的意义？**

![mediaSink](../Image/mediaSink.png)

### **二、接口介绍**
> **从源码中isXXXX()函数以小窥大，区分框架需求与实现细节的差异**   

**A. 框架如何解耦？**  [wrap函数]  
**B. 框架如何进行埋点？**    [callback函数]  
**C. 框架如何管理不同组件之间共性与差异?** [isXXX()函数and派生]

![mediaSink1](../Image/mediaSink1.png)
![mediaSink1](../Image/mediaSink2.png)

### **相关类图 [Media]**
![media](../Image/medium1.png)