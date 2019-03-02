#ifndef GAME_H
# define GAME_H

#define GAMEDUN game->dungeon

typedef struct monster{
    pair_t location;
    int characteristics;
    int speed;
    int turn;
    int num;
    int pc;


}monster_t;

typedef struct heap_monster{
    int turn;
    int num; //the corresponding index of the actual monster
}heap_monster_t;

typedef struct game{
    int over;
    dungeon_t* dungeon;
    heap_t turnheap;
    monster_t* monsters;
    int nummon;
    int map_tunnel[DUN_HEIGHT][DUN_WIDTH];
    int map_no_tunnel[DUN_HEIGHT][DUN_WIDTH];
}game_t;



int update_game(game_t* game);
int init_game(dungeon_t* dungeon, game_t* game, int nummon);
int generate_monsters(game_t* game);
int end_game(game_t* game, int reason);  



#endif