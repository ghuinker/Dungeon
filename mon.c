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

void shortest(uint8_t map[DUNGEON_Y][DUNGEON_X],int in_x, int in_y, pair_t *pos){
  pair_t move;
  int min=UCHAR_MAX;
  int slick=0;
  int map_cost;
  
  while(slick<8){
    int x=0;
    int y=0;
    int fix=0;
    
    if(get_bit(slick,0))
      x=1;
    if(get_bit(slick, 1))
      y = 1;
    
    if(get_bit(slick, 2)){
      fix=1;
    }

    if(x>DUNGEON_X-1 || x<1)
      x = 0;
    if(y>DUNGEON_Y-1 || y<1)
      y = 0;
    
    printf("Shortest: %d, %d\n",in_x +x -fix , in_y +y - fix);
    map_cost = map[in_x +x-fix][in_y +y -fix];
    printf("Map Cost: %d\n",map_cost); 
    if(map_cost < min){
      printf("Cost Down\n");
      min = map_cost;
      move[dim_x] = in_x +x -fix;
      move[dim_y] = in_y +y -fix ;
    }

    
    slick++;
  }

  if(min == UCHAR_MAX)
    return;
  printf("New Space should be: %d, %d", move[dim_x], move[dim_y]);
  pos = &move;
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
  if((type & (1<<0)) >> 0){
    //Non Tunneling
    if(!((type & (1<<2)) >> 2)){
      shortest(d->pc_distance, pos[dim_x], pos[dim_y],&pos);
    } else{
     shortest(d->pc_tunnel, pos[dim_x], pos[dim_y], &pos);
    }
  } else{
    straight_path(d->pc.position, next_pos);
    //Non tunneling
    if(!((type & (1<<2)) >> 2)){
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
