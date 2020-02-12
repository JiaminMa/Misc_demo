# Chapter 3 File IO Note

```
#include <fcntl.h>
#include <unistd.h>
```
### int open(const char *path, int flag, ... /*mode\_t mode*/);
### int openat(int fd, const char *relative_path, int flag, ... /*mode\_t mode*/);
https://blog.csdn.net/wang1902568721/article/details/47796173

### int creat(const char *path, mode_t mode);
### int close(int fd);
### off\_t lssek(int fd, off\_t offset, int whence);
### size\_t read(int fd, void *buf, size\_t nbytes);
### size\_t write(int fd, const void *buf, size_t nbytes);
Buffer size = 4096 is best

### int dup(int fd);
### int dup2(int fd, int fd2);
This will close fd2, then return a new file descriptor

### int fsync(int fd);
### int fdatasync(int fd);
Only sync data, no file character.
### int sync(void);

### int fcntl(int fd, int cmd, ... /*int arg*/)
```
(1) Copy one exist file descriptor: F_DUPFD, F_CLOEXEC
(2) Get file descriptor flag: F_GETFD, F_SETFD
(3) Get file state flag: F_GETFL, F_SETFL
    O_RDONLY,
    O_WRONLY,
    O_RDWR,
    O_EXEC,
    O_SEARCH,
    O_APPEND,
    O_NONBLOCK,
    O_SYNC,
    O_DSYNC,
    O_RSYNC,
    O_FSYNC,
    O_ASYNC,
(4) Get async IO permission: F_GETOWN, F_SETOWN
(5) Get record lock: F_GETLK, F_SETLK, F_SETLKW
```

### int ioctl(int fd, int request, ...);
