# 第八章 进程控制 笔记

```
include <unistd.h>
```

## 进程标识
### pid_t getpid();
### pid_t getppid();
### uid_t getuid();
### uid_t geteuid();
### gid_t getgid();
### gid_t getegid();

## 创建新进程
### pid_t fork();
### pid_t vfork();	
可移植的应用程序不应该使用这个函数。因为
- 它在exec或者exit之前在父进程的地址空间运行。
- 父进程必定比子进程晚运行。

## 进程等待
### pid_t wait(int *staloc);
### pid_t waitpid(pid_t pid, int *staloc, int options);

|宏|说明|
|-|-|
|WIFEXITED(status)|正常返回|
|WIFSIGNALED(status)|异常终止|
|WIFSTOPEED(status)|暂停子进程的返回状态|
|WIFCONTINUED(status)|继续子进程的返回状态|

### int wait_tid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
|||
|-|-|
|P_PID|等待进程|
|P_PGID|等待进程组中的任一子进程|
|P_AIL|等待任一子进程|
### pid_t wait3(int *staloc, int options, struct rusage *rusage);
### pid_t wait4(pid_t pid, int *staloc, int options, struct rusage *rusage);

### int execl(const char *pathname, const char *arg0, ...);
### int execv(const char *pathname, char *const argv[]);
### int execle(const char *pathname, const char *arg0, ...);
### int execve(const char *pathname, char *const argv[], char *const envp());
### int execlp(const char *filename, const char *arg0, ...);
### int execvp(const char *filename, char *const argv[]);
### int fexecve(int fd, char *const argv[], char *const envp[]);

图8-14

## 更改用户id和组id
### int setuid(uid_t uid);
### int setgid(gid_t gid);
### int setreuid(uid_t ruid, uid_t euid);
### int setregid(gid_t rgid, git_t egid);

### int system(const char *cmdstring);
### char *getlogin(void);

## 进程调度
### int nice(int incr);
nice越大，调度优先级越小
### int getpriority(int which, id_t who);
### int setpriority(int which, id_t who, int value)
