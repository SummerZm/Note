## SQL中的视图 ##

### <b>什么是视图</b> ###
- 视图作为一张虚拟表，封装了底层与数据表的接口。相当于是一张表或多张表的数据结果集。
- 视图可以帮我们简化复杂的SQL查询，可以重用，而不需要考虑视图中包含的基础查询的细节。
- 视图是SELECT语句的集合，执行时会提前编译好，可以反复使用。在底层执行顺序的时候和SELECT语句是一样的。

### <b>如何创建、更新和删除视图</b> ###
- 视图基本语法：
    ```SQL
        -- 创建视图   
        -- 创建好一张视图之后，还可以在它的基础上继续创建视图， 可以把视图当成表来用
        CREATE VIEW view_name AS
        SELECT column1, column2
        FROM table
        WHERE condition

        -- 更新视图
        ALTER VIEW view_name AS
        SELECT column1, column2
        FROM table
        WHERE condition

        -- 删除视图
        DROP VIEW view_name
    ```

### <b>如何使用视图来简化我们的 SQL 操作</b> ###
- 视图的用途：
    ```SQL
        -- 格式化数据
        CREATE VIEW player_team AS 
        SELECT CONCAT(player_name, '(' , team.team_name , ')') AS player_team 
        FROM player JOIN team WHERE player.team_id = team.team_id
        -- 使用视图计算字段
        CREATE VIEW game_player_score AS
        SELECT game_id, player_id, (shoot_hits-shoot_3_hits)*2 AS shoot_2_points, shoot_3_hits*3 AS shoot_3_points, shoot_p_hits AS shoot_p_points, score
        FROM player_score
        -- 使用视图简化连接语句
     ```

### <b>视图和临时表的区别是什么，它们各自有什么优缺点</b> ###
- 针对不同用户开放不同的数据查询权限
- 视图对底层数据表的数据进行修改会受到很多限制
- 临时表是真实存在的数据表，不过它不用于长期存放数据，只为当前连接存在，关闭连接后，临时表就会自动释放。

