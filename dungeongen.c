#include <stdio.h>
#include "dungeon.h"

char dun[27][80];

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
void createDungeon(int diff){
  setScene();
  printDun();
}

