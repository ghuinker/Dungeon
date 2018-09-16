#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/time.h>
#include <errno.h>


#include "path.h"
#include "heap.h"




static void dijkstra_open(dungeon_t *d, uint32_t dist[DUNGEON_Y][DUNGEON_X],  pair_t from, pair_t to)
{
  static corridor_path_t path[DUNGEON_Y][DUNGEON_X], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;


  if (!initialized) {
    for (y = 0; y < DUNGEON_Y; y++) {
      for (x = 0; x < DUNGEON_X; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }

    }
    initialized = 1;
  }
  
  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      path[y][x].cost = INT_MAX;
    }
  }

  path[from[dim_y]][from[dim_x]].cost = 0;

  heap_init(&h, corridor_path_cmp, NULL);

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
     
      if (mapxy(x, y) == ter_floor_room || mapxy(x,y) == ter_floor_hall ) {
        path[y][x].hn = heap_insert(&h, &path[y][x]);
      } else {
        path[y][x].hn = NULL;
      }
    }
  }

  while ((p = heap_remove_min(&h))) {
    p->hn = NULL;
    int pos =0;
    if ((p->pos[dim_y] == to[dim_y]) && p->pos[dim_x] == to[dim_x]) {
      for (x = to[dim_x], y = to[dim_y];
           (x != from[dim_x]) || (y != from[dim_y]);
           p = &path[y][x], x = p->from[dim_x], y = p->from[dim_y]) {
	//If this Location is something you want to check

        if (mapxy(x, y) == ter_floor_room || mapxy(x,y) == ter_floor_hall) {
	  //mapxy(x, y) = ter_debug;
          //hardnessxy(x, y) = 0;
	  if(dist[y][x] == INT_MAX){
	    dist[y][x] = pos++;
	  }
        }
      }
      heap_delete(&h);
      return;
    }
    /*
//Diagonal Movement
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] +1 ].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] +1 ].cost >
         p->cost )) {
      path[p->pos[dim_y] - 1][p->pos[dim_x] +1 ].cost =
        p->cost ;
      path[p->pos[dim_y] - 1][p->pos[dim_x] +1 ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x] +1 ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
				   [p->pos[dim_x]  +1].hn);
    }
    if ((path[p->pos[dim_y]  -1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y]  -1][p->pos[dim_x] - 1].cost >
         p->cost)) {
      path[p->pos[dim_y] -1 ][p->pos[dim_x] - 1].cost =
        p->cost;
      path[p->pos[dim_y]  -1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] -1 ][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]  -1]
				   [p->pos[dim_x] - 1].hn);
    }

    if ((path[p->pos[dim_y] +1 ][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y] +1 ][p->pos[dim_x] + 1].cost >
         p->cost )) {
      path[p->pos[dim_y] +1 ][p->pos[dim_x] + 1].cost =
        p->cost ;
      path[p->pos[dim_y] +1 ][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] +1 ][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]  +1]
				   [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] -1 ].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] -1 ].cost >
         p->cost )) {
      path[p->pos[dim_y] + 1][p->pos[dim_x] -1 ].cost =
        p->cost;
      path[p->pos[dim_y] + 1][p->pos[dim_x] -1 ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x] -1 ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
				   [p->pos[dim_x]-1  ].hn);
    }
    */
    //Straight Line last because it looks better
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost >
         p->cost )) {
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost =
        p->cost ;
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]    ].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost >
         p->cost)) {
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost =
        p->cost;
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost >
         p->cost )) {
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost =
        p->cost ;
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost >
         p->cost )) {
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost =
        p->cost;
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x]    ].hn);
    }
    
  }
  
}

void render_all_paths(dungeon_t *d){
  printf("Rendering All Paths\n");
  
}

void render_open_paths(dungeon_t *d){
  printf("Rendering Open Paths\n");

  int y, x;

  pair_t from, to;
  from[dim_x] = d->pc[dim_x];
  from[dim_y] = d->pc[dim_y];
 

  uint32_t dist[DUNGEON_Y][DUNGEON_X];
  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      dist[y][x] = INT_MAX;
    }
  }

  for(y=0; y<DUNGEON_Y; y++)
    for(x=0; x<DUNGEON_X; x++)
      if(dist[y][x] == INT_MAX)
	if(in_room(d, y, x)){
	  to[dim_y] = y;
	  to[dim_x] = x;
	  
	  dijkstra_open(d, dist, from, to);
      }
  

  
  for(y=0; y<DUNGEON_Y; y++){
    for(x=0; x<DUNGEON_X; x++)
      if(x == d->pc[dim_x] && y == d->pc[dim_y])
	printf("@");
    
      else if(dist[y][x] != INT_MAX)
	printf("%d", dist[y][x] %10);
      else
	printf(" ");
    printf("\n");
  }
  
}
