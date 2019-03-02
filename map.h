#ifndef MAP_H
# define MAP_H

typedef struct dungeon dungeon_t;

void dijkstra_no_weight(dungeon_t *d, int map[DUN_HEIGHT][DUN_WIDTH]);
void dijkstra(dungeon_t *d, int map[DUN_HEIGHT][DUN_WIDTH]);

#endif
