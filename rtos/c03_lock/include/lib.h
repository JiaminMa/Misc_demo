#ifndef LIB_H
#define LIB_H

#include <stdint.h>

typedef struct bitmap_tag {
    uint32_t bitmap;
}bitmap_t;

extern void bitmap_init(bitmap_t *bitmap);
extern uint32_t bitmap_count(void);
extern void bitmap_set(bitmap_t *bitmap, uint32_t pos);
extern void bitmap_clear(bitmap_t *bitmap, uint32_t pos);
extern uint32_t bitmap_get_first_set(bitmap_t *bitmap);

#endif /*LIB_H*/
