## **数据库的核心价值和内涵理解**

### **一、数据库的核心：增、删、改、查**
- **增，删，改**
    1. 数据的一致性：事务隔离机制
    ```sh
    # 四个事务隔离级别：确保事务内部和外部得独立性。
    # 解决办法： MVVC 快照机制。按照时间先后合并修改记录
    # 思考：Git 代码管理为何不使用和数据库同样按时间先后的合并机制，出现同行冲突的粉情况必须自己手动处理冲突？
    #   1. 数据库的事务确保了每一个修改有完整性和独立性，业务逻辑决定
    #   2. Git 则无法强制每一个用户上传内容时，确保内容的完整性和独立性。例如：
    #       a. 一大功能代码，可能会在不同时段提交。每个提交之间一般具有依赖关系，不像事务那样完整独立      
    ```

- 查：数据存储各种层面上的优化。
    1. 索引的常见模型
    ```sh
    # a. 哈希表索引：是一种以键 - 值（key-value）存储数据的结构 （等值查询）
    #     Memcached 及其他一些 NoSQL 引擎。
    #
    # b. 有序数组索引：只适用于静态存储引擎（范围/等值查询）
    #     
    # c. N叉树，跳表索引： N 叉树由于在读写上的性能优点，以及适配磁盘的访问模式 （范围/等值/修改）
    # 
    ```






















