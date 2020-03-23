## Python 操作数据库 ##

### 简单项目使用 ###

```python
# pip install mysql-connector

# -*- coding: UTF-8 -*-
import mysql.connector
# 打开数据库连接
db = mysql.connector.connect(
       host="localhost",
       user="root",
       passwd="XXX", # 写上你的数据库密码
       database='wucai', 
       auth_plugin='mysql_native_password'
)
# 获取操作游标 
cursor = db.cursor()
# 执行SQL语句
cursor.execute("SELECT VERSION()")
# 获取一条数据
data = cursor.fetchone()
print("MySQL版本: %s " % data)


# 查询身高大于等于2.08的球员
sql = 'SELECT player_id, player_name, height FROM player WHERE height>=2.08'
cursor.execute(sql)
data = cursor.fetchall()
for each_player in data:
  print(each_player)

# 关闭游标&数据库连接
cursor.close()
db.close()

```

### 复杂项目使用 - ORM（对象关系映射: Object Relation Mapping） ###

- 什么是 ORM 框架？
    1. 持久化层在业务逻辑层和数据库层起到了衔接的作用，它可以将【内存中的数据模型】和【存储模型】互转
    2. 持久性就是将对象数据永久存储在数据库中
    3. ORM提供了一种持久化模式，可以高效地对数据库进行访问
    4. 采用ORM可以从数据库的设计层面转化成面向对象的思维

- 为什么要使用 ORM 框架？
    1. 让程序员关注业务逻辑层面，而不是底层数据库该如何访问，如何编写SQL语句获取数据等等。
    2. 随着项目规模的增大，在代码层编写 SQL 语句访问数据库会降低开发效率，也会提升维护成本。
    3. ORM 抽象了数据的存取，同时也兼容多种 DBMS。
    4. ORM 当然也会付出一些代价，比如性能上的一些损失。面对一些复杂的数据查询，ORM 会显得力不从心。

- Python 中的 ORM 框架都有哪些？
    1. Django：Python 的 WEB 应用开发框架，本身走大而全的方式。
        - Django 采用了 MTV 的框架模式，包括了 Model（模型），View（视图）和 Template（模版）
        - Model 模型只是 Django 的一部分功能，我们可以通过它来实现数据库的增删改查操作。
        - 一个 Model 映射到一个数据表。

    2. SQLALchemy：常用的 ORM 框架之一。支持 ORM 和支持原生 SQL 两种方式的工具。
    3. peewee：这是一个轻量级的 ORM 框架，简单易用。