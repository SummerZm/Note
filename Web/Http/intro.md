## **HTTP 简介**
> [GET和POST的区别](!https://www.zhihu.com/question/28586791)

### **A. 浏览器的GET和POST**
- GET:  浏览器中定义是获取资源  
    1. 一般只有消息头。
    2. **不能用于修改后台资源**
    3. **幂等，可以反复多次调用**

- POST: 浏览器中用于提交表单
    1. 表单提交在浏览器中有两种格式  
        ```sh
        # A. application/x-www-form-urlencoded用来传输简单的数据，"key1=value1&key2=value2"
        # B. multipart/form-data格式传文件
        ```
    2. 一般消息头，消息体两部分组成
    3. **非幂等，不能重复提交**
        ```sh
        # 因为前端可能实现“下单按键”有bug，造成用户一次点击发出N个请求。你不能说因为POST by design应该是不幂等就不管了。
        ```

### **B. 接口中的GET和POST**
> 指通过Ajax api, curl，postman之类的工具发出来的GET和POST请求

- GET/POST能用在后端各个子服务的调用中（即当一种RPC协议使用）
- REST充分运用GET、POST、PUT和DELETE，约定了这4个接口分别获取、创建、替换和删除“资源”，REST最佳实践还推荐在请求体使用json格式。
    ```sh
    # json相对于x-www-form-urlencoded的优势在于：
    # A. 可以有嵌套结构
    # B. 可以支持更丰富的数据类型
    ```
- GET请求可以实现缓存
- **接口中的POST请求的参数既可以在url中也可以在body中或者两者都有，没有浏览器那么多限制**
- REST中POST 和 PUT区别
    ```sh
    #
    # PUT的实际语义是“replace”，PUT的请求体应该是完整的资源，包括id在内。
    # 服务器应该先根据请求提供的id进行查找，如果存在一个对应id的元素，就用请求中的数据整体替换已经存在的资源；
    # 如果没有，就用“把这个id对应的资源从【空】替换为【请求数据】
    # 
    # 【使用PUT还是POST创建资源，完全要看是不是提前可以知道资源所有的数据（尤其是id），以及是不是完整替换】
    # 客户端能否知道服务器上资源的Id
    # 
    # 例子：
    # 对于AWS S3这样的对象存储服务，当想上传一个新资源时，其id就是“ObjectName”可以提前知道；
    # 同时这个api也总是完整的replace整个资源。这时的api用PUT的语义更合适；而对于那些id是服务器端自动生成的场景，POST更合适一些。
    #
    # 现实中总是有REST的变体，也可能用非REST的协议（比如JSON-RPC、SOAP等），每种情况中的GET和POST又会有所不同。
    #
    ```

### **C. 关于GET和POST安全性**
- 请求过程会经过大量的第三方中间结点(括网关，代理等),这些结点的access log会记录报文信息。解决办法：https
- 内部可信网络避免使用http等明文代理，没加密数据流经主网关内部运维人员可以看到内容
- 安全是由很多细节组成的一个完备体系，比如返回私密数据的mask，XSS，CSRF，跨域安全，前端加密，钓鱼，salt， POST/GET在安全这件事上仅仅是个小角色
- 因此单独讨论POST和GET本身哪个更安全意义并不是太大

### **D. 关于编码**
> [浏览器URL编码](!http://www.ruanyifeng.com/blog/2010/02/url_encoding.html)
- GET和POST在http中url用什么编码，body用什么编码
- 常见错误说法： GET的参数只能支持ASCII，而POST能支持任意binary，包括中文。
    ```sh
    # url只能支持ASCII的说法源自于RFC1738:
    #    "Thus, only alphanumerics, the special characters "$-_.+!*'(),", and   reserved characters used for their reserved purposes 
    #        may be used unencoded within a URL."
    #
    # 实际上这里规定的仅仅是一个ASCII的子集[a-zA-Z0-9$-_.+!*'(),]。它们是可以“不经编码”在url中使用,
    # 比如尽管空格也是ASCII字符，但是不能直接用在url里。
    ```
- 那这个“编码”是什么呢？如果有了特殊符号和中文怎么办呢？ [percent encoding编码](https://en.wikipedia.org/wiki/Percent-encoding​en.wikipedia.org)
    1. percent encoding编码：url中的一坨%和16位数字组成的序列。
    2. percent encoding编码：只把字符转换成URL可用字符，但是却不管字符集编码（比如中文到底是用UTF8还是GBK）
        ```sh
        # 浏览器的地址栏是不受开发者控制,同样一个带中文的url
        # 有的浏览器一定要用GBK（比如老的IE8）
        # 有的一定要用UTF8（比如chrome）。后端就可能认不出来。
        # 解决方案：Ajax发出的编码形式开发者是可以100%控制的
        ```
    3. 在浏览器地址栏可以看到中文。但这种url在发送请求过程中，浏览器会把中文用字符编码+Percent Encode翻译为真正的url，再发给服务器。浏览器地址栏里的中文只是想让用户体验好些
    4. HTTP Body相对好些，因为有个Content-Type来比较明确的定义

-  GET+url的情况，只要不涉及到在老旧浏览器的地址栏输入url，也不会有什么太大的问题。
-  POST情况
    ```sh
    # 浏览器发出的POST请求:  application/x-www-form-urlencoded /  multipart/form-data 或者是两者混合。
    # Ajax或者其他HTTP Client发出去的POST请求： body格式就非常自由了，常用的有json，xml，文本，csv……甚至是你自己发明的格式。
    ```
   
### **E. 浏览器的POST需要发两个请求吗？**
- HTTP请求可以被大致分为“请求头”和“请求体”两个部分
- 使用HTTP时大家会有一个约定，即所有的“控制类”信息应该放在请求头中，具体的数据放在请求体里
    ```sh
    # 服务器端接收到请求后，就可以先拿到请求头部，查看用户是不是有权限上传，文件名是不是符合规范等
    # 如果不符合，就不再处理请求体的数据了，直接丢弃。而不用等到整个请求都处理完了再拒绝。
    # 如果符合，且需要转发，则可以进行zero copy转发【效率高】
    # 代价就是会多一次Round Trip
    ```
- 请求体的数据不多，那么一次性全部发给服务器可能反而更好
- 解决方案：
    ```sh
    # 客户端就能做一些优化，比如内部设定一次POST的数据超过1KB就先只发“请求头”，否则就一次性全发。
    # 因为不管怎么发都是符合HTTP协议的，因此我们应该视为这种优化是一种实现细节
    ```

### **F. 关于URL的长度**
- HTTP协议本身对URL长度并没有做任何规定。实际的限制是由客户端/浏览器以及服务器端决定的。
- IE8浏览器：“URL的最大长度是2083个字符，path的部分最长是2048个字符”。

### **G. 其他**
- 在HTTP协议的RPC说明中，规范下的区别主要在于语义下的区别：GET用于获取资源，POST用于处理资源
- 代理一般不会缓存 post 请求的响应内容




