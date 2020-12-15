## NAGLE算法简介
- 查看网络连接状态
    1. netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'
        ```sh
        # /^tcp/： 匹配以tcp开头的行，对这些行进行处理.
        # {++S[$NF]}： 以最后一列字段为key,对匹配中的行逐行统计累加处理，形成数组表.
        # NF: 表示当前处理的行有多少列.
        # $NF: 表示最后一列字段.
        # END {for(a in S) print a, S[a]}： END awk代码块标识,打印数组.

        # 例子：
        netstat -na | awk '/^tcp/ {++D[$NF]} END{for(a in D) print a, D[a]}'
        LISTEN 19
        ESTABLISHED 6
        ```

- 2. TCP socket 分多次发送较少的数据时，对端可能会很长时间收不到数据，导致本端应用程序认为超时报错。
    ```c
        // 1. Nagle算法通过积累小包到一定门阀后才发送，进而减少需要传输的数据包，来提高网络带宽利用率。
        // 2. 实时性高的应用要关闭Nagle算法
        // 3. Nagle算法+延迟确认机制会导致大概40ms的延时 [未验证]
        // 4. TCP_NODELAY和TCP_CORK都是禁用Nagle算法，NODELAY完全关闭， 而TCP_CORK完全由自己决定
        // 5. include <netinet/tcp.h> for linux
        int on = 1;
        #ifdef WIN32
            setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const char *)&on, sizeof(on));
            setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(on));
        #else
            setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void *)&on, sizeof(on));
            setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, (void *)&on, sizeof(on));
        #endif
    ```

