#ifndef PATH_H
# define PATH_H

# ifdef __cplusplus
extern "C" {
# endif

# include <stdint.h>
#include "heap.h"

  //prototypes here
  typedef struct corridor_path {
  heap_node_t *hn;
  uint8_t pos[2];
  uint8_t from[2];
  int32_t cost;
} corridor_path_t;

  typedef enum dim {
  dim_x,
    dim_y,
    num_dims
    } dim_t;

  typedef int16_t pair_t[num_dims];

#define DUNGEON_X              80
#define DUNGEON_Y              21
#define MIN_ROOMS              5
#define MAX_ROOMS              9
#define ROOM_MIN_X             4
#define ROOM_MIN_Y             2
#define ROOM_MAX_X             14
#define ROOM_MAX_Y             8
#define SAVE_DIR               ".rlg327"
#define DUNGEON_SAVE_FILE      "dungeon"
#define DUNGEON_SAVE_SEMANTIC  "RLG327-F2018"
#define DUNGEON_SAVE_VERSION   0U

#define mapxy(x, y) (d->map[y][x])
#define mappair(pair) (d->map[pair[dim_y]][pair[dim_x]])
#define hardnesspair(pair) (d->hardness[pair[dim_y]][pair[dim_x]])
#define hardnessxy(x, y) (d->hardness[y][x])

  typedef enum __attribute__ ((__packed__)) terrain_type {
    ter_debug,
      ter_wall,
      ter_wall_immutable,
      ter_floor,
      ter_floor_room,
      ter_floor_hall,
      } terrain_type_t;

  typedef struct room {
    pair_t position;
    pair_t size;
  } room_t;

  typedef struct dungeon {
    uint32_t num_rooms;
    room_t *rooms;
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
    pair_t pc;
  } dungeon_t;

  static int32_t corridor_path_cmp(const void *key, const void *with) {
    return ((corridor_path_t *) key)->cost - ((corridor_path_t *) with)->cost;
  }

  //Path.c
  void render_all_paths(dungeon_t *d);
  void render_open_paths(dungeon_t *d);


# ifdef __cplusplus
}
# endif

#endif
