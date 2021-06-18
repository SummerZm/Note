## **第三方打包网站部署问题简记**

### **安装数据库**
- **调试的时候可能需要使用到远程登录，这时候会遇到一些问题**
    1. **给予任何主机访问mysql的权限**
        ```SQL
        -- ssh 登录上设备。
        GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY 'your password' WITH GRANT OPTION;
        -- 使权限修改生效。
        FLUSH PRIVILEGES;
        -- 通过修改配置文件 my.cnf，改变 mysqld 监听的IP地址。
        -- 需要修改的字段： bind-address = 服务器的实际IP地址。
        ``` 
    
- **更新php配置文件**
    1. **修改PHP上传文件大小**
        ```sh
        # A. 一个服务器会可能安装多个版本PHP，要找对正确版本的配置文件。
        # B. PHP可能会对不同服务器提供不同的配置文件
        # C. 我的环境配置文件位置: /etc/php/7.3/apache2/php.ini
        # D. 需要修改的配置项: upload_max_filesize
        # E. 可能需要修改的项：post_max_size
        ```

- **更新apache配置文件**
    1. **加载rewrite_mod: 要先修改加载配置重启，再修改该模块的使用配置，否则apachectl restart会报错**
    2. **laravel 配置虚拟站点**
        ```sh
        # 如何配置laravel前后端分离虚拟站点
        # 1. 使用 rewrite模块的 allowOverride all
        # 2. 修改 .htaccess 规则文件
        # 3. 修改 mod_rewrite.conf 的配置文件, 添加以下:
        #    目的：让apache服务器区分识别同目录下的 index.html 和 index.php
             <IfModule dir_module>
                DirectoryIndex index.php
             </IfModule>
             <IfModule mod_php7.c>
                AddType application/x-httpd-php.pho
             </IfModule>
        #  
        ```
    3. **关于服务器线程环境变量的解决办法**
        ```sh
        # 对于Ubuntu系统，找到环境变量配置文件，通常在/etc/apache2/envvars或者/usr/local/apache/bin/envvars(lamp.sh安装)中。
        # 打开envvars文件，在文件末尾export系统和用户环境变量，一般：
        #   A. 系统变量在/etc/environment中
        #   B. 所有用户的环境变量在/etc/profile中
        #   C. 单个用户的环境变量在~/.bashrc中
        export PATH=/home/XXXXXX/soft/anaconda2/bin:/usr/local/cuda-8.0/......
        export PYTHONPATH=.......
        ```

- **目录权限配置**
    1. apache 服务程序以某个系统用户运行时，会出现权限不足导致一些读写问题。

- **在PHP代码相关问题**
    1. php代码执行shell文件,环境变量问题导致无法找到要执行的命令工具：在shell文件中设置路径环境变量。
    2. 在命令行中终端执行shell脚本，能成功执行；但是在php却报出执行错误编码问题：在shell文件中设置语言环境变量。
    3. php代码执行shell文件，如果shell文件中有重定向 '>log ' 语句，其内容会在 respone 消息体中出现。



