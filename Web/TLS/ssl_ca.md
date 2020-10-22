## **通信中的SSL 与 CA**
> [参考链接](http://blog.csdn.net/tiandyoin/article/details/37880457)

### **一、CA流程简介**
- **A. 客户端发起SSL请求带有一个随机数据给服务器**
- **B. 服务器将带有自己公钥的证书【CA颁发】和一个随机数发给浏览器；如果开启双向认证会在此处发起客户端认证**
- **C. 浏览器使用CA的根证书验证服务器证书，获得证书中的公钥，并使用该公钥加密一个随机数发给服务器**
- **D. 服务器利用前面三个随机数生成本次会话秘钥**
- **E. 双方相互发送前面所有的报文HASH校验**

### **二、如何自建CA？**
- **A. 创建根文件秘钥 root.key** 
    ```sh
    # 可以设置密码.
    openssl genrsa -ras -out root.key
    ```
- **B. 创建证书申请文件 root.csr**
    ```sh
    # 需要使用A步骤中的root.key, 需要填入国家，公司，等相关信息.
    openssl req -new -key root.key -out root.csr
    ```
- **C. 创建根证书root.crt**
    ```sh
    # 需要输入A中的密码.
    # 这个根证书要放到浏览器的证书管理中心.
    openssl x509 -req -days 3650 -sha1 -extensions v3_ca -signkey root.key -in root.req -out root.crt
    ```

### **三、如何颁发服务器证书**
- **A. 创建服务器秘钥**
    ```sh
    openssl genrsa -out server.key 2048
    ```

- **B. 创建服务器申请证书**
    ```sh
    # 用于向CA机构申请证书
    # 需要使用A步骤中的的 server.key. 需要填入国家，公司，等相关信息.
    openssl req -new -key server.key -out server.req
    ```

- **C. 创建服务器证书**
    ```sh
    # 1. 将 server.csr 发送到CA机构；我们自建CA，通常在一台服务器上
    # 2. 自建CA机构用 root.crt 和 root.key 给  server.csr 签名 生成 server.crt
    # 3. 自建CA机构将 server.crt 发给服务器 
    openssl x509 -req -days 730 -sha1 -extensions v3_req -CA root.crt -CAkey root.key -CAcreateserial -in server.csr -out server.crt
    ```

- **D. 如何在代码中使用服务器证书和服务器私钥**
    ```c
        ...
        // certPath =  server.crt
        if (SSL_CTX_use_certificate_file(*ctx, certPath, 1) <= 0) {
            ERR_print_errors_fp(stdout);
            SS_LOG(SS_DEBUG, "SSL use certificate invalid");
            return ret;
        }
        // pathvateKeyPath = server.key
        if (SSL_CTX_use_PrivateKey_file(*ctx, pathvateKeyPath, 1) <= 0) {
            ERR_print_errors_fp(stdout);
            SS_LOG(SS_DEBUG, "SSL use privatekey invalid");
            return ret;
        }
        // 验证
        if (!SSL_CTX_check_private_key(*ctx)) {
            ERR_print_errors_fp(stdout);
            SS_LOG(SS_DEBUG, "SSL check privatekey invalid");
            return ret;
        }
        ...
    ```

### **四、如何颁发浏览器证书**
- **A. 和服务器申请证书的流程一致**
- **B. 如何将证书和浏览器秘钥安装到浏览器上**
    ```sh
    #将客户端证书文件client.crt和客户端证书密钥文件client.key合并成客户端证书安装包client.pfx：
    openssl pkcs12 -export -in client.crt -inkey client.key -out client.pfx
    ```



