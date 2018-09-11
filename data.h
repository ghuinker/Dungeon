#ifndef DATA_H
# define DATA_H


# ifdef __cplusplus
extern "C" {
# endif

# include <stdint.h>

#define DUNGEON_X              80
#define DUNGEON_Y              21
#define MIN_ROOMS              5
#define MAX_ROOMS              9
#define ROOM_MIN_X             4
#define ROOM_MIN_Y             2
#define ROOM_MAX_X             14
#define ROOM_MAX_Y             8



  typedef enum dim {
    dim_x,
    dim_y,
    num_dims
  } dim_t;

  typedef int16_t pair_t[num_dims];



  enum __attribute__ ((__packed__)) terrain_type;
  typedef enum __attribute__ ((__packed__)) terrain_type terrain_type_t;

typedef enum __attribute__ ((__packed__)) terrain_type {
  ter_debug,
  ter_wall,
  ter_wall_immutable,
  ter_floor,
  ter_floor_room,
  ter_floor_hall,
} terrain_type_t;

  struct room;
  typedef struct room room_t;

typedef struct room {
  pair_t position;
  pair_t size;
} room_t;


  struct dungeon;
  typedef struct dungeon dungeon_t;

typedef struct dungeon {
  uint32_t num_rooms;
  room_t rooms[MAX_ROOMS];
  terrain_type_t map[DUNGEON_Y][DUNGEON_X];
  /* Since hardness is usually not used, it would be expensive to pull it *
   * into cache every time we need a map cell, so we store it in a        *
   * parallel array, rather than using a structure to represent the       *
   * cells.  We may want a cell structure later, but from a performanace  *
   * perspective, it would be a bad idea to ever have the map be part of  *
   * that structure.  Pathfinding will require efficient use of the map,  *
   * and pulling in unnecessary data with each map cell would add a lot   *
   * of overhead to the memory system.                                    */
  uint8_t hardness[DUNGEON_Y][DUNGEON_X];
} dungeon_t;

int readIn(int argc, char *argv[]);
  void readDungeon(dungeon_t *d);
  void loadDungeon(dungeon_t *d);
  void saveDungeon(dungeon_t *d);

# ifdef __cplusplus
}
# endif

#endif
