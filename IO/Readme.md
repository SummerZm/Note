## **常见IO简介**

### **IO库简介**
- **Libevent、libev、libuv三个网络库，都是c语言实现的异步事件库**
    1. libevent :名气最大，应用最广泛，历史悠久的跨平台事件库；
    2. libev :较libevent而言，设计更简练，性能更好，但对Windows支持不够好；
    3. libuv :开发node的过程中需要一个跨平台的事件库，他们首选了libev，但又要支持Windows，故重新封装了一套，linux下用libev实现，Windows下用IOCP实现；

