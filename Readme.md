# **Introduction** #

## **Preview** ##

## **Directories intro** ##

### **[Latency in computer](./Hareware/latency.md)** ###
|Time Cost||
|:--|:--|
|纳秒级|cpu计算cache访问|
|10~100纳秒级|cpu访问内存|
|微秒级|内存读取1M数据|
|百微秒级|1.SSD读取1M数据; 2.局域网ping; 3.Redis一次查询|
|毫秒级|机械硬盘读取1M数据|
|10~100ms|1.域网ping; 2.局域网数据库查询|
|秒级|访问网站|
|||

### **Dir: C** ###
> From 《C expert》

|File|Comment|
|:---|:---|
|chapter01|C语言发展历史|
|chapter02-problem|C语言设计上先天缺陷|
|chapter03-pointer|C语言指针声明|
|chapter04-value|C语言赋值本质&数组|
|chapter05-compiler|C语言编译链接相关问题|
|chapter06-stack|C语言堆栈与异常处理实现|
|chapter07-history|计算机cpu发展历史&内存表示速记|
|chapter08-typecast|C语言不同标准下类型转化处理不同|
|chapter09-array|C语言数组传参|
|chapter10-volatile|C语言使用内存中而不是寄存器的数据&可见性&互斥性|
|chapter11-crlf|C语言回车换行&文件打开方式|
|chapter12-num|整型存储方式|
|chapter13-mutilfile|可执行文件的分布|
|chapter14-variable|C语言静态变量的使用|
|chapter15-thread|C语言线程安全与可重入|
|chapter16-segfault|C语言段错误排查|
|chapter17-tips|C语言技巧|

### **Dir: C++** ###

|File|Comment|
|:---|:---|
|intro.txt|C++简介|
|Exception.txt|C++异常|
|inherit.txt|C++继承|
|Note.txt|C++问题集|
|reload.txt|C++重载操作符|
|template.txt|C++模板编程|

### **Dir: History** ###
|File|Comment|
|:---|:---|
|History_computerTheory|计算机数学理论历史|
|History_math_part1|中世纪前数学史|
|History_math_part2|中世纪~费马数学史|

### **Dir: SQL** ###
|File|Comment|
|:---|:---|
|[Dir] 	[Intro](SQL/intro) | <b>SQL 基本概念简介</b>|
|[intro.md](SQL/intro/intro.md) | SQL 整体框架背景相关知识|
|[base.md](SQL/intro/base.md) | SQL 基本常用命令|
|[join.md](SQL/intro/join.md) | SQL 连接介绍|
|[mysql-log.md](SQL/intro/mysql-log.md) | mysql redo-log and bin-log|
|[stored-procdure.md](SQL/intro/stored-procdure.md) | SQL 存储过程简介 |
|[transaction.md](SQL/intro/transaction.md)|SQL 事务级别异常整体介绍|
|[view.md](SQL/intro/view.md)|SQL 视图简介|
|[Dir]	[Optimization](SQL/Optimization) | <b>SQL 优化相关</b>|
|[intro.md](SQL/Optimization/intro.md)|mysql 性能优化思路|
|[index.md](SQL/Optimization/index.md)|mysql 实现原理详细介绍|
|[lock.md](SQL/Optimization/lock.md)|mysql 锁机制与MVVC机制介绍|
|[NF.md](SQL/Optimization/NF.md)|SQL 范式理论简介|
|[optimizer.md](SQL/Optimization/optimizer.md)|mysql 查询优化器介绍|
|[storage.md](SQL/Optimization/storage.md)|mysql 数据存储原理|
|[Dir]	[Practice](SQL/Practice)|<b>SQL 实践介绍</b>|
|[master_slave_sync.md](SQL/Practice/storage.md)|mysql 主从一致简介|
|[recovery.md](SQL/Practice/storage.md)|mysql 数据恢复介绍|

### **Dir: Linux** ###
|File|Comment|
|:---|:---|
|[Dir]	[CPU](Linux/CPU)  | <b>上下文相关概念</b>  |
|[Dir]	[Disk](Linux/Disk)  | <b>文件系统，磁盘IO相关概念</b>  |
|[Dir]	[Memory](Linux/Memory)  | <b>内存指标相关知识</b>  |
|[Dir]	[Network](Linux/Network)  | <b>网络性能，nat相关知识</b>  |
|[Dir]	[Other](Linux/Other) | <b>动态追踪内核，性能优化知识</b> |

### **Dir: Algorithm** ###
> <b>编程就是在 同步，异步， 并发， 时间/空间限制不同场景下使用 判断，分支，循环对数据进行存储，分析，修改</b>
- 概念解释
    1. 数据结构：构建合适数据结构用于简洁高效的操作数据
        - 例如：环形数组
        - 难点：需求明确/逻辑严谨/接口设计相关经验

    2. 数据处理：数据和数据存储结构的修改
        - 例如：一维结构[array]改变成二维结构[set/map/tree/queue/heap]；
        - 例如：双指针去重
        - 难点：熟悉API。

    3. 数据分析：寻找数据特性，根据特性进行数据处理
        - 例如：简单数据特性 - 顺序、前N个、等等
        - 例如：复杂数据特性 - 符合某种规则, 异位词、共同树祖先、等等
        - 难点：寻找重复子问题；思维/逻辑严谨
        - 备注：一些简单数据分析问题，在数据处理时就已经被解决了

|File|Comment|
|:---|:---|
|[Dir] Base | <b>目录：基础数据结构实现</b> |
|[Dir] Leafxu | <b>目录：算法笔记，心得，总结</b> |
|[Dir] Structure|<b>目录：构建合适数据结构用于简洁高效的操作数据</b>|
|Note.md|构建简洁高效便于操作的数据结构注意点和一些实现|
|[Dir] Process|<b>目录：数据和数据存储结构的修改</b>|
|common.md|常见的数据处理转换代码模板|
|[Dir] Analize|<b>目录：寻找数据特性，根据特性进行数据处理</b>|
|qList.md|常见数据分析问题思路|
|[Dir] Leetcode | <b>目录：Leetcode题目</b> |
|[Dir] Array | <b>目录：数组题目</b> |
|Rotate.cpp|旋转/环形mod数组 - 数据结构|
|FizzBuzz.cpp|使用map映射-数据处理|
|Remove-duplicates-from-sorted-array.cpp|双指针去重 - 数据处理|
|groupAnagrams.cpp|Array->Map 转换成方便处理(统计)的数据 - 数据处理&分析|
|valid-anagrams.cpp|比较数据是否具有共性 - 数据分析|
|[Dir] Heap | <b>目录：堆题目</b> |
|top-k-frequent-elements.cpp|一维数组转二维Map结构 - 数据处理|
|[Dir] Tree | <b>目录：树题目</b> |

### **Dir: Web** ###
|File|Comment|
|:---|:---|
|[Dir] Auth | <b>Some web auth question.</b> |