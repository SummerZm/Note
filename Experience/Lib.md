## **动态链接问题集合**

### **一个程序在另外一台服务器上提示动态库缺失问题**
> **libc++.so cannot open shared objects file**
-  获取程序的动态链接情况 
    ```sh
    # 例：
    # 在程序能正常使用的服务器上执行: zipalign 程序 
    ldd zipalign
    #linux-vdso.so.1 (0x00007ffef9d64000)
    # libc++.so => /work/projects/pack/lbDriver/./lib64/libc++.so (0x00007f1a47da2000)
    # librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f1a47a7d000)
    # libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007f1a47863000)
    # libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f1a47646000)
    # libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f1a47342000)
    # libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f1a4712b000)
    # libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f1a46d8c000)
    # libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f1a46b88000)
    # /lib64/ld-linux-x86-64.so.2 (0x00007f1a47c85000)

    #  在程序不能正常使用的服务器上执行: zipalign 程序 
    ldd zipalign
    # linux-vdso.so.1 =>  (0x00007ffd52f05000)
    # libc++.so => not found
    # librt.so.1 => /lib64/librt.so.1 (0x00007fab2f805000)
    # libz.so.1 => /lib64/libz.so.1 (0x00007fab2f5ef000)
    # libpthread.so.0 => /lib64/libpthread.so.0 (0x00007fab2f3d3000)
    # libm.so.6 => /lib64/libm.so.6 (0x00007fab2f0d1000)
    # libgcc_s.so.1 => /lib64/libgcc_s.so.1 (0x00007fab2eebb000)
    # libc.so.6 => /lib64/libc.so.6 (0x00007fab2eaed000)
    # /lib64/ld-linux-x86-64.so.2 (0x00007fab2fa0d000)
    ```

- 获取服务器动态库支持情况
    ```sh
    # 例： 
    # 在程序能正常使用的服务器上执行
    ldconfig -p | grep lib | grep c++
    # libstdc++.so.6 (libc6,x32) => /usr/libx32/libstdc++.so.6
    # libstdc++.so.6 (libc6,x86-64) => /usr/lib/x86_64-linux-gnu/libstdc++.so.6
    # libstdc++.so.6 (libc6) => /usr/lib32/libstdc++.so.6
    # liblua5.2-c++.so.0 (libc6,x86-64) => /usr/lib/x86_64-linux-gnu/liblua5.2-c++.so.0

    # 在程序不能正常使用的服务器上执行
    ldconfig -p | grep lib | grep c++
    # libstdc++.so.6 (libc6,x86-64) => /lib64/libstdc++.so.6
    ```
- 通过对比可知缺失了库： /work/projects/pack/lbDriver/./lib64/libc++.so (0x00007f1a47da2000)

### **64位和323位库版本兼容问题**
> **/lib/ld-linux.so.2: bad ELF interpreter问题**
- 原因和解决办法
    ```sh
    # 原因：在64位的机子安装了32位的程序
    # 解决办法： yum install glibc.i686   【慎重安装】
    # 也许有多种解决办法，自行百度
    ```
   




