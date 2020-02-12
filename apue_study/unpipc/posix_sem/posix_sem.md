#API note
```
#include <semaphore.h>
```

### sem\_t *sem\_open(const char *name, int flag, ... /*mode_t mode, unsigned int value*/);
### int sem\_close(sem\_t *sem);
### int sem\_unlink(sem\_t *sem);
### int sem\_wait(sem\_t *sem);
### int sem\_trywait(sem\_t *sem);
### int sem\_post(sem\_t *sem);
### int sem\_getvalue(sem\_t *sem, int *valp);
