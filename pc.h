#ifndef PC_H
# define PC_H

# include <stdint.h>

# include "dims.h"

dungeon dungeon;

class pc {
}

void pc_delete(pc *pc);
uint32_t pc_is_alive(dungeon *d);
void config_pc(dungeon *d);
uint32_t pc_next_pos(dungeon *d, pair dir);
void place_pc(dungeon *d);
uint32_t pc_in_room(dungeon *d, uint32_t room);

#endif
