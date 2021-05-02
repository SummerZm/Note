## **工作中的设计问题**

### **关于获取系统版本问题**
- **现状：读系统版本问题在android是build.prop属性读不到，就用uname命令读，读不到再去/proc/cmdline中解析**
    ```sh
    # 优点：多备份机制
    # 缺点：
    #   a. 增加代码复杂度,代码量多更容易藏问题
    #   b. 上层软件多路径获取信息，增加了出问题排查的难度
    #   c. 下层软件多途径提供信息也有类似的问题
    #  
    # 思考: 以上缺点基于：经常出现问题的场景，但我们做了多途径备份，出问题的概率属于极少数，所以缺点不明显
    #   a. 因为代码量多更容易藏问题，所以多途径备份看似提高了容错率，事实上也带入隐藏了潜在问题
    #   b. 单点获取可以有时间写出高效，稳定，简洁的代码；这不也是从另一角度，降低了风险吗？
    #   
    # 这是代码设计的颗粒度控制问题
    ```