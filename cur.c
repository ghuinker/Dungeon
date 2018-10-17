#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

#include "dungeon.h"
#include "move.h"
#include "pc.h"
#include "npc.h"

void render_dungeon_curs(dungeon_t *d){
  pair_t p;
  
  for (p[dim_y] = 0; p[dim_y] < DUNGEON_Y; p[dim_y]++) {
    for (p[dim_x] = 0; p[dim_x] < DUNGEON_X; p[dim_x]++) {
      if (charpair(p)) {
	mvaddch(p[dim_y]+1, p[dim_x],charpair(p)->symbol);
      } else {
        switch (mappair(p)) {
        case ter_wall:
        case ter_wall_immutable:
	  mvaddch(p[dim_y]+1, p[dim_x], ' ');
          break;
        case ter_floor:
        case ter_floor_room:
	  mvaddch(p[dim_y]+1, p[dim_x], '.');
          break;
        case ter_floor_hall:
	  mvaddch(p[dim_y]+1, p[dim_x], '#');
          break;
        case ter_debug:
	  mvaddch(p[dim_y]+1, p[dim_x], '*');
          break;
        }
      }
    }
  }

  refresh();
}



void print_message(char *mes){
  int i=0;
  while(mes[i]){
    mvaddch(0, i, mes[i++]);
  }
}

void clear_message(){
  move(0, 0);
  clrtoeol();
}



void mon_menu(dungeon_t *d){
  clear_message();


  pair_t p;

  char *list = malloc(34 * d->num_monsters);

  int size = 0;

  endwin();
  for (p[dim_y] = 0; p[dim_y] < DUNGEON_Y; p[dim_y]++) 
    for (p[dim_x] = 0; p[dim_x] < DUNGEON_X; p[dim_x]++)
      if (charpair(p)) 
	if(charpair(p)->symbol != '@'){
	  memcpy(list+size, "Monster ", 8);
	  size = size +8;
	  memcpy(list + size, &charpair(p)->symbol, 1);
	  size = size +1;
	  memcpy(list+size, " : ", 3);
	  size = size +3;
	  
	  int dist_y = charpair(p)->position[dim_y] - d->pc.position[dim_y];
	  int dist_x = charpair(p)->position[dim_x] - d->pc.position[dim_x];

	  char loc[22];

	  char vert[6];
	  char hor[5];

	  if(dist_y < 0)
	    strcpy(vert, "South");
	  else
	    strcpy(vert, "North");
	  if(dist_x < 0)
	    strcpy(hor, "West");
	  else
	    strcpy(hor, "East");
	
	  sprintf(loc, "%d %s and %d %s\n", abs(dist_y), vert, abs(dist_x), hor);

	  
	  memcpy(list+size, loc, strlen(loc));
	  size = size + strlen(loc);
	}

  int i=0, j=0, k=0;
  while(list[i]){
    mvaddch(j, k++, list[i]);
    if(list[i++] == '\n'){
      j++;
      k=0;
    }
  }

  free(list);
  while(!getch()){

  }
  endwin();
  exit(1);
  
  clear();
}

void run_curses(dungeon_t *d){
  int ch;
  pair_t next;
  while(pc_is_alive(d) && dungeon_has_npcs(d)){
    if((ch = getch()) != ERR){
      switch(ch){
      case 'q':
	break;
      case '5':
      case ' ':
	ch = 5;
	//Upper Left
      case '7':
      case 'y':
	next[dim_x] = d->pc.position[dim_x] - 1;
	next[dim_y] = d->pc.position[dim_y] - 1;
	break;
	//Up
      case '8':
      case 'k':
	next[dim_x] = d->pc.position[dim_x] + 0;
	next[dim_y] = d->pc.position[dim_y] - 1;
	break;
	//Upper Right
      case '9':
      case 'u':
	next[dim_x] = d->pc.position[dim_x] + 1;
	next[dim_y] = d->pc.position[dim_y] - 1;
	break;
	//Right
      case '6':
      case 'l':
	next[dim_x] = d->pc.position[dim_x] + 1;
	next[dim_y] = d->pc.position[dim_y] - 0;
	break;
	//Lower Right
      case '3':
      case 'n':
	next[dim_x] = d->pc.position[dim_x] + 1;
	next[dim_y] = d->pc.position[dim_y] + 1;
	break;
	//Down
      case '2':
      case 'j':
	next[dim_x] = d->pc.position[dim_x] + 0;
	next[dim_y] = d->pc.position[dim_y] + 1;
	break;
	//Lower Left
      case '1':
      case 'b':
	next[dim_x] = d->pc.position[dim_x] - 1;
	next[dim_y] = d->pc.position[dim_y] + 1;
	break;
	//Left
      case '4':
      case 'h':
	next[dim_x] = d->pc.position[dim_x] - 1;
	next[dim_y] = d->pc.position[dim_y] - 0;
	break;
      }
      if(ch){
	if(ch == 'q')
	  break;
	if(ch == 5){
	  do_moves(d);
	  render_dungeon_curs(d);
	  clear_message();
	}
	else if(hardnesspair(next) ==0){
	  move_character(d, &d->pc, next);
	  do_moves(d);
	  render_dungeon_curs(d);
	  clear_message();
	} else{
	  print_message("There is a wall in the way!");
	}
	ch = 0;
      }
    }
  }

  endwin();
}


void init_curses(dungeon_t *d){
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  mon_menu(d);
  
  render_dungeon_curs(d);
  
}
