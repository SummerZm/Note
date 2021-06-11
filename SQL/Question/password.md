## **Mysql 远程登录密码修改**
- **给予任何主机访问mysql的权限**
    GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY 'your password' WITH GRANT OPTION;

- **使权限修改生效**
    FLUSH PRIVILEGES;