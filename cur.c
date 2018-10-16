#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

#include "dungeon.h"
#include "move.h"

void render_dungeon_curs(dungeon_t *d){
  pair_t p;
  
  for (p[dim_y] = 0; p[dim_y] < DUNGEON_Y; p[dim_y]++) {
    for (p[dim_x] = 0; p[dim_x] < DUNGEON_X; p[dim_x]++) {
      if (charpair(p)) {
	mvaddch(p[dim_y], p[dim_x],charpair(p)->symbol);
      } else {
        switch (mappair(p)) {
        case ter_wall:
        case ter_wall_immutable:
	  mvaddch(p[dim_y], p[dim_x], ' ');
          break;
        case ter_floor:
        case ter_floor_room:
	  mvaddch(p[dim_y], p[dim_x], '.');
          break;
        case ter_floor_hall:
	  mvaddch(p[dim_y], p[dim_x], '#');
          break;
        case ter_debug:
	  mvaddch(p[dim_y], p[dim_x], '*');
          break;
        }
      }
    }
  }

  refresh();
}

void run_curses(dungeon_t *d){
  int ch;
  pair_t next;
  while(1){
    if((ch = getch()) != ERR){
      switch(ch){
      case '7':
      case 'y':
	next[dim_x] = d->pc.position[dim_x] - 1;
	next[dim_y] = d->pc.position[dim_y] - 1;
	break;
      case '8':
      case 'k':
	next[dim_x] = d->pc.position[dim_x] + 0;
	next[dim_y] = d->pc.position[dim_y] - 1;
	break;
      case '9':
      case 'u':
	next[dim_x] = d->pc.position[dim_x] + 1;
	next[dim_y] = d->pc.position[dim_y] - 1;
	break;
      case '6':
      case 'l':
	next[dim_x] = d->pc.position[dim_x] + 1;
	next[dim_y] = d->pc.position[dim_y] - 0;
	break;
      case '3':
      case 'n':
	next[dim_x] = d->pc.position[dim_x] + 1;
	next[dim_y] = d->pc.position[dim_y] + 1;
	break;
      case '2':
      case 'j':
	next[dim_x] = d->pc.position[dim_x] + 0;
	next[dim_y] = d->pc.position[dim_y] + 1;
	break;
      case '1':
      case 'b':
	next[dim_x] = d->pc.position[dim_x] - 1;
	next[dim_y] = d->pc.position[dim_y] + 1;
	break;
      case '4':
      case 'h':
	next[dim_x] = d->pc.position[dim_x] - 1;
	next[dim_y] = d->pc.position[dim_y] - 0;
	break;
      }
      if(ch){
	move_character(d, &d->pc, next);
	do_moves(d);
	render_dungeon_curs(d);
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
  
  
  render_dungeon_curs(d);

  
  
}
