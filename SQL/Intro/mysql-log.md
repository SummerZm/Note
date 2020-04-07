## <b>Mysql日志恢复机制</b> ##

### redo log ###
1. InnoDB 引擎特有
2. redo log 是<b>物理日志</b>，记录的是“在某个数据页上做了什么修改”
3. 循环写: 不能完整持久保存
4. redo log 涉及到IO，但是顺序写，并且可以组提交

### bin log ###
1. MySQL 的 Server 层实现的，所有引擎都可以使用
2. binlog 是<b>逻辑日志</b>，记录的是这个SQL语句
3. 追加写且具有归档功能

### 数据库的一致性 ####
- 必须要保证redo log 和 bin log 日志一致
- 处理机制：
    1. 1 prepare阶段 2 写binlog 3 commit
    2. 在2之前崩溃时，重启恢复：后发现没有commit，回滚。备份恢复：没有binlog 。
    3. 当在3之前崩溃，重启恢复：虽没有commit，但prepare和binlog完整，重启后会自动commit。

- 其他
    1. bin log 并不是以数据恢复为目的的
    2. 不能只靠binlog做恢复，因为业务逻辑可能因为误删操作的行为，插入了逻辑错误的语句
    3. innodb_flush_log_at_trx_commit = 1，则每个redo log 都同步到磁盘 
    4. sync_binlog，则每个 sync_binlog 都同步到磁盘 

### 数据库日志流程简介 ### 
1. 首先客户端通过tcp/ip发送一条sql语句到server层的SQL interface
2. SQL interface接到该请求后，先对该条语句进行解析，验证权限是否匹配
3. 验证通过以后，分析器会对该语句分析,是否语法有错误等
4. 接下来是优化器器生成相应的执行计划，选择最优的执行计划
5. 之后会是执行器根据执行计划执行这条语句。
    - 在这一步会去open table,如果该table上有MDL，则等待。如果没有，则加在该表上加短暂的MDL(S)
    - 如果opend_table太大,表明open_table_cache太小。需要不停的去打开frm文件

6. 进入到引擎层
    - 首先会去innodb_buffer_pool里的data dictionary(元数据信息)得到表信息
    - 通过元数据信息,去lock info里查出是否会有相关的锁信息，并把这条update语句需要的锁信息写入到lock info里(锁这里还有待补充)

7. 涉及到的老数据通过快照的方式存储到innodb_buffer_pool里的undo page里,并且记录undo log修改的redo
    - 如果data page里有就直接载入到undo page里
    - 如果没有，则需要去磁盘里取出相应page的数据，载入到undo page里

8. 在innodb_buffer_pool的data page做update操作。并把操作的物理数据页修改记录到redo log buffer里
    - update事务可能会涉及到多个页面的修改，所以redo log buffer里可能会记录多条页面的修改信息。
    - 因为group commit的原因，这次事务所产生的redo log buffer可能会跟随其它事务一同flush并且sync到磁盘上

9. 同时修改的信息，会按照event的格式,记录到binlog_cache中。
    - 这里注意binlog_cache_size是transaction级别的,不是session级别的参数,
    - 一旦commit之后，dump线程会从binlog_cache里把event主动发送给slave的I/O线程

10. 之后把这条sql,需要在二级索引上做的修改
    - 写入到change buffer page，等到下次有其他sql需要读取该二级索引时，再去与二级索引做merge
    - 随机I/O变为顺序I/O,但是由于现在的磁盘都是SSD,所以对于寻址来说,随机I/O和顺序I/O差距不大

11. 此时update语句已经完成，需要commit或者rollback
    - commit操作，由于存储引擎层与server层之间采用的是内部XA(保证两个事务的一致性,这里主要保证redo log和binlog的原子性),所以提交分为prepare阶段与commit阶段
    - prepare阶段,将事务的xid写入，将binlog_cache里的进行flush以及sync操作(大事务的话这步非常耗时)
    - commit阶段，由于之前该事务产生的redo log已经sync到磁盘了。所以这步只是在redo log里标记commit
    - 当binlog和redo log都已经落盘以后，如果触发了刷新脏页的操作，先把该脏页复制到doublewrite buffer里
    - 把doublewrite buffer里的刷新到共享表空间，然后才是通过page cleaner线程把脏页写入到磁盘中
