#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>
#include "os_stdio.h"
#include "lib.h"

typedef enum event_type_tag {
    EVENT_TYPE_UNKNOWN = 0,
}event_type_e;

typedef struct event_tag {
    event_type_e type;
    list_t wait_list;
}event_t;

void evnet_init(event_t *event, event_type_e type);

#endif /*EVENT_H*/
