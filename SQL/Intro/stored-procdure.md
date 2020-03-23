## <b>SQL存储过程</b> ##

> 存储过程是程序化的 SQL，可以直接操作底层数据表

- <b>基本语法</b>
    ```SQL
    -- SELECT…INTO：把从数据表中查询的结果存放到变量中，也就是为变量赋值
    -- IF…THEN…ENDIF：条件判断语句，我们还可以在 IF…THEN…ENDIF 中使用 ELSE 和 ELSEIF 来进行条件判断
    -- CASE：CASE 语句用于多条件的分支判断，使用的语法是下面这样的
    -- WHEN expression1 THEN ...  WHEN expression2 THEN ... 
    -- ELSE 
    -- LOOP、LEAVE 和 ITERATE：LOOP 是循环语句，使用 LEAVE 可以跳出循环，使用 ITERATE 则可以进入下一次循环
    -- REPEAT…UNTIL…END REPEAT：这是一个循环语句，首先会执行一次循环，然后在 UNTIL 中进行表达式的判断
    -- WHILE…DO…END WHILE：这也是循环语句，先进行条件判断

    CREATE PROCEDURE 存储过程名称([参数列表])
    BEGIN
        需要执行的语句
    END

    CREATE PROCEDURE `add_num`(IN n INT)
    BEGIN
        DECLARE i INT;
        DECLARE sum INT;
        
        SET i = 1;
        SET sum = 0;
        WHILE i <= n DO
                SET sum = sum + i;
                SET i = i +1;
        END WHILE;
        SELECT sum;
    END

    -- DELIMITER 定义临时结束符
    -- 我们不希望 SQL 逐条执行，而是采用存储过程整段执行的方式，因此需要临时定义新的 DELIMITER
    DELIMITER //
    CREATE PROCEDURE `add_num`(IN n INT)
    BEGIN
        DECLARE i INT;
        DECLARE sum INT;
        
        SET i = 1;
        SET sum = 0;
        WHILE i <= n DO
                SET sum = sum + i;
                SET i = i +1;
        END WHILE;
        SELECT sum;
    END //
    DELIMITER ;

    -- 参数类型： IN， OUT， INOUT
    CREATE PROCEDURE `get_hero_scores`(
        OUT max_max_hp FLOAT,
        OUT min_max_mp FLOAT,
        OUT avg_max_attack FLOAT,  
        s VARCHAR(255)
        )
    BEGIN
        SELECT MAX(hp_max), MIN(mp_max), AVG(attack_max) FROM heros WHERE role_main = s INTO max_max_hp, min_max_mp, avg_max_attack;
    END

    -- 调用存储过程
    CALL get_hero_scores(@max_max_hp, @min_max_mp, @avg_max_attack, '战士');
    SELECT @max_max_hp, @min_max_mp, @avg_max_attack;

    ```
- <b>储存过程优缺点</b>
1. 存储过程只在创造时进行编译，之后的使用都不需要重新编译，执行效率高
2. 存储过程的安全性强，我们在设定存储过程的时候可以设置对用户的使用权限
3. 减少网络传输量，因为代码封装到存储过程中，每次使用只需要调用存储过程
4. 进行相对复杂的数据库操作时，使用一条一条的SQL语句，可能要连接多次，现在变成了一次存储过程一次连接

5. 可移植性差，存储过程不能跨数据库移植
6. 调试困难，只有少数 DBMS 支持存储过程的调试
7. 存储过程的版本管理也很困难，比如数据表索引发生变化了，可能会导致存储过程失效
8. 存储过程本身没有版本控制，版本迭代更新的时候很麻烦
9. 高并发场景要减少数据库的压力, 有时数据库会分库分表，对可扩展性要求很高，这时存储过程会很难维护，增加数据库的压力

11. 微软、IBM 等公司使用存储过程, 国内的阿里并不推荐开发人员使用存储过程 

