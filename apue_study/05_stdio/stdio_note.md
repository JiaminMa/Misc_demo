# 第五章 标准I/O库 笔记

### int fwide(FILE *fp, int mode);
```
#include <stdio.h>
#include <wchar.h>

mode:负为字节定向，正为宽定向(多个字节)，不定为不指定流定向
返回值为流的定向
```

### void setbuf(File *restrict fp, char *restrict buf)
```
#include <stdio.h>
指定流的Buf，长度为BUFSIZ
```

### void setvbuf(File *restrict fp, char *restrict buf, int mode, size_t size)
```
#include <stdio.h>
指定流的Buf，长度为BUFSIZ
mode: _IOFBF全缓冲，_IOLBF行缓冲，_IONBF不带缓冲
```

## 打开/关闭流
### int fflush(FILE *fp)
将流中未写的数据传送至内核

### FILE *fopen(const char *restrict pathname, const char *restrict type);
打开pathname的文件转化为流
### FILE *freopen(const char *restrict pathname, const char *restrict type, FILE *restrict type);
打开文件到指定的流
### FILE *fopen(int fd, const char *restrict type);
从已有的文件描述符中打开一个流

O_RDONLY, O_WRONLY, O_CREATE, O_TRUNC, O_RDWR, O_APPEND

### int fclose(FILE *fp);

## Byte buffered IO
### int getc(FILE *fp);
### int fgetc(FILE *fp);
### int getchar(void); = getc(stdin)
### int putc(int c, FILE *fp);
### int fputc(int c, FILE *fp);
### int putchar(int c);

## Line Buffered IO
### char *fgets(char *restrict buf, int n, FILE *restrict fp);
### char *gets(char *buf);
Deprecated, 会引起buffer overflow

### int fputs(const char *restrict str, FILE *restrict fp);
### int puts(const char *str);
Depraceted。
Note:推荐只有fgets和fputs， 需要用户处理'\n'


## Full(Binary) Buffered IO
### size_t fread(void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
### size_t fwrite(const void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);

```
struct {
	short count;
	long total;
	char name[NAMESIZE];
} item;

if(fwrite(&item, sizeof(item), 1, fp) != 1)
```

## 定位流
### long ftell(FILE *fp);
### int fseek(FILE *fp, long offset, int whence);
### void rewind(FILE *fp);
将流设置到文件的起始位置

### off_t ftello(FILE *fp);
### int fseeko(FILE *fp, off_t offset, int whence)
带o的唯一区别是偏移量类型是off_t而不是long

### fgetpos(FILE *fp, fpos_t *pos);
### fsetpos(FILE *fp, fpos_t *pos);

## 内存流
### FILE *fmemopen(void *restrict buf, size_t size, const char *restrict type);
### FILE *open_memstream(char **bufp, size_t *sizep);
### FILE *open_wmemstream(wchar_t **bufp, size_t *sizep);

## 习题
### 5.1
### 5.2
还是会打印一行输入的字符串，原因是buffer overflow

### 5.3
输出字符为0

### 5.4 - 5.7
