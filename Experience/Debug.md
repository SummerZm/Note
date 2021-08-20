## **工作中的调试问题**

### **编译相关问题**
- **在C++代码类中添新成员变量，编译成功，程序运行直接在其他线程崩溃**
```sh
# 原因：Make的时候，工程文件只对修改的文件对应的.o文件进行重新编译；其他.o文件用的仍是就的结构类型
# 解决：make clean && make 
```

### **锁问题**
- 在函数调用里递归使用互斥锁会导致线程锁死

### **stack smashing detected**
- **原因：堆栈溢出**
    ```C
    // ex1: ...
    AES_KEY* aes; /* error*/
    AES_set_encrypt_key(key, 128, &aes);  // write something to aes-pointer without alloc memory. 
    HTTP_AES_cfb1_crypt_file(infile, outfile, 0, &aes);
    return 0   /* stack smashing*/

    // ex2: ...
    AES_KEY aes;    /* OK */
    AES_set_encrypt_key(key, 128, &aes);
    HTTP_AES_cfb1_crypt_file(infile, outfile, 0, &aes);
    return 0        /* stack smashing*/

    ```
