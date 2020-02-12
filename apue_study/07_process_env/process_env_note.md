#第七章 进程环境 笔记

### 进程终止
- 从main返回
- 调用exit
- 调用_exit或者_Exit
- 最后一个线程从其启动程序返回
- 从最后一个线程调用pthread_exit
- 调用abort
- 接到一个信号
- 最后一个线程对齐取消请求做出响应

### int atexit(void *(func)(void));
注册进程退出函数

### 进程存储空间
```
-------------------
命令行参数和环境变量
-------------------
 	栈
 	.
 	.
 	.
 	堆
--------------------
bss
--------------------
data
--------------------
text
--------------------
```

### int setjmp(jmp_buf env);
### void longjmp(jmp_buf env, int val);
```
#include <setjmp.h>
```
全局变量，静态变量和volatile变量不受优化影响，如果要编写一个使用非巨变跳转的可移植程序， 必须使用volatile属性。

### int getrlimit(int resource, struct rlimit *rlptr);
### int setrlimit(int resource, const struct rlimit *rlptr);
```
#include <sys/resource.h>
```
