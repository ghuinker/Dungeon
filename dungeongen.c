#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dungeon.h"

struct room{
  int x;
  int y;
  int width;
  int height;
};

char dun[80][27];
struct room rooms[10];

void setScene(){
  int i, j;
  for(i=0; i<80; i++)
    for(j=0; j<27; j++)
      dun[i][j]=' ';
  for(i=0; i<80; i++){
    dun[i][0]='-';
    dun[i][26]='-';
  }
  for(i=1; i<26; i++){
    dun[0][i]='|';
    dun[79][i]='|';
  }
}

void printDun(){
  int i, j;
  for(i=0; i<27; i++){
    for(j=0; j<80; j++)
      printf("%c", dun[j][i]);
    printf("\n");    
  }
  printf("\n");
}

int checkRoom(int x, int y, int width, int height){
  int i, j;
  if(x+width>=79 || x<2)
    return 0;
  if(y+height>=26 || y<2)
    return 0;

  for(i=x-1; i<x+width+1; i++)
    for(j=y-1; j<y+height+1; j++)
      if(dun[i][j] == '.')
	return 0;
  return 1;
}

int createRoom(int x, int y, int width, int height){
  if(!checkRoom(x, y, width, height)){
    return 0;

  }
  int i, j;
  for(i=x; i<x+width; i++)
    for(j=y; j<y+height; j++)
      dun[i][j]='.';
  return 1;

}

struct room getRanRoom(){
  struct room room;
  room.x = rand() % (77 + 1 - 2) + 2;
  room.y = rand() % (24 + 1 - 2) + 2;
  room.width = rand() % (20 + 1 - 3) + 3;
  room.height = rand() % (20 + 1 -2) + 2;
  return room;
}

void initRooms(){
  int  tries;
  int count = 0;
  while(count<5 || tries<100){
    if(count>=10)
      break;
    struct room room = getRanRoom();

    if(createRoom(room.x, room.y, room.width, room.height))
      rooms[count++] = room;
    tries++;
  }
  printf("%d\n", tries);

  
  
}



void createDungeon(int diff){
  srand(time(NULL));
  setScene();
  initRooms();
  printDun();
}

