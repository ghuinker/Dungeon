#include <stdio.h>
#include <stdint.h>
#include <string.h>

# include "mon.h"
# include "dungeon.h"
# include "utils.h"

void create_mon(dungeon_t *d, mon_t *mon){
  mon-> type =(char) rand_range(0,15);
  mon->speed =(char) rand_range(5,20);

  mon->position[dim_x] = rand_range(1, DUNGEON_X-1);
  mon->position[dim_y] = rand_range(1, DUNGEON_Y-1);
  
  if(!((mon->type & (1<<2)) >> 2)){
    while(hardnesspair(mon->position) != 0){
	mon->position[dim_x] = rand_range(1, DUNGEON_X-1);
	mon->position[dim_y] = rand_range(1, DUNGEON_Y-1);
    }	
  }
}

void init_monsters(dungeon_t *d, char nummon){
  int i;  
  printf("Creating %d monsters\n", nummon);
  d->nummon = nummon;
  d->monsters = malloc(sizeof(*d->monsters) * d->nummon);
  for(i=0; i<nummon; i++){
    create_mon(d, &(d->monsters[i]));
  }
}
