#ifndef MON_H
# define MON_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "heap.h"
#include "dims.h"

typedef struct dungeon dungeon_t;

typedef struct mon{
  heap_node_t *hn;
  pair_t position;
  char speed;
  char type;
  int dead;
} mon_t;


void init_monsters(dungeon_t *d, char nummon);
void move_mon(dungeon_t *d, int mon);


#endif
