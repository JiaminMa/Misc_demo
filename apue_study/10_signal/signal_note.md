# Chapter10 Signal Note

```
#include <signal.h>
```

### void (*signal(int signo, void (*func)(int)))(int);
- 程序启动，当执行一个程序时，所有信号的状态都是系统默认或忽略
- 进程创建，当一个进程调用fork时，其子进程继承父进程的信号处理方式

### 信号可重入函数
图10-4 P272 <br/>
不可重入原因:
- 使用静态数据结构
- 调用malloc或free
- 标准的I/O函数

### int kill(pid\_t pid, int signo);
|-|-|
|pid > 0|将该信号发送给进程ID为pid的进程|
|pid == 0|将该信号发送给与发送进程属于同一进程组的所有进程|
|pid < 0|将该信号发送给其他进程组ID等于pid绝对值|
|pid == -1|发送给所有有权限接受信号的进程|

### int raise(int signo);

### unsigned int alarm(unsigned int seconds);
### int pause(void);

### int sigemptyset(sigset\_t *set);
### int sigfillset(sigset\_t *set);
### int sigaddset(sigset\_t *set, int signo);
### int sigdelset(sigset\_t *set, int signo);
### int sigismember(const sigset\_t *set, int signo);

### int sigprocmask(int how, const sigset\_t *restrict set, sigset\_t *restrict oset);

### int sigpending(sigset\_t *set);
### int sigaction(int signo, const struct sigaction *restrict act, struct sigaction *restrict oact);

### int sigsuspend(const sigset\_t *sigmask);
暂时没理解

### void abort(void);

### unsigned int sleep(unsigned int seconds);

### int sigqueue(pid\_t pid, int signo, const union sigval value);
暂时没理解
