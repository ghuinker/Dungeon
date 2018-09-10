#ifndef DATA_H
# define DATA_H


# ifdef __cplusplus
extern "C" {
# endif

# include <stdint.h>

int readIn(int argc, char *argv[]);
  void readDungeon(dungeon_t *d);

# ifdef __cplusplus
}
# endif

#endif
