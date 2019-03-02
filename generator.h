#ifndef GENERATOR_H
# define GENERATOR_H

#include "heap.h"
#include "coordinates.h"

#define DUN_WIDTH 80
#define DUN_HEIGHT 21
#define MAX_ROOMS 6
#define FILE_VERISON 0
#define FILE_NAME "/.rlg327/01.rlg327"
#define DUN_ROOMS dungeon->rooms
#define DUN_ARR dungeon->dungeon
#define DUN_HARD dungeon->hardness
#define ROCK ' '
#define BORDER 'X'
#define CORRIDOR '#'
#define GROUND '.'

#define hardnesspair(pair) (dungeon->hardness[pair[dim_y]][pair[dim_x]])


typedef struct room{
    int x_pos;
    int y_pos;
    int x_len;
    int y_len;
}room_t;

typedef struct dungeon {
    char dungeon[DUN_HEIGHT][DUN_WIDTH];
    uint16_t num_rooms;
    room_t *rooms;
    uint8_t hardness[DUN_HEIGHT][DUN_WIDTH];
    pair_t pc;
    pair_t up_stair;
    pair_t down_stair;
} dungeon_t;

typedef struct corridor_path {
  heap_node_t *hn;
  uint8_t pos[2];
  int32_t cost;
} corridor_path_t;

typedef struct {
    int x_pos;
    int y_pos;
}stair;

void print_dungeon      (dungeon_t* dungeon);
void populate_dungeon   (dungeon_t* dungeon);
int create_rooms        (dungeon_t* dungeon);
void print_room_array   (dungeon_t* dungeon);
int connect_rooms       (dungeon_t* dungeon, int r1, int r2);
int find_closest_room   (dungeon_t*, int curr_room);
void generate_dungeon   (dungeon_t* dungeon, stair u_stairs[1], stair d_stairs[1], int nummon);
int saver               (dungeon_t* dungeon, stair u_stairs[1], stair d_stairs[1]);
void make_hard          (dungeon_t* dungeon);
int loader              (dungeon_t* dungeon);
uint32_t file_size      ();
int in_room(dungeon_t* dungeon, pair_t location);



#endif