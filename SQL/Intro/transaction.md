## <b>SQL事务</b> ##
> MySQL5.5之前，默认的存储引擎是 MyISAM，5.5版本之后默认存储引擎是 InnoDB。它们的区别之一是InnoDB支持事务

- <b>SQL事务的特性</b>
1. 原子性（Atomicity）：要么完全执行，要么都不执行。
2. 一致性（Consistency）：当事务提交后，或者当事务发生回滚后，数据库的完整性约束不能被破坏。
3. 隔离性（Isolation）：每个事务都是彼此独立的，不会受到其他事务的执行影响。一个事务在提交之前，对其他事务都是不可见的。
4. 持久性（Durability）：系统崩溃或者存储介质发生故障，数据修改依然是有效的。通过日志，让系统恢复到最后一次成功的更新状态。
5. 原子性是基础，隔离性是手段，一致性是约束条件，持久性是目的。

- <b>事务基本语法</b>
1. START TRANSACTION 或者 BEGIN，作用是显式开启一个事务。
2. COMMIT：提交事务。当提交事务后，对数据库的修改是永久性的。
3. ROLLBACK 或者 ROLLBACK TO [SAVEPOINT]，意为回滚事务。
4. SAVEPOINT：在事务中创建保存点，方便后续针对保存点进行回滚。一个事务中可以存在多个保存点。
5. RELEASE SAVEPOINT：删除某个保存点。
6. SET TRANSACTION，设置事务的隔离级别。
7. CURSOR，如何使用游标
    ```SQL    
    CREATE PROCEDURE `alter_attack_growth`()
    BEGIN
        -- 创建接收游标的变量
        DECLARE temp_id INT;  
        DECLARE temp_growth, temp_max, temp_start, temp_diff FLOAT;  

        -- 创建结束标志变量  
        DECLARE done INT DEFAULT false;
        -- 【定义游标】     
        --  DECLARE cur_hero CURSOR FOR SELECT id, attack_growth, attack_max, attack_start FROM heros; for oracle/Postgre SQL
        DECLARE cur_hero CURSOR FOR SELECT id, attack_growth, attack_max, attack_start FROM heros;
        -- 指定游标循环结束时的返回值  
        DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = true;  
        -- 【打开游标】
        OPEN cur_hero;  
        -- 【读取游标】
        FETCH cur_hero INTO temp_id, temp_growth, temp_max, temp_start;
        REPEAT
            IF NOT done THEN
                SET temp_diff = temp_max - temp_start;
                IF temp_growth < 5 THEN
                    IF temp_diff > 200 THEN
                            SET temp_growth = temp_growth * 1.1;
                    ELSEIF temp_diff >= 150 AND temp_diff <=200 THEN
                            SET temp_growth = temp_growth * 1.08;
                    ELSEIF temp_diff < 150 THEN
                            SET temp_growth = temp_growth * 1.07;
                    END IF;                       
                ELSEIF temp_growth >=5 AND temp_growth <=10 THEN
                    SET temp_growth = temp_growth * 1.05;
                END IF;
                UPDATE heros SET attack_growth = ROUND(temp_growth,3) WHERE id = temp_id;
            END IF;
        FETCH cur_hero INTO temp_id, temp_growth, temp_max, temp_start;
        UNTIL done = true END REPEAT;
        -- 【关闭游标】
        CLOSE cur_hero;
        -- 【释放游标】
        DEALLOCATE cur_hero; 
    END
    ```

- <b>事务的提交</b>
1. 事务有两种方式，分别为隐式事务和显式事务。隐式事务实际上就是自动提交
2. Mysql设置事务提交方式：set autocommit =0/1/2
    ```SQL
    -- completion=0：执行COMMIT提交事务,需要使用 START TRANSACTION 或者 BEGIN开启新事务
    -- completion=1：提交事务后，相当于执行了 COMMIT AND CHAIN，也就是开启一个链式事务【每个语句都处于事务中】
    -- completion=2，这种情况下 COMMIT=COMMIT AND RELEASE，也就是当我们提交后，会自动与服务器断开连接。
    ```

- <b>事务并发处理异常</b>

1. 脏读（Dirty Read）: 在事务A读到了事务B还没有提交的数据
2. 不可重复读（Norepeatable Read）: 在事务A内，同一条记录，两次读取的结果不同
3. 幻读（Phantom Read）: 一个事务读取了另一个事务插入的新纪录

- <b>事务的隔离级别</b>
1. 隔离级别简介
    ```SQL
    -- 【读未提交】 一个事务读取了另一个事务改写但还未提交的数据
    
    -- 【读已提交】 只能读到已经提交的内容，可以避免脏读的产生，常见的默认隔离级。
    -- 事务A一开始读到的数据是xxx, 事务B在事务A结束前修改这数据为yyy并且提交成功，事务A再次去读时为yyy
    -- 想要避免不可重复读或者幻读，需要在SQL查询的时候编写带加锁的 SQL 语句
    
    -- 【可重复读】 保证一个事务在相同查询条件下两次查询得到的数据结果是一致的
    -- 事务A一开始读到的数据是xxx, 事务B在事务A结束前修改这数据为yyy并且提交成功，事务A再次去读时仍为xxx
    -- MySQL 默认的隔离级别就是可重复读

    -- 【可串行化】将事务进行串行化，也就是在一个队列中按照顺序执行，可串行化是最高级别的隔离等级

    -- 四个级别分别
    -- 无限制（可以并发读写）
    -- 写事务加行锁（可以并发读、读完立刻释放锁而不是等事务结束）
    -- 读写事务都加行锁（可以并发读、读写都是事务结束才释放锁）
    -- 表锁（读写事务序列化执行、单线程执行）。 时间开销依次递增所以随着隔离等级递增并发性能会降低。
    ```

2. 隔离级别对并发异常的影响
    |       |脏读 |不可重复读|幻读|
    |:--:|:---:|:--:|:--:|
    |读未提交|YES |YES      |YES |
    |读已提交|NO  |YES      |YES |
    |可重复读|NO  |NO       |YES |
    |可串行化|NO  |NO       |NO  |

- <b>长事务问题排查</b>
    1. 查找耗时大于10s的事务
    ```SQL
        select * from information_schema.innodb_trx where TIME_TO_SEC(timediff(now(),trx_started))>10
    ```
