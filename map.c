#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "generator.h"
#include "map.h"

int32_t corridor_path_cmp(const void *key, const void *with) {
  return ((corridor_path_t *) key)->cost - ((corridor_path_t *) with)->cost;
}



void print_heat_map_int(int path[DUN_HEIGHT][DUN_WIDTH]){

    int y = 0, x = 0;

    for (y = 0; y < DUN_HEIGHT; y++) {
        for (x = 0; x < DUN_WIDTH; x++) {
            if(path[y][x] == 0){
                printf("\033[1;32m"); // bold green
                printf("@");
                printf("\033[0m");
            }else if (path[y][x] != INT_MAX){
                if(path[y][x] < 10){
                    printf("\033[0;32m"); // green
                    printf("%d", path[y][x] % 10);
                    printf("\033[0m");
                }else if(path[y][x] < 20){
                    printf("\033[0;33m"); // yellow
                    printf("%d", path[y][x] % 10);
                    printf("\033[0m");
                }else if(path[y][x] < 30){
                    printf("\033[1;33m"); // bold yellow
                    printf("%d", path[y][x] % 10);
                    printf("\033[0m");
                }else{
                    printf("\033[0;31m"); //red
                    printf("%d", path[y][x] % 10);
                    printf("\033[0m");
                }
                
            }else{
                printf(" ");
            }
        }
        printf("\n");
    }

}



void print_heat_map(corridor_path_t path[DUN_HEIGHT][DUN_WIDTH]){

    int y = 0, x = 0;

    for (y = 0; y < DUN_HEIGHT; y++) {
        for (x = 0; x < DUN_WIDTH; x++) {
            if(path[y][x].cost == 0){
                printf("\033[1;32m"); // bold green
                printf("@");
                printf("\033[0m");
            }else if (path[y][x].cost != INT_MAX){
                if(path[y][x].cost < 10){
                    printf("\033[0;32m"); // green
                    printf("%d", path[y][x].cost % 10);
                    printf("\033[0m");
                }else if(path[y][x].cost < 20){
                    printf("\033[0;33m"); // yellow
                    printf("%d", path[y][x].cost % 10);
                    printf("\033[0m");
                }else if(path[y][x].cost < 30){
                    printf("\033[1;33m"); // bold yellow
                    printf("%d", path[y][x].cost % 10);
                    printf("\033[0m");
                }else{
                    printf("\033[0;31m"); //red
                    printf("%d", path[y][x].cost % 10);
                    printf("\033[0m");
                }
                
            }else{
                printf(" ");
            }
        }
        printf("\n");
    }

}

void copy_map(corridor_path_t path[DUN_HEIGHT][DUN_WIDTH], int map[DUN_HEIGHT][DUN_WIDTH]){

  int y, x;
    for (y = 0; y < DUN_HEIGHT; y++) {
        for (x = 0; x < DUN_WIDTH; x++) {
          map[y][x] = path[y][x].cost;
        }
    }

  //print_heat_map(path);
  //print_heat_map_int(map);
  

}


void dijkstra(dungeon_t *dungeon, int map[DUN_HEIGHT][DUN_WIDTH])
{
  static corridor_path_t path[DUN_HEIGHT][DUN_WIDTH], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;

    //Cost of source is 0.
  pair_t to;
  to[dim_x] = dungeon->pc[dim_x];
  to[dim_y] = dungeon->pc[dim_y];



  if (!initialized) {
    for (y = 0; y < DUN_HEIGHT; y++) {
      for (x = 0; x < DUN_WIDTH; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }

  //For all nodes, cost of each node is infinity.
  
  for (y = 0; y < DUN_HEIGHT; y++) {
    for (x = 0; x < DUN_WIDTH; x++) {
      path[y][x].cost = INT_MAX;
    }
  }

    path[to[dim_y]][to[dim_x]].cost = 0;

  

  heap_init(&h, corridor_path_cmp, NULL);

    //add to heap.
  for (y = 0; y < DUN_HEIGHT; y++) {
    for (x = 0; x < DUN_WIDTH; x++) {
      if (DUN_ARR[y][x] != BORDER) {
        path[y][x].hn = heap_insert(&h, &path[y][x]);
      } else {
        path[y][x].hn = NULL;
      }
    }
  }

  while ((p = heap_remove_min(&h))) { //While not empty.
    p->hn = NULL;

      /*We need 8 of these (the diagnols*/
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost >
         p->cost +  (hardnesspair(p->pos))/85 + 1)) {
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost =
        p->cost + (hardnesspair(p->pos))/85 + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]    ].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost >
         p->cost + (hardnesspair(p->pos))/85 + 1)) {
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost =
        p->cost + (hardnesspair(p->pos))/85 + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost >
         p->cost + (hardnesspair(p->pos))/85 + 1)) {
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost =
        p->cost + (hardnesspair(p->pos))/85 + 1;
        heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost >
         p->cost + (hardnesspair(p->pos))/85 + 1)) {
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost =
        p->cost + (hardnesspair(p->pos))/85 + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x]    ].hn);
    }

    //Diags
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost >
         p->cost + (hardnesspair(p->pos))/85 + 1)) {
      path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost =
        p->cost + (hardnesspair(p->pos))/85 + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost >
         p->cost + (hardnesspair(p->pos))/85 + 1)) {
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost =
        p->cost + (hardnesspair(p->pos))/85 + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost >
         p->cost + (hardnesspair(p->pos))/85 + 1)) {
      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1 ].cost =
        p->cost + (hardnesspair(p->pos))/85 + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]  + 1].hn);
    }
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1 ].cost >
         p->cost + (hardnesspair(p->pos))/85 + 1)) {
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost =
        p->cost + (hardnesspair(p->pos))/85 + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x] - 1].hn);
    }
  }


    copy_map(path, map);
  

}

