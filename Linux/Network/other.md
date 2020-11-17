##Centos 网络配置##
1. **网卡配置文件中网卡硬件地址要和ifconfig -a 显示的硬件地址一致**
2. **网卡配置文件名/文件中的设备名要和ifconfig -a显示的某一网卡名一致**
3. 重启网络命令: servive network restart
4. 查看网卡状态: service network status
5. 查看内存所有网卡: ifconfig -a
