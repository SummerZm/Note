## <b>数据库基本技巧</b> ##
- <b>MYSQL 引擎查看</b>
    ```SQL
    show engines;
    +--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
    | Engine             | Support | Comment                                                        | Transactions | XA   | Savepoints |
    +--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
    | FEDERATED          | NO      | Federated MySQL storage engine                                 | NULL         | NULL | NULL       |
    | MRG_MYISAM         | YES     | Collection of identical MyISAM tables                          | NO           | NO   | NO         |
    | MyISAM             | YES     | MyISAM storage engine                                          | NO           | NO   | NO         |
    | BLACKHOLE          | YES     | /dev/null storage engine (anything you write to it disappears) | NO           | NO   | NO         |
    | CSV                | YES     | CSV storage engine                                             | NO           | NO   | NO         |
    | MEMORY             | YES     | Hash based, stored in memory, useful for temporary tables      | NO           | NO   | NO         |
    | ARCHIVE            | YES     | Archive storage engine                                         | NO           | NO   | NO         |
    | InnoDB             | DEFAULT | Supports transactions, row-level locking, and foreign keys     | YES          | YES  | YES        |
    | PERFORMANCE_SCHEMA | YES     | Performance Schema                                             | NO           | NO   | NO         |
    +--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
    9 rows in set (0.02 sec)
    ```

- <b>SQL语句性能分析</b>
    ```SQL
    -- 查看性能分析功能是否开
    select @@profiling;
    set profiling=1;

    -- 执行SQL
    select * from users;
    show profile;

    -- 结果
    +----------------------+----------+
    | Status               | Duration |
    +----------------------+----------+
    | starting             | 0.000060 |
    | checking permissions | 0.000010 |
    | Opening tables       | 0.000018 |
    | init                 | 0.000015 |
    | System lock          | 0.000005 |
    | optimizing           | 0.000005 |
    | statistics           | 0.000012 |
    | preparing            | 0.000010 |
    | executing            | 0.000002 |
    | Sending data         | 0.000038 |
    | end                  | 0.000004 |
    | query end            | 0.000005 |
    | closing tables       | 0.000008 |
    | freeing items        | 0.000010 |
    | cleaning up          | 0.000009 |
    +----------------------+----------+
    15 rows in set, 1 warning (0.00 sec)
    ```

- <b>Mysql 查看连接池</b>
    ```SQL
    -- 长连接过多会导致内存被大量占用 
    -- MySQL 5.7 以上版本，每次执行一个比较大的操作后，可执行 mysql_reset_connection 重新初始化连接资源。这个过程不需要重连和重新做权限验证
    show processlist;
    +------+------+-----------+------+---------+------+-------+------------------+
    | Id   | User | Host      | db   | Command | Time | State | Info             |
    +------+------+-----------+------+---------+------+-------+------------------+
    | 3780 | root | localhost | NULL | Query   |    0 | init  | show processlist |
    +------+------+-----------+------+---------+------+-------+------------------+
    1 row in set (0.00 sec)
    ```

