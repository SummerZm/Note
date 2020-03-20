## 数据库中连接 ##
- 总述
    1. 内连接：将多个表之间满足连接条件的数据行查询出来。它包括了等值连接、非等值连接和自连接
    2. 外连接：会返回一个表中的所有记录，以及另一个表中匹配的行。它包括了左外连接、右外连接和全连接
    3. 交叉连接：也称为笛卡尔积，返回左表中每一行与右表中每一行的组合


- 连接注意事项
    1. SQL-99的交叉连接更清爽,可读。
        ```SQL
            SELECT ...
            FROM table1
                JOIN table2 ON table1和table2的连接条件
                    JOIN table3 ON table2和table3的连接条件
        ```
    2. 不是所有的DBMS 都支持全外连接。MySQL、Access、SQLite、MariaDB 不支持，Oracle、DB2、SQL Server支持
    3. Oracle 中是不存在AS，使用表别名的时候，直接在表名后面写上表别名即可，比如 player p
    4. SQLite 的外连接只有左连接SQLite 是一款轻量级的数据库软件
    5. 使用自连接取代子查询， 子查询是通过未知表进行查询后的条件判断，自连接是通过已知的自身数据表进行条件判断

- 笛卡尔乘积
    1. 数学运算。假设有两个集合X和Y，那么X和Y的笛卡尔积就是X和y的所有组合。
    ```SQL
        -- SQL-92
        SELECT * FROM player, team
        -- SQL-99
        SELECT * FROM player CROSS JOIN team
    ``` 

- 等值连接
    ```SQL
        -- SQL-92
        SELECT player_id, player.team_id, player_name, height, team_name
        FROM player AS a, team AS b 
        WHERE a.team_id = b.team_id
        -- SQL-99
        SELECT player_id, team_id, player_name, height, team_name 
        FROM player NATURAL JOIN team 

        SELECT player_id, player.team_id, player_name, height, team_name
        FROM player JOIN team 
        ON player.team_id = team.team_id

        SELECT player_id, team_id, player_name, height, team_name 
        FROM player JOIN team USING(team_id)
    ```

- 非等值连接
    ```SQL
        -- SQL-92
        SELECT p.player_name, p.height, h.height_levelFROM player AS p, height_grades AS hWHERE p.height 
        FROM player AS p, height_grades AS h
        BETWEEN h.height_lowest AND h.height_highest
        -- SQL-99
        SELECT p.player_name, p.height, h.height_level
        FROM player as p JOIN height_grades as h
        ON height BETWEEN h.height_lowest AND h.height_highest
    ```

- 外连接
    1. 显示主表全部的行，而第剩下的表则显示对应连接的信息
    ```SQL
        -- [左外连接] SQL-92
        SELECT * FROM player, team where player.team_id(+) = team.team_id
        -- [右外连接] SQL-92
        SELECT * FROM player, team where player.team_id = team.team_id(+)

        -- SQL-99
        SELECT * FROM player RIGHT JOIN team ON player.team_id = team.team_id
        SELECT * FROM player LEFT JOIN team ON player.team_id = team.team_id

        -- MYSQL不支持全连接
    ```
- 自连接
    1. 查询条件使用了当前表的字段
    ```SQL
        -- SQL-92
        SELECT b.player_name, b.height
        FROM player as a , player as b
        WHERE a.player_name = '布雷克-格里芬' and a.height < b.height

        -- SQL-99
        SELECT b.player_name, b.height
        FROM player as a 
        JOIN player as b ON a.player_name = '布雷克-格里芬' and a.height < b.height
    ```