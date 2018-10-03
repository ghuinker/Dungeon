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
    
    
    if(tun)
      map_cost = d->pc_tunnel[in_y +y][in_x +x];
    else
      map_cost = d->pc_distance[in_y +y][in_x +x];
    
    if(map_cost < min){
      min = map_cost;
      move[dim_x] = in_x +x;
      move[dim_y] = in_y +y;
    }
  }

  if(min == 256)
    return;
  if(tun || hardnesspair(move) == 0){
    d->monsters[mon].position[dim_x] = move[dim_x];
    d->monsters[mon].position[dim_y] = move[dim_y];
  }
}

void straight_path(dungeon_t *d, int mon){
  
  if(d->pc.position[dim_x]<d->monsters[mon].position[dim_x])
    d->monsters[mon].position[dim_x] = d->monsters[mon].position[dim_x] -1;
  else if(d->pc.position[dim_x]>d->monsters[mon].position[dim_x])
   d->monsters[mon].position[dim_x] = d->monsters[mon].position[dim_x] +1;

  if(d->pc.position[dim_y]<d->monsters[mon].position[dim_y])
    d->monsters[mon].position[dim_y] = d->monsters[mon].position[dim_y] -1;
  else if(d->pc.position[dim_y]>d->monsters[mon].position[dim_y])
    d->monsters[mon].position[dim_y] = d->monsters[mon].position[dim_y] +1;

}

void move_mon(dungeon_t *d, int mon){
  //0000
  pair_t prev;
  char type = d->monsters[mon].type;

  prev[dim_x] = d->monsters[mon].position[dim_x];
  prev[dim_y] = d->monsters[mon].position[dim_y];

  //printf("Previos x: %d, y: %d\n", d->monsters[mon].position[dim_x], d->monsters[mon].position[dim_y]);
  //Just move based on tunneling
  straight_path(d, mon);
  if(!get_bit(type, 2)){
      if(hardnesspair(d->monsters[mon].position)!=0){
	//printf("Resetting Position\n");
	d->monsters[mon].position[dim_x] = prev[dim_x];
	d->monsters[mon].position[dim_y] = prev[dim_y];
	}
    }
  //printf("Next x: %d, y: %d\n", d->monsters[mon].position[dim_x], d->monsters[mon].position[dim_y]);
  
  /*
  pos[dim_x] = d->monsters[mon].position[dim_x];
  pos[dim_y] = d->monsters[mon].position[dim_y];
  next_pos = &(d->monsters[mon].position);

  
  
  //Can't see pc (and erratic)
  if(!get_bit(type,1))
    printf("Not Tele\n");
  
  
  //Smart
  if(get_bit(type, 0)==1){
    //Non Tunneling
    if(!get_bit(type, 2)){
      shortest(d, mon, 0, pos[dim_x], pos[dim_y]);
    } else{
      shortest(d, mon, 1, pos[dim_x], pos[dim_y]);
    }
  } else{
    straight_path(d->pc.position, next_pos);
    printf("NextPos: %d, %d\n", *next_pos[dim_x], *next_pos[dim_y]);
    //Non tunneling
    if(!get_bit(type, 2)){
      if(hardnesspair(*next_pos) != 0)
	next_pos = &pos;
    }
    d->monsters[mon].position[dim_x] = *next_pos[dim_x];
    d->monsters[mon].position[dim_y] = *next_pos[dim_y];
    
  }

  
  */
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
