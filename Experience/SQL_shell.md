## **Shell 删除录像文件同时链接数据库珊删除记录操作**

```sh
#!/bin/sh
file_num=2
filepath=/resource/records
getpercent(){
	percent=`df /resource -h | grep -v Filesystem| awk 'NR==2{print int($5)}'`
}

getpercent
#循环判断分区使用率是否超过70%
while [ $percent -ge 70 ]
do
      total_file_num=`ls $filepath -l | grep -e ".mkv" -e ".mp4" | wc -l` # 统计目录文件的个数
      if [ $file_num -ge $total_file_num ];then
            echo "您要删除的文件数目太多!"
            break;
      else
         cd $filepath && ls $filepath -ltr | grep -v 'total' | grep -e ".mkv" -e ".mp4" | awk '{print $9}' | head -n $file_num |\
                 xargs -I %% mysql -uroot lb_record -s -e "update conference set is_del=1 where id=(select c_id from conference_videos where filename='%%');"
      	 cd $filepath && ls $filepath -ltr | grep -v 'total' | grep -e ".mkv" -e ".mp4" | awk '{print $9}' | head -n $file_num | xargs rm -rf
      fi
      getpercent
done

#清理 buff/cache 1

echo "开始清除缓存"
sync;sync;sync #写入硬盘，防止数据丢失
sleep 10 #延迟10秒
echo 3 > /proc/sys/vm/drop_caches

#清理日志
#find /usr/local/ -mtime +7 -name "*.log" -exec rm -rf {} \;
```


