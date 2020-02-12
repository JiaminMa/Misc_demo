#API note
```
#include <mqueue.h>
```

### mqd\_t mq\_open(const char *name, int flag, ... /*mode\_t mode, struct mq\_attr *attr*/)
### int mq\_close(mqd\_t mqdes);
### int mq\_unlink(const char *name);
### int mq\_getattr(mqd\_t mqdes, struct mq\_attr *attr);
### int mq\_setattr(mqd\_t mqdes, const struct mq\_attr *attr, struct mq\_attr *attr);
```
struct mq_attr {
    long mq_flags;
    long mq_maxmsg;
    long mq_msgsize;
    long mq_curmsgs;
}
```

### int mq\_send(mqd\_t mqdes, const char *ptr, size\_t len, unsigned int prio);
### ssize\_t mq\_receive(mqd\_t mqdes, char *ptr, size\_t len, unsigned int *priop);
### int mq\_notirfy(mqd\_t mqdes, const struct sigevent *notification);

```
union sigval {
    int sigval_int;
    void *sigval_ptr;
}

struct sigevent {
    int sigev_notify;
    int sigev_signo;
    union sigval sigev_value;
    void (*sigev_notify_function)(union sigval);
    pthread_attr_t *sigev_notify_attributes;
}
```

### POSIX实时信号
- 信号是排队的
- 信号值较小的信号优先于较大的信号递交
- 当某个非实时信号传递时， 传递给它的信号处理程序的唯一参数改信号的值
