#ifndef _TYPES_H_
#define _TYPES_H_

#define TRUE    1
#define	FALSE	0
#define EXTERN extern

typedef char bool_t;
typedef unsigned int cpuflg_t;
typedef unsigned int reg_t;
typedef unsigned int size_t;
typedef unsigned int adr_t;
typedef int sysstus_t;
typedef unsigned int cpu_t;
typedef void* buf_t;
#define NULL 0

#define LKHEAD_T __attribute__((section(".head.text")))
#define LKHEAD_D __attribute__((section(".head.data")))
#define ALIGN(x, a)     (((x) + (a) - 1) & ~((a) - 1))

typedef int drvstus_t;
#define DFCERRSTUS (-1)
#define DFCOKSTUS (0)
#define NO_HAND (-1)

#endif /*_TYPES_H_*/
