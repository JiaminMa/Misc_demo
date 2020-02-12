# API Note

```
#include <pthread.h>
```
### int pthread\_mutex\_lock(pthread\_mutex\_t *mptr);
### int pthread\_mutex\_trylock(pthread\_mutext\_t *mptr);
### int pthread\_mutex\_unlock(pthread\_mutext\_t *mptr);
### int pthread\_cond\_wait(pthread\_cond\_t *cptr, pthread\_mute\_t *mptr);
### int pthread\_cond\_signal(pthread\_cond\_t *cptr);
### int pthread\_cond\_broadcast(pthread\_cond\_t *cptr);
### int pthread\_cond\_timedwait(pthread\_cond\_t *cptr, pthread\_mutex\_t *mptr,
                                const sturct timesepc *abstime);
### int pthread\_mutex\_init(pthread\_mutex\_t *mptr, const pthread\_mutexattr\_t (attr);
### int pthread\_mutex\_destory(pthread\_mutex\_t *mptr);

### int pthread\_cond\_init(pthread\_cond\_t *mptr, const pthread\_condattr\_t (attr);
### int pthread\_cond\_destory(pthread\_cond\_t *mptr);

### 持有锁期间进程中止
没有办法让系统在进程中止时自动释放持有锁，如果是被另一个现成取消的情况，name该现成可以安装将在被取消时调用的清理函数。
