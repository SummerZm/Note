## **Live555 工程角度代码结构分析**
> 引子：如何阅读源码？

### **基类1.TasKScheduler**
> 引子：调度器一般哪些功能？  
 
![TaskScheduler0.png](Image/TaskScheduler0.png)      

![TaskScheduler1.png](Image/TaskScheduler1.png)    

### **基类2.UsageEnvironment**

![UsageEnvironment.png](Image/UsageEnvironment.png) 

### **基类3.GroupSocket** 

![GroupSocket.png](Image/GroupSocket.png) 

### **基类4.Medium**
> **把 Medium 放到不同的 envir 里;要点；C++ 静态成员函数 - 性能 - 包裹调用.**   

![medium0.png](Image/medium0.png)
![medium1.png](Image/medium1.png)
![medium2.png](Image/medium2.png)