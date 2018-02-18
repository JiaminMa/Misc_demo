#ifndef _KERNEL_MM_H
#define _KERNEL_MM_H
#include "types.h"

#define MALCSZ_MIN (0)
#define MALCSZ_MAX (0x400000)
adr_t krlnew(size_t mmsize);
bool_t krldelete(adr_t fradr,size_t frsz);
#endif

