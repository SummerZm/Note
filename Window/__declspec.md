## **Window __declspec介绍**

### **__declspec介绍**
  1. _declspec(dllexport)与_declspec(dllimport)是相互呼应，只有在DLL内部用dllexport作了声明，才能 在外部函数中用dllimport导入相关代码。
  2. 实际上，在应用程序访问DLL时，实际上就是应用程序中的导入函数与DLL文件中的导出函数进行链接。链接的方式有两种：隐式链接和显式链接。
  3. 隐式链接: 通过编译器提供给应用程序关于DLL的名称和DLL函数的链接地址，在应用程序中使用dllimport即表明使用隐式链接。不过不是所有的隐式链接都使用dllimport。
  4. 显式链接: 应用程序用语句显式地加载DLL，编译器不需要知道任何关DLL的信息。

### **__declspec相关问题**
>  不使用__declspec(dllimport) 也能正确编译代码，但使用 __declspec(dllimport) 使编译器可以生成更好的代码
  1. 编译器之所以能够生成更好的代码，是因为它可以确定函数是否存在于 DLL 中，这使得编译器可以生成跳过间接寻址级别的代码。
  2. 必须使用 __declspec(dllimport) 才能导入 DLL 中使用的静态变量变量
  3. 正确编译代码不代表能正常连接

### **__declspec代码**  
```C++
#if defined(_WIN32)
#  if defined(EXPORTING_MYMATH)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else // non windows
#  define DECLSPEC
#endif
```
