### A. 交叉编译openssl时生成的 libssl.a 没有使用 -fpic 参数；当我们静态链接libssl.a时,用到-fpic会报错。需要soi使用fpic重新编译libssl.a
### B. WEB服务器大量ClOSE_WAIT 后台服务挂死问题:
- a. 服务器端 netstat 发现接受队列有1个字节没被上层应用读走，一直ClOSE_WAIT
- b. 抓包发现，谷歌浏览器TCP 链接发送了心跳包
- c. openssl.0.2版本不会出现这问题，openssl.1.1.1出现此问题
- d. openssl1.1.1k + 谷歌浏览器会出现这个问题，搜狗不会出现问题
- e. 猜测：openssl1.1.1k + 谷歌浏览器每个请求都会创建一个连接。发送了keepalive的单字节包后，立刻关闭连接. 导致服务器大量的CLOSE_WAIT











































