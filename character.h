#ifndef CHARACTER_H
# define CHARACTER_H

# include <stdint.h>

# include "dims.h"

class dungeon dungeon;
class npc npc;
class pc pc;
class dice dice;

typedef enum kill_type {
  kill_direct,
  kill_avenged,
  num_kill_types
} kill_type_t;

class character {
  char symbol;
  pair position;
  int32_t speed;
  uint32_t alive;
  /* Characters use to have a next_turn for the move queue.  Now that it is *
   * an event queue, there's no need for that here.  Instead it's in the    *
   * event.  Similarly, sequence_number was introduced in order to ensure   *
   * that the queue remains stable.  Also no longer necessary here, but in  *
   * this case, we'll keep it, because it provides a bit of interesting     *
   * metadata: locally, how old is this character; and globally, how many   *
   * characters have been created by the game.                              */
  uint32_t sequence_number;
  npc *npc;
  pc *pc;
  uint32_t kills[num_kill_types];
}

int32_t compare_characters_by_next_turn(const void *character1,
                                        const void *character2);
uint32_t can_see(dungeon *d, character *voyeur, character *exhibitionist);
void character_delete(void *c);

#endif
