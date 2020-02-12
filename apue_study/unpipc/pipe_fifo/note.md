# API Note
### int pipe(int fd[2])
```
#include <unistd.h>
read, write, close
```
### FILE *popen(const char * command, const char *type);
type:r, w
### int pclose(FILE *stream)

### int mkfifo(const char *pathname, mode_t mode);
```
#include <sys/types.h>
#include <sys/stat.h>
open, read, write, close, unlink
```
