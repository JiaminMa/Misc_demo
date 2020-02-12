# POSIX Shared memory API Note

```
#include <sys/mman.h>
```
### void *mmap(void *addr, size\_t len, int prot, int flags, int fd, off\_t offset);
### int munmap(void *addr, size\_t len);
### int msync(void *addr, size\_t len, int flags);
### BSD 匿名内存映射
### SVR4 /dev/zero/内存映射
### int shm\_open(const char *name, int oflag, mode\_t mode);
### int shm\_unlink(const char *name);


