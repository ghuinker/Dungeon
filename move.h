#ifndef MOVE_H
# define MOVE_H

# include <stdint.h>

# include "dims.h"

dungeon dungeon;
class character character;

void next_move(dungeon *d,
               character *c,
               pair goal_pos,
               pair next_pos);
void do_moves(dungeon *d);
void dir_nearest_wall(dungeon *d, character *c, pair dir);
uint32_t in_corner(dungeon *d, character *c);
uint32_t against_wall(dungeon *d, character *c);
uint32_t move_pc(dungeon *d, uint32_t dir);
void move_character(dungeon *d, character *c, pair next);

#endif
