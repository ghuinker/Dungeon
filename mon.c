#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

# include "mon.h"
# include "dungeon.h"
# include "utils.h"

int is_in_room(dungeon_t *d, pair_t pos){
  pair_t *size=&(d->rooms[0].size), *loc=&(d->rooms[0].position);
  
  if(pos[dim_x]>*loc[dim_x])
    if(pos[dim_y]>*loc[dim_y])
      if(pos[dim_x]<*loc[dim_x] + *size[dim_x])
	if(pos[dim_y]<*loc[dim_y]+*size[dim_y])
	  return 1;
  return 0;
}

int get_bit(int num, int bit){
  return (num >> bit) & 1;
}

void shortest(dungeon_t *d, int mon, int tun, int in_x, int in_y){
  pair_t move;
  int min=256;
  int i,x,y, map_cost;

  move[dim_x] = in_x;
  move[dim_y] = in_y; 
  
  for(i=0; i<8; i++){
    switch(i){
    case 0:x=0;y=-1;break;
    case 1:x=1;y=-1;break;
    case 2:x=1;y=0;break;
    case 3:x=1;y=1;break;
    case 4:x=0;y=1;break;
    case 5:x=-1;y=1;break;
    case 6:x=-1;y=0;break;
    case 7:x=-1;y=-1;break;
    }
    
    
    printf("Shortest: %d, %d\n",in_x +x,in_y +y);
    if(tun)
      map_cost = d->pc_tunnel[in_y +y][in_x +x];
    else
      map_cost = d->pc_distance[in_y +y][in_x +x];
    printf("Map Cost: %d\n",map_cost);
    
    if(map_cost < min){
      min = map_cost;
      move[dim_x] = in_x +x;
      move[dim_y] = in_y +y;
    }
  }

  if(min == 256)
    return;
  printf("New Space should be: %d, %d\n", move[dim_x], move[dim_y]);
  d->monsters[mon].position[dim_x] = move[dim_x];
  d->monsters[mon].position[dim_y] = move[dim_y];
}

void straight_path(pair_t pc, pair_t *mon){
  if(pc[dim_x]<*mon[dim_x])
    *mon[dim_x] = *mon[dim_x] -1;
  else if(pc[dim_x]>*mon[dim_x])
    *mon[dim_x] = *mon[dim_x] +1;

  if(pc[dim_y]<*mon[dim_y])
    *mon[dim_y] = *mon[dim_y] -1;
  else if(pc[dim_y]>*mon[dim_y])
    *mon[dim_y] = *mon[dim_y] +1;
}

void move_mon(dungeon_t *d, int mon){
  //0000
  pair_t pos;
  pair_t *next_pos;
  char type = d->monsters[mon].type;

  

  pos[dim_x] = d->monsters[mon].position[dim_x];
  pos[dim_y] = d->monsters[mon].position[dim_y];
  next_pos = &(d->monsters[mon].position);

  
  
  //Can't see pc (and erratic)
  if(!((type & (1<<0)) >> 0) && !is_in_room(d, pos))
    return;
  
  printf("Previos x: %d, y: %d\n", pos[dim_x], pos[dim_y]);
  //Smart
  if(get_bit(type, 0)){
    //Non Tunneling
    if(!get_bit(type, 2)){
      shortest(d, mon, 0, pos[dim_x], pos[dim_y]);
    } else{
      shortest(d, mon, 1, pos[dim_x], pos[dim_y]);
    }
  } else{
    straight_path(d->pc.position, next_pos);
    //Non tunneling
    if(!get_bit(type, 2)){
      if(hardnesspair(*next_pos) != 0)
	next_pos = &pos;
    }
    d->monsters[mon].position[dim_x] = *next_pos[dim_x];
    d->monsters[mon].position[dim_y] = *next_pos[dim_y];
    
  }

  printf("Next x: %d, y: %d\n", d->monsters[mon].position[dim_x], d->monsters[mon].position[dim_y]);
}

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
