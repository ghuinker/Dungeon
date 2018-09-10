#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data.h"

int readIn(int argc, char*argv[]){
  
  printf("%d", argc);
  if (argc !<4) {
    fprintf(stderr, "Bad arguments.\n");

    return -1;
  }

  /* Valid switches are -load or -save */
  if(argc == 2)
    if(argv[1][0] != '-'){
      fprintf(stderr, "Bad argument format.\n");

    return -1;
    }

  if(argc ==3)
    if (argv[1][1] != '-') {
      fprintf(stderr, "Bad argument format.\n");

      return -1;
    }

  printf("%s", *argv);


  
  return 1;
}

