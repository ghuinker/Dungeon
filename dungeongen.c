#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "dungeon.h"

struct room{
  int x;
  int y;
  int width;
  int height;
  int connection;
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
  room.connection = -1;
  return room;
}

int dis(int x1, int y1, int x2, int y2){
  return (int) sqrt((pow((x2-x1), 2)+(pow((y2-y1), 2))));
}

int mid(int point, int length){
  return abs(point + (length/2));
}

int distance(struct room rm1, struct room rm2){
  int cx1 = mid(rm1.x, rm1.width);
  int cy1= mid(rm1.y, rm1.height);
  int cx2 = mid(rm2.x, rm2.width);
  int cy2 = mid(rm2.y, rm2.height);
  return dis(cx1, cy1, cx2, cy2);
}

int * pathPoints(struct room rm1, struct room rm2){
  static int pts[4];  
  int startx = rm1.x;
  int starty = rm1.y;
  int endx = rm2.x;
  int endy = rm2.y;
  
  if(rm1.x < rm2.x){
    startx = rm1.x;
    starty = rm1.y;
    endx = rm2.x;
    endy = rm2.y;
    if(rm1.x + rm1.width < rm2.x){
      startx += rm1.width;
      starty++;
    }
    if(rm1.y + rm1.height <=rm2.y)
      starty += rm1.height;
  }

  if(rm2.x< rm1.x){
    startx = rm2.x;
    starty = rm2.y;
    endx = rm1.x;
    endy = rm1.y;
    if(rm2.x + rm2.width < rm1.x){
      startx += rm2.width;
      starty++;
    }
    if(rm2.y + rm2.height <= rm1.height)
      starty += rm2.height;
  }

  

  pts[0] = startx;
  pts[1] = starty;
  pts[2] = endx;
  pts[3] = endy;


  return pts;
}

int connect(int roomIndex, int len){
  int i, minDis, minRoom;
  minDis = 1000;
  struct room room = rooms[roomIndex];
  for(i=0; i<len; i++){
    struct room tmp = rooms[i];
    int dist;
    if(i==roomIndex || roomIndex==rooms[i].connection)
      continue;
    if((dist = distance(room, tmp))<minDis){
      minDis = dist;
      minRoom = i;
    }
  }
  rooms[roomIndex].connection = minRoom;
  return 0;
}


int createPath(struct room rm1, struct room rm2){ 
  int *pts;
  pts = pathPoints(rm1, rm2);
  int i, j;
  for(i=pts[0]; i<pts[2]; i++){
    if(dun[i][pts[1]] == ' '){
      dun[i][pts[1]] = '#';
      if(pts[1] < pts[3])
	pts[1]++;
      if(pts[1] > pts[3]) 
	pts[1]--;
    }
  }
  if(pts[1] != pts[3]){
    for(j=pts[1]; j<pts[3]; j++)
      if(dun[i][j] == ' ')
	dun[i][j] = '#';
    for(j=pts[1]; j>pts[3]; j--)
      if(dun[i][j]== ' ')
	dun[i][j] = '#';
  }
  return 0;
}

void initPaths(int len){
  int i;
  for(i=0; i<len; i++){
    connect(i, len);
    printf("%d:%d\n", i, rooms[i].connection);
  }
  for(i=0; i<len; i++){
    createPath(rooms[i],rooms[rooms[i].connection]);
  }
}

void initRooms(){
  int  tries;
  int count = 0;
  while(count<5 || tries<100){
    if(count>=10)
      break;
    struct room room = getRanRoom();

    if(createRoom(room.x, room.y, room.width, room.height)){
      rooms[count++] = room;
      dun[room.x][room.y] = count-1 +'0';
    }
    tries++;
  }
  
  initPaths(count);
  
}


void createDungeon(int diff){
  srand(time(NULL));
  setScene();
  initRooms();
  printDun();
}

