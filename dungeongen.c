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

char dun[27][80];
struct room rooms[10];

void setScene(){
  int i, j;
  for(i=0; i<27; i++)
    for(j=0; j<80; j++)
      dun[i][j]=' ';
  for(i=0; i<80; i++){
    dun[0][i]='-';
    dun[26][i]='-';
  }
  for(i=1; i<26; i++){
    dun[i][0]='|';
    dun[i][79]='|';
  }
}

void printDun(){
  int i, j;
  for(i=0; i<27; i++){
    for(j=0; j<80; j++)
      printf("%c", dun[i][j]);
    printf("\n");    
  }
  printf("\n");
}

int checkRoom(int x, int y, int width, int height){
  int i, j;
  if(x+width>=26 || x<2)
    return 0;
  if(y+height>=79 || y<2)
    return 0;

  for(i=x; i<x+width; i++)
    for(j=y; j<y+height; j++)
      if(dun[i][j] == '.')
	return 0;
  return 1;
}

int createRoom(int x, int y, int width, int height){
  if(!checkRoom(x, y, width, height)){
    printf("Not Valid")
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
  room.x = rand() % (24 + 1 - 2) + 2;
  room.y = rand() % (77 + 1 - 2) + 2;
  room.width = rand() % (20 + 1 - 3) + 3;
  room.height = rand() % (20 + 1 -2) + 2;
  printf("%d, %d, %d, %d\n", room.x, room.y, room.width, room.height);
  return room;
}

void initRooms(){
  struct room room = getRanRoom();
  createRoom(room.x, room.y, room.width, room.height);
  
}



void createDungeon(int diff){
  srand(time(NULL));
  setScene();
  initRooms();
  printDun();
}

