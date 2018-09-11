#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data.h"

FILE *f;

typedef struct {
  char *type;
  uint32_t marker;
  uint32_t size;
  uint8_t x;
  uint8_t y;
  char *dungeon[DUNGEON_Y][DUNGEON_X];
  uint8_t rooms[];

} save_t;




void loadDungeon(dungeon_t *d){
  char loc[80];
  //strcpy(loc, getenv("HOME"));
  strcpy(loc, "");
  strcat(loc, ".rlg327/dungeon");
  f = fopen(loc, "r");
  if(f == NULL)
    printf("f is null");

  save_t s;
  fread(s, sizeof(save_t), 1, f);
  
  int i, j;
  for(i=0; i<DUNGEON_Y; i++){
    for(j=0; j<DUNGEON_X; j++)
      printf("%s", s.dungeon[i][j]);
    printf("\n");
  }

  fclose(f);

}

void saveDungeon(dungeon_t *d){
  char loc[80];
  //strcpy(loc, getenv("HOME"));
  strcpy(loc, "");
  strcat(loc, ".rlg327/dungeon");
  f = fopen(loc, "w");
  if(f == NULL)
    printf("f is null");
  save_t s;
  fwrite(s, sizeof(save_t), 1, f);
  
  fclose(f);
}

int returnAction(char*argv[]){
  if(strlen(*argv) == (size_t)6){
    if(!strcmp(*argv, "--save"))
       return 1;
       else if(!strcmp(*argv, "--load"))
	 return 2;
  }
  return -1;
}


int readIn(int argc, char*argv[]){
  if(argc ==1)
    return 0;
  
  if (!(argc < 4)) {
    fprintf(stderr, "Bad arguments.\n");

    return -1;
  }

  int action =-1;
  /* Valid switches are -load or -save */
 
  if(argc == 2){
    action = returnAction(&argv[1]);
  } 

  if(argc == 3){
    int action2 = -1;
    action = returnAction(&argv[1]);
    action2 = returnAction(&argv[2]);
  
    if(action != action2)
      if(action != -1 && action2 != -1)
	action = 3;
    }

  printf("Action: %d\n", action);
  return action;

}


