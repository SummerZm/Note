## **Opemcu RTSP framework**

### **MCURtspServer**
- 【member】MCUH323EndPoint *ep： 即是一台服务也是一个终端
- 【member】MCUSipEndPoint *sep

### **MCURtspConnection**
- 【inherit】MCUSipConnection --> 【inherit】MCUH323Connection
-  通过传入的SIP消息的SDP内容，构建出sip对象；通过该对象建立RSTP链接

### **ConferenceStreamMember**
- 【inherit】 ConferenceMember


