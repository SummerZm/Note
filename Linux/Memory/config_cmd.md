## <b>Linux内存配置和性能指标</b> ##
> <b>使用缓存和不使用缓存性能相差近百倍</b>

### <b>1. Linux内存配置</b> ###
- /proc/pid/oom_adj 
    1. 设置某一进程的OOM阈值  OOM（Out of memory）
    2. 取值：[-17, 15] （17:禁用OOM）
    3. 目的：避免某一进程用光内存

- /proc/meminfo
    1. Buffer: 具体的文件系统与物理硬盘数据交互使用的内存【直接读写磁盘这个值会变大】
    2. Cached: VFS系统缓存物理硬盘相关数据使用的内存【直接读写文件这个值会变大】
    3. SReclaimable: Slab伙伴内存管理系统可回收部分

- /proc/sys/vm/min_free_kbytes
    1. 设置最小内存阀值（水位），其他阀值由最小阀值通过计算自动生成
    2. pages_low = pages_min*5/4
    3. pages_high = pages_min*3/2 

- /proc/sys/vm/zone_reclaim_mode
    1. CPU处于NUMA架构时，设置回收内存的方式
    2. 0从其他Node找内存，1回收本地内存， 2回写脏页获取内存， 4交换swap获取内存
    3. /proc/zoneinfo：CPU处于NUMA架构时，则有多组信息
    4. 内存剩余很多但是总出现swap,很可能CPU是NUMA架构导致

- /proc/sys/vm/swappiness
    1. swap 内存换出的积极度 [0-100]
    2. 等于0：非常不积极但还是会换出

- 配置swap区
    1. 分区方式
    2. 文件方式
        ```sh
        # 创建Swap文件
        $ fallocate -l 8G /mnt/swapfile
        # 修改权限只有根用户可以访问
        $ chmod 600 /mnt/swapfile
        # 配置Swap文件
        $ mkswap /mnt/swapfile
        # 开启Swap
        $ swapon 
        ```

### <b>2. Linux性能指标</b> ###
- free命令[系统级]
    1. used：已使用内存
    2. free: 未使用内存
    3. share: 共享内存
    4. buff/cache: 缓存和缓冲区大小
    5. available: 未使用内存和可回收缓存

- top命令 [进程级]
    1. VIRT：进程虚拟进程大小。
    2. RES: 常驻内存大小。即：物理内存大小
    3. SHR: 共享内存大小。（共享内存，动态库，程序代码段）
    4. MEM：进程使用的物理内存占用系统总内存的百分比。

- 清理缓存
    1. echo 3 > /proc/sys/vm/drop_caches
    2. 清理文件页、目录项、Inodes 等各种缓存

- dd命令
    - 向磁盘读写数据：
        1. dd if=/dev/sda1 of=/dev/null bs=1M count=1024
        2. dd if=/dev/urandom of=/dev/sdb1 bs=1M count=2048

    - 向文件读写数据：
        1. dd if=/dev/urandom of=/tmp/file bs=1M count=500
        2. dd if=/tmp/file of=/dev/null bs=1M count=500