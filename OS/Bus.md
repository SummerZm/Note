## **总线简介**

### **一、计算机总线图**
**A. PCIe(Peripheral Component Interconnect Express)总线：速率几十Gb/s；并行总线架构。**  
**B. SCSI(Small Computer System Interface)：速率 640MB/s;高速硬盘。**  
**C. USB(Univversal Serial Bus): 速率一代12 Mb/s;二代480Mb/s;三代5Gb/s。**  
**D. 其他总线**
```sh  
#1. PCIe(Peripheral Component Interconnect)  
#2. DDR3/4  
#3. ISA(Industry Standard Architecture)  
```
![image](Image/Bus.png)

### **二、计算机中断**
**A. 从操作系统层看中断的种类：1.软中断；2.硬中断**  
**B. 从硬件层简述IO发生时的处理方式：**  
    1. 阻塞：用户层程序等待内核层系统调用完成.  
    2. 中断：用户层程序不等待内核层系统调用完成，不阻塞；通过中断通知IO事件的到来/结束.  
    3. DMA： 直接内存访问芯片；把一些IO事件交给的专门的芯片处理.  
