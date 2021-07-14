## **编译过程中的"undefined reference to" 链接问题**

### 1. 链接时缺失了相关目标文件(*.o文件).
### 2. 链接时缺少相关的库文件.
### 3. 链接的库文件使用了另一个没参与编译的库文件.
### 4. 多个库文件链接顺序问题.
### 5. 定义与实现不一样.
### 6. 在C++代码中链接c语言的库，没有使用__cplusplus宏.
### 7. 目标文件中没有相应定义实现
- 验证方法：对目标文件或者库执行 nm libxxx | grep funxxx  
    1. 正常情况下一般会输出： 
    ```sh
    # U  funsss
    # T  funsss
    # 
    # U: 表示声明 
    # T：表示实现 [代码段]
    ```

## **Linux 程序搜索动态库的方法**
### A. ELF可执行文件中动态段中DT_RPATH所指定的路径
- 在编译目标代码时, 对gcc加入链接参数“-Wl,-rpath”指定动态库搜索路径
- eg：gcc -Wl,-rpath,/home/arc/test,-rpath,/lib/,-rpath,/usr/lib/,-rpath,/usr/local/lib test.c

### B. 环境变量LD_LIBRARY_PATH 指定的动态库搜索路径

### C. /etc/ld.so.cache中所缓存的动态库路径
- 这个可以通过先修改配置文件/etc/ld.so.conf中指定的动态库搜索路径，然后执行ldconfig命令来改变。
- eg：由于优先级B>C, 有时修改此路径也不一定有效

### D. 默认的动态库搜索路径：/lib
### E. 默认的动态库搜索路径：/usr/lib