- <b>SQL常用命令</b>
    ```SQL
    -- 创建一个名为nba的数据库
    CREATE DATABASE nba; 
    DROP DATABASE nba; 

    -- 创建表结构的语法：CREATE TABLE [table_name](字段名 数据类型，......)
    -- 语句最后以分号（;）作为结束符
    -- 最后一个字段的定义结束后没有逗号
    CREATE TABLE player (
        player_id int(11) NOT NULL AUTO_INCREMENT,
        player_name varchar(255) NOT NULL
    );

    -- SQL语句的组织顺序 【很重要】
    -- SELECT ... FROM ... WHERE ... GROUP BY ... HAVING ... ORDER BY ...

    -- 添加字段
    ALTER TABLE player ADD (age int(11));
    -- 修改字段
    ALTER TABLE player RENAME COLUMN age to player_age3. 
    -- 修改字段的数据类型
    MODIFY (player_age float(3,1));4. 
    -- 删除字段
    ALTER TABLE player DROP COLUMN player_age;

    -- DISTINCT去重
    -- DISTINCT 需要放到所有列名的前面【很重要】
    -- DISTINCT 是对后面所有列名的组合进行去重【很重要】
    SELECT DISTINCT attack_range FROM heros

    -- 查询模板
    SELECT name, role_main, role_assist, hp_max, mp_max, birthdate
    FROM heros 
    WHERE (role_main IN ('法师', '射手') OR role_assist IN ('法师', '射手')) 
    AND DATE(birthdate) NOT BETWEEN '2016-01-01' AND '2017-01-01'
    ORDER BY (hp_max + mp_max) DESC

    -- 聚集函数
    -- 自动忽略值为 NULL 的数据行
    SELECT COUNT(*), AVG(hp_max), MAX(mp_max), MIN(attack_max), SUM(defense_max)
    FROM heros
    WHERE role_main = '射手' or role_assist = '射手'

    -- 去除重复的hp_maz再平均
    SELECT ROUND(AVG(DISTINCT hp_max), 2) FROM heros

    -- 先进行分组再统计
    SELECT COUNT(*), role_assist FROM heros GROUP BY role_assist
    SELECT COUNT(*) as num, role_main, role_assist FROM heros GROUP BY role_main, role_assist ORDER BY num DESC

    -- Having 是对group by 进行限制的           【很重要】
    -- select 在 group by 和 having 之后执行    【很重要】
    -- 聚集函数在 group by之后 和 having之前执行 【很重要】
    SELECT COUNT(*) as num, role_main, role_assist
    FROM heros
    GROUP BY role_main, role_assist 
    HAVING num > 5 
    ORDER BY num DESC

    -- 关联子查询和非关联子查询 - 子查询执行的次数
    -- 子查询关键词:  存在性检测子查询EXISTS; 集合比较子查询 IN、SOME、 ANY 和 ALL
    -- IN是先执行子查询，得到一个结果集，将结果集代入外层谓词条件执行主查询，子查询只需要执行一次；
    -- EXISTS是先从主查询中取得一条数据，再代入到子查询中，执行一次子查询，判断子查询是否能返回结果，主查询有多少条数据，子查询就要执行多少次。
    -- 判断标准：“小表驱动大表”
    SELECT * FROM A WHERE cc IN (SELECT cc FROM B) 
    SELECT * FROM A WHERE EXIST (SELECT cc FROM B WHERE B.cc=A.cc)

    -- 子查询作为主查询的列，使用子查询作为计算字段出现在SELECT主查询中
    SELECT team_name, (SELECT count(*) FROM player WHERE player.team_id = team.team_id) AS player_num
    FROM team

    -- 表锁
    LOCK TABLE product_comment READ;
    UNLOCK TABLE;

    -- 行锁
    SELECT comment_id, product_id, comment_text, user_id 
    FROM product_comment 
    WHERE user_id = 912178 LOCK IN SHARE MODE

    -- 查找耗时大于10s的事务
    -- 监控 information_schema.Innodb_trx 表，设置长事务阈值，超过就报警 / 或者 kill
    select * from information_schema.innodb_trx where TIME_TO_SEC(timediff(now(),trx_started))>10
    ```
    
- <b>SQL函数</b>
    1. SQL函数与SQL基础数据类型一一对应
        - 算术函数
        - 字符串函数 [兼容问题]
        - 日期函数 [性能问题]
        - 转换函数
    
- <b>数据表的常见约束</b>
    1. 主键约束 [UNIQUE+NOT NULL]  可以由多个字段复合组成
    2. 外键约束 [被关联主键的值or空]
    3. 唯一性约束 [相当于一个约束和普通索引]
    4. NOT NULL
    5. DEFAULT  [字段的默认值]
    6. CHECK约束 [ CHECK(height>=0 AND height<3) ]

- <b>MYSQL 相关信息查看</b>
    ```SQL
    -- 查看IO单位页大小
    show variables like '%innodb_page_size%';
    -- 是否开启自适应索引
    show variables like '%adaptive_hash_index';
    -- innodb_file_per_table=ON，意味着每张表都会单独保存为一个.ibd 文件。
    show variables like 'innodb_file_per_table';
    -- 查看缓冲池的大小
    show variables like 'innodb_buffer_pool_size'
    -- 设置缓冲池的大小128M
    set global innodb_buffer_pool_size=134217728 
    -- 查看缓冲池个数
    show variables like 'innodb_buffer_pool_instances';
    -- 查看SQL语句的执行成本，输出查询访问的页数和查询花费时间
    show status like 'last_query_cost';
    -- 查看是否启用了查询缓存 
    show variables like '%query_cache%';
    ```

- <b>Mysql其他</b>
    1. 在一个表上有更新的时候，跟这个表有关的查询缓存会失效