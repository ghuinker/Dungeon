#ifndef EVENT_H
# define EVENT_H

# include <stdint.h>

# include "dungeon.h"

class character character;

typedef enum event_type {
  event_characterurn,
}

class event {
  event_type_t type;
  uint32_t time;
  uint32_t sequence;
  union {
    character *c;
  };
}

int32_t compare_events(const void *event1, const void *event2);
event_t *new_event(dungeon *d, event_type_t t, void *v, uint32_t delay);
event_t *update_event(dungeon *d, event_t *e, uint32_t delay);
void event_delete(void *e);

#endif