void dijkstra_no_weight(dungeon_t *dungeon, int map[DUN_HEIGHT][DUN_WIDTH])
{
  static corridor_path_t path[DUN_HEIGHT][DUN_WIDTH], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;

    //Cost of source is 0.
  pair_t to;
  to[dim_x] = dungeon->pc[dim_x];
  to[dim_y] = dungeon->pc[dim_y];
  


  if (!initialized) {
    for (y = 0; y < DUN_HEIGHT; y++) {
      for (x = 0; x < DUN_WIDTH; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }

  //For all nodes, cost of each node is infinity.
  
  for (y = 0; y < DUN_HEIGHT; y++) {
    for (x = 0; x < DUN_WIDTH; x++) {
      path[y][x].cost = INT_MAX;
    }
  }

  path[to[dim_y]][to[dim_x]].cost = 0;

  

  heap_init(&h, corridor_path_cmp, NULL);

    //add to heap.
  for (y = 0; y < DUN_HEIGHT; y++) {
    for (x = 0; x < DUN_WIDTH; x++) {
      if (DUN_ARR[y][x] != BORDER) {
        path[y][x].hn = heap_insert(&h, &path[y][x]);
      } else {
        path[y][x].hn = NULL;
      }
    }
  }

  while ((p = heap_remove_min(&h))) { //While not empty.
    p->hn = NULL;

      /*We need 8 of these (the diagnols*/
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
        (DUN_ARR[p->pos[dim_y] - 1][p->pos[dim_x]] != ROCK) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost >
         p->cost + 1)) {
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost =
        p->cost + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]    ].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
        (DUN_ARR[p->pos[dim_y]][p->pos[dim_x] - 1] != ROCK) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost >
         p->cost + 1)) {
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost =
        p->cost + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
        (DUN_ARR[p->pos[dim_y]][p->pos[dim_x] + 1] != ROCK) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost >
         p->cost + 1)) {
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost =
        p->cost + 1;
        heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
        (DUN_ARR[p->pos[dim_y] + 1][p->pos[dim_x]] != ROCK) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost >
         p->cost + 1)) {
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost =
        p->cost + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x]    ].hn);
    }

    //Diagnols
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1 ].hn) &&
        (DUN_ARR[p->pos[dim_y] + 1][p->pos[dim_x] + 1] != ROCK) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]  + 1].cost >
         p->cost + 1)) {
      path[p->pos[dim_y] + 1][p->pos[dim_x]  + 1].cost =
        p->cost + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1 ].hn) &&
        (DUN_ARR[p->pos[dim_y] + 1][p->pos[dim_x] - 1] != ROCK) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost >
         p->cost + 1)) {
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost =
        p->cost + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) &&
        (DUN_ARR[p->pos[dim_y] - 1][p->pos[dim_x] - 1] != ROCK) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost >
         p->cost + 1)) {
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost =
        p->cost + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) &&
        (DUN_ARR[p->pos[dim_y] - 1][p->pos[dim_x] + 1] != ROCK) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost >
         p->cost + 1)) {
      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost =
        p->cost + 1;
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x] + 1].hn);
    }
  }

  copy_map(path, map);

}

