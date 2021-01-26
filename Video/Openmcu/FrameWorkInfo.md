## **PTlib框架信息**
> **如何组织粘合一个框架？需求是什么？**

### **1. 目录结构信息**
### **2. 代码流程脉络**
- **Openmcu: 编码支持以插件的形式编入项目中. [MCUpluginCodecManager()]**
- **Openmcu: 开启用户认证时，从配置文件中的读取需要验证的用户密码信息. [InitAccount()-reg.h]**
- **Openmcu: 有一个用户队列Account和已经注册的用户队列bookAccount**
- **Openmcu: 注册创建了一个Regconnection, 然后保存利用已有Connection创建SipConnection.**
- **Openmcu: 有各种monitor和manager**

### **3. 软件线程是如何跑起来的**
- ****
- 
- 
- 