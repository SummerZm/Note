## Linux 权限控制介绍 ##
-  Linux权限安全模型 DAC，全称是Discretionary Access Control，自主访问控制
    1. 将资源访问者分成三类, 分别是Owner, Group，Other
    2. 访问权限分成read、 write、 execute
    3. 访问者通常是进程有自己的uid/gid, 通过uid/gid和文件权限匹配, 来确定是否可以访问
    4. 进程理论上所拥有的权限与执行它的用户的权限相同

- MAC（Mandatory Access control），强制性访问控制
    1. 任何进程想在SELinux系统中干任何事情，都必须先在安全策略配置文件中赋予权限
    2. 凡是没有出现在安全策略配置文件中的权限，进程就没有该权限
    3. MAC  === SELinux(Security-Enhanced Linux) 由美国国家安全局(NSA)发起
    4. Linux Kernel 2.6 版本后，直接整合进入SELinux， 搭建在Linux Security Module(LSM)基础上. 
    5. SELinux不能完全适用于Android系统。为此，NSA针对Android系统，在SElinux基础上开发了SEAndroid。 
    6. Android 5.0(L)上有完整的开启SELinux，并对SELinux进行深入整合形成了SEAndroid。
        ```sh
        SEAndroid有Permissive跟Enforce两种模式
            1. Permissive 模式，只打印audit 异常LOG，不拒绝请求
            2. Enforce 模式，即打印audit 异常LOG, 也拒绝请求。
        ```
