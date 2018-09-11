#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data.h"

FILE *f;



void loadDungeon(dungeon_t *d){
  char loc[80];
  //strcpy(loc, getenv("HOME"));
  strcpy(loc, "");
  strcat(loc, ".rlg327/dungeon");
  f = fopen(loc, "r");
  if(f == NULL)
    printf("f is null");
  fread(d, sizeof(dungeon_t), 1, f);
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
  
  fwrite(d, sizeof(dungeon_t), 1, f);
  
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


