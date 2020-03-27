## MYSQL数据恢复 ##

### .ibd 文件损坏了，找回数据 ###
- 工具：Percona Data Recovery Tool for InnoDB
- 原理：Delete 只是标记物理页为脏页,不会立马回收数据，发现损坏时，立刻停止写入
- Binlog开启可以使用闪回工具，比如 mysqlbinlog 或者 binlog2sql
- InnoDB 是有自动恢复机制的，如果自动修复错误失效。可以调整参数nnodb_force_recovery人工恢复
    ```SQL
    -- innodb_force_recovery有7种状态
    -- 0表示不进行强制恢复
    -- 4以上可能造成不可恢复性错
    -- 大于 0 时，只能进行 SELECT 读取操作，还是有限制的读取
    show variables like 'innodb_force_recovery
    ```
- 恢复流程
    1. 开启 innodb_force_recovery
    2. 用非innodb引擎创建新表 【通常：MyIsam】
    3. 通过"select * from user limit N" 逐步调整 N 确认数据损坏的位置
    4. 将正常的数据导入新表
    5. 关闭innodb_force_recovery
    6. 重启切换引擎