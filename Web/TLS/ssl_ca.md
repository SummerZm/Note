## **通信中的SSL 与 CA**
> [参考链接](http://blog.csdn.net/tiandyoin/article/details/37880457)  [OpenSSL命令详解](https://blog.csdn.net/scuyxi/article/details/54884976)

### **一、CA流程简介**
- **A. 客户端发起SSL请求带有一个随机数据给服务器**
- **B. 服务器将带有自己公钥的证书【CA颁发】和一个随机数发给浏览器；如果开启双向认证会在此处发起客户端认证**
- **C. 浏览器使用CA的根证书验证服务器证书，获得证书中的公钥，并使用该公钥加密一个随机数发给服务器**
- **D. 服务器利用前面三个随机数生成本次会话秘钥**
- **E. 双方相互发送前面所有的报文HASH校验**
- **F. 证书文件术语**
    ```sh
    # .key: 私有的密钥
    # .csr: 证书签名请求（证书请求文件），含有公钥信息，certificate signing request的缩写
    # .crt: 证书文件，certificate的缩写
    # .crl: 证书吊销列表，Certificate Revocation List的缩写
    # .pem: 用于导出，导入证书时候的证书的格式，有证书开头，结尾的格式
    ```

### **二、公钥私钥的核心功能**
- 加密: 公钥用于对数据进行加密，私钥用于对数据进行解密
- 签名: 私钥用于对数据进行签名，公钥用于对签名进行验证
- **为什么用公钥加密，私钥签名？**
    ```sh
    # 通用场景【不要钻牛角尖】： 
    # 用私钥签名是由于私钥只有一把，具有唯一性所以可以用来验证持有者。
    # 用公钥加密是由于公钥分发给大家，大家可以通过公钥和私钥持有者说悄悄话。
    ```

- **如何生成公密钥**
    ```sh
    # genrsa       产生RSA密钥命令。
    # -out         输出路径,这里指private/server.key.pem。
    # 2048         指RSA密钥长度位数，默认长度为512位。
    # 生成私钥: 
    openssl genrsa -out private.key 2048
    # 到出公钥: 
    openssl rsa -in private.key -pubout -out public.key 
    ```

### **二、如何自建CA？**
- **A. 创建根文件秘钥 root.key** 
    ```sh
   # 可以设置密码.
    # 如果出现以下问题: unable to write 'random state'; 请检查openssl.cnf中'RANDFILE'所指向目录文件的权限或所在文件系统是否可读.
    openssl genrsa -out root.key 2048
    # 或者需要输入密码 [lonbon]
    openssl genrsa -aes256 -out root.key 2048
    ```
- **B. 创建证书申请文件 root.csr**
    ```sh
    # 需要使用A步骤中的root.key, 需要填入国家，公司，等相关信息.
    openssl req -new -key root.key -out root.csr
    ```
- **C. 创建根证书 root.crt**
    ```sh
    # 需要输入A中的密码.
    # 这个根证书要放到浏览器的证书管理中心.
    openssl x509 -req -days 36500 -sha1 -extensions v3_ca -signkey root.key -in root.csr -out root.crt
    ```

### **三、如何颁发服务器证书**
> **证书申请者向CA机构提供信息，CA机构为申请者生成公密钥和创建证书** 

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

### **五、如何颁发SDK证书**
- **生成ｐ１２格式的证书（利用生成的CA根证书和服务证书的ｃｒｔ和ｋｅｙ文件生成P１２文件）**
    ```sh
    # 这里的password 和 xx.crt文件的password 不一样
    openssl pkcs12 -export -in xxx.com.crt -inkey xxx.com.key -passin pass:changeit -name *.xxx.com -chain -CAfile CA.crt -password pass:changeit -caname *.xxx.com -out xxx.com.p12
    ```

- **使用jdk keytool工具进行生成tomcat/jboss端使用的证书文件（window下执行）**
    ```sh
    # 查看p12证书
    keytool -rfc -list -keystore xxx.com.p12 -storetype pkcs12
    # 转换p12证书为jks证书文件
    keytool -importkeystore -srckeystore xxx.com.p12 -srcstoretype PKCS12 -deststoretype JKS -destkeystore xxx.com.jks
    # 利用jks证书生成cer证书
    keytool -export -alias *.xxx.com -keystore xxx.com.jks -storepass changeit -file xxx.com.cer
    # 利用cer证书文件生成jdk所使用的文件
    keytool -import -alias *.xxx.com -keystore cacerts -file xxx.com.cer
    # 合并证书
    keytool -import -alias *. xxx.com -file D:\java\jdk1.7.0_67\xxx.com\ xxx.com.cer -storepass changeit -keystore D:\java\jdk1.7.0_67\ xxx.com\cacerts
    ```

### **五、其他相关命令**
- **查看自签名CA证书：openssl x509 -text -in ca.cert**
- **证书格式转换：openssl x509 -in server.crt -out server.pem**

### **六、命令简介**
- **openssl ca 组合命令** 
    ```sh
    # -selfsign: 使用对证书请求进行签名的密钥对来签发证书。即"自签名"。【生成证书请求文件使用秘钥和签发证书的秘钥是同一把。场景：即是CA，也是证书申请者】
    # -keyfile arg: CA的私钥证书文件。【非自签】
    # -in file: 需要进行处理的PEM格式的证书。
    # -out file: 处理结束后输出的证书文件。
    # -cert file: 用于签发的根CA证书。
    # -days arg: 指定签发的证书的有效时间。
    # -keyform arg：CA的根私钥证书文件格式 [PEM|ENGINE]。
    # -key arg: CA的根私钥证书文件的解密密码(如果加密了的话)
    # -config file  
    ```

- **openssl req 组合命令【用于生成证书申请文件 or CA机构证书】**
    ```sh
    # -req         ...
    # -new            表示新请求。
    # -key            用于签名待生成的请求证书的密钥,这里为client.key文件
    # -keyform arg    [DER|NET|PEM]
    # -out            输出路径,这里为client.csr文件
    # -subj           指定用户信息
    # -days           表示有效天数,这里为3650天。
    # -sha1           表示证书摘要算法,这里为SHA1算法。
    # -extensions     表示按OpenSSL配置文件v3_req项添加扩展
    # -in             表示输入文件,这里为private/server.csr
    # -inform arg     输入文件格式 [DER|PEM]
    # -out            表示输出文件,这里为certs/server.cer 
    # -outform arg    输出文件格式 [DER|PEM]
    # -passin         用于签名待生成的请求证书的私钥文件的解密密码   
    # -x509           输出一个X509格式的证书 
    # -[digest]         HASH算法 [md5|sha1|md2|mdc2|md4|sha256|...]
    # -newkey rsa:bits  生成一个bits长度的RSA私钥文件，用于签发
    # -text             text显示格式
 
    # ============================================================================ 
    # A. 利用CA的RSA密钥创建一个自签署的CA证书(X.509结构) 
    # openssl req -new -x509 -days 3650 -key ca.key -out ca.crt
    
    # B. 用server.key生成证书签署请求CSR(这个CSR用于发送给CA中心等待签发)
    # openssl req -new -key server.key -out server.csr
    
    # C. 查看CSR的细节
    # openssl req -noout -text -in server.csr
    #=============================================================================
    ```

- **openssl genrsa [args] [numbits]【生成RSA参数】**
    ```sh
    # A. 对生成的私钥文件是否要使用加密算法进行对称加密
    #    -des: CBC模式的DES加密
    #    -des3: CBC模式的3DES加密
    #    -aes128: CBC模式的AES128加密 
    #    -aes192: CBC模式的AES192加密
    #    -aes256: CBC模式的AES256加密

    # B. -passout arg: arg为对称加密(des、3des、aes)的密码(使用这个参数就省去了console交互提示输入密码的环节)
    # C. -out file: 输出证书私钥文件
    # D. [numbits]: 密钥长度

    # ============================================================================
    # 生成一个1024位的RSA私钥，并用3DES加密(密码为123456)，保存为server.key文件
    # openssl genrsa -out server.key -passout pass:123456 -des3 1024
    # ============================================================================ 
    ```

- **openssl rsa【RSA数据管理】**
    ```sh
    # -inform arg   输入密钥文件格式 [DER(ASN1)|NET|PEM(base64编码格式)]
    # -outform arg  输出密钥文件格式 [DER(ASN1)|NET|PEM(base64编码格式)]
    # -in arg       待处理密钥文件 
    # -passin arg   输入这个加密密钥文件的解密密钥(如果在生成这个密钥文件的时候，选择了加密算法了的话)
    # -out arg      待输出密钥文件
    # -passout arg  如果希望输出的密钥文件继续使用加密算法的话则指定密码 
    # -des:    CBC模式的DES加密
    # -des3:   CBC模式的3DES加密
    # -aes128: CBC模式的AES128加密
    # -aes192: CBC模式的AES192加密
    # -aes256: CBC模式的AES256加密
    # -text:   以text形式打印密钥key数据 
    # -noout:  不打印密钥key数据 
    # -pubin:  检查待处理文件是否为公钥文件
    # -pubout: 输出公钥文件

    # ============================================================================
    #  对私钥文件进行解密
    #  openssl rsa -in server.key -passin pass:123456 -out server_nopass.key
    #  利用私钥文件生成对应的公钥文件
    #  openssl rsa -in server.key [-passin pass:123456] -pubout -out server_public.key
    # ============================================================================ 
    ```
- **openssl x509【证书处理工具-显示证书的内容，转换其格式，给CSR签名等】**
    ```sh
    # -inform arg   待处理X509证书文件格式 [DER|NET|PEM]
    # -outform arg  待输出X509证书文件格式 [DER|NET|PEM]
    # -in arg       待处理X509证书文件
    # -out arg      待输出X509证书文件
    # -req          表明输入文件是一个"请求签发证书文件(CSR)"，等待进行签发 
    # -days arg     表明将要签发的证书的有效时间 
    # -CA arg       指定用于签发请求证书的根CA证书 
    # -CAform arg   根CA证书格式(默认是PEM) 
    # -CAkey arg    指定用于签发请求证书的CA私钥证书文件，如果这个option没有参数输入，那么缺省认为私有密钥在CA证书文件里有
    # -CAkeyform arg  指定根CA私钥证书文件格式(默认为PEM格式)
    # -CAserial arg   指定序列号文件(serial number file)
    # -CAcreateserial 如果序列号文件(serial number file)没有指定，则自动创建它     

    # ============================================================================
    #  转换DER证书为PEM格式
    #  openssl x509 -in cert.cer -inform DER -outform PEM -out cert.pem

    #  使用根CA证书对"请求签发证书"进行签发，生成x509格式证书
    #  openssl x509 -req -days 3650 -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out serverx509.crt

    #  打印出证书的内容
    #  openssl x509 -in server.crt -noout -text
    
    #  证书验签
    #  openssl verify -CAfile demoCA/cacert.pem usercert.cr
    # ============================================================================ 
    ```

### **七、注意事项**
- **A. 根证书的Common Name填写root**
- **B. 根证书的这个字段和客户端证书、服务器端证书不能一样**

### **八\ 常见问题处理**
#### **如何解决通用名称不匹配的问题**
- 1. 站点地址未包含在证书的通用名称中
    ```sh
    # 最常见的是，当为域名（例如www.example.com）购买SSL证书时，
    # 认证机构还包括不带前导www前缀的域（成功验证后）。
    # 证书发行后可以添加所需的域名（对于多域证书）。
    # 为此，您将需要使用新的SAN名称重新颁发证书。
    ```

- 2. 证书未安装或未正确安装在服务器上
- 3. 网站未安装SSL证书，但是与具有正确安装的SSL证书的另一个站点具有相同的IP地址
    ```sh
    # a. 某些托管服务可能需要静态IP地址才能进行SSL安装。如果将SSL证书安装在具有动态IP地址的网站上，则可能会干扰其他网站的操作。
    # b. 在某些情况下，客户端连接或托管服务器不支持SNI（服务器名称指示）。但是，几乎每个现代托管服务提供商都支持SNI。
    # c. 可以通过获取静态IP地址（并更改DNS设置）来解决上述问题。
    ```

- 4. 您正在尝试访问IP地址。
    ```sh
    # a. 如果有人尝试为IP地址建立HTTPS连接，则会发生错误。对IP地址的HTTPS请求不包含服务器名称，这将导致类似的错误。
    ```

- 5. 托管参数将覆盖证书设置
    ```sh
    # 您的托管服务提供商可能会应用将SSL绑定到每个域的设置。如果您从第三方提供商处购买SSL / TLS证书并进行安装，则会看到“通用名称不匹配”错误。
    # 转到SSL证书检查器，并概述有关证书的完整信息。如果您发现公用名CN或SAN包含您的托管服务提供商的名称，则很可能是引起问题的根源。
    # 您需要联系托管服务提供商，并要求他们删除SSL证书。如果他不这样做，那么在这种情况下，您将需要寻找其他提供者。您的网络托管服务提供商不应限制您安装SSL证书的能力。
    ```

- 6. 域名与另一个已安装证书使用的旧IP地址关联。
    ```sh
    # 如果域名与未更改的旧IP地址关联，并且其他证书引用了相同的IP地址，则可能会遇到“通用名称不匹配”错误。您可以通过更改DNS记录来解决此问题。
    ```

- 7. DNS记录最近已更改。
    ```sh
    # 如果与关联的Web域的IP地址最近已更改，则可能会发生错误。如果您购买了静态IP地址并安装了SSL证书，则该域可能仍会连接到以前使用的IP地址。
    # 解决此问题的正确方法是，等到DNS记录被更新。错误将消失。
    ```


















































