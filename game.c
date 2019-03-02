#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <limits.h>

#include "generator.h"
#include "game.h"
#include "coordinates.h"
#include "map.h"


int32_t monster_turn_comp(const void *key, const void *with) {
  return ((monster_t *) key)->turn - ((monster_t *) with)->turn;
}


int check_fastest(game_t* game, int monster){

    int lowest = INT_MAX - 1;
    int move = 0;


    if(game->monsters[monster].characteristics & 0x4) { //tunneling
        if(game->map_tunnel[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x] - 1] < lowest){
            lowest = game->map_tunnel[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x] - 1];
            move = 0;
        }if(game->map_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x] - 1] < lowest){
            lowest = game->map_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x] - 1];
            move = 1;
        }if(game->map_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x]] < lowest){
            lowest = game->map_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x]];
            move = 2;
        }if(game->map_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x] + 1] < lowest){
            lowest = game->map_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x] + 1];
            move = 3;
        }if(game->map_tunnel[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x] + 1] < lowest){
            lowest = game->map_tunnel[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x] + 1];
            move = 4;
        }if(game->map_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x] + 1] < lowest){
            lowest = game->map_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x] + 1];
            move = 5;
        }if(game->map_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x]] < lowest){
            lowest = game->map_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x]];
            move = 6;
        }if(game->map_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x] - 1] < lowest){
            lowest = game->map_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x] - 1];
            move = 7;
        }
        //no legal move.
        if(game->map_tunnel[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x] - 1] == INT_MAX && move == 0){
            move = -1;
        }
    }else{
        if(game->map_no_tunnel[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x] - 1] < lowest){
            lowest = game->map_no_tunnel[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x] - 1];
            move = 0;
        }if(game->map_no_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x] - 1] < lowest){
            lowest = game->map_no_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x] - 1];
            move = 1;
        }if(game->map_no_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x]] < lowest){
            lowest = game->map_no_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x]];
            move = 2;
        }if(game->map_no_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x] + 1] < lowest){
            lowest = game->map_no_tunnel[game->monsters[monster].location[dim_y]-1][game->monsters[monster].location[dim_x] + 1];
            move = 3;
        }if(game->map_no_tunnel[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x] + 1] < lowest){
            lowest = game->map_no_tunnel[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x] + 1];
            move = 4;
        }if(game->map_no_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x] + 1] < lowest){
            lowest = game->map_no_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x] + 1];
            move = 5;
        }if(game->map_no_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x]] < lowest){
            lowest = game->map_no_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x]];
            move = 6;
        }if(game->map_no_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x] - 1] < lowest){
            lowest = game->map_no_tunnel[game->monsters[monster].location[dim_y] + 1][game->monsters[monster].location[dim_x] - 1];
            move = 7;
        }
        //no legal move
        if(game->map_no_tunnel[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x] - 1] == INT_MAX && move == 0){
            move = -1;
        }
    }   

    
    return move;


}


int monster_move(game_t* game, int monster, int direction){

    if(direction == -1){
        return -1;
    }
    
    //my pc movement algorithm that moves right if it can, else left.
    if(game->monsters[monster].pc == 1){
        pair_t check;
        check[dim_x] = game->monsters[monster].location[dim_x] + 1;
        check[dim_y] = game->monsters[monster].location[dim_y];
        if(in_room(game->dungeon, check)) direction = 4;
        else direction = 0;
    }
    

   int xmod, ymod;

    switch(direction){
        case 0: //left
            xmod = -1;
            ymod = 0;
            break;
        case 1: //upleft
            xmod = -1;
            ymod = -1;
            break;
        case 2: //up
            xmod = 0;
            ymod = -1;
            break;
        case 3: //upright
            xmod = 1;
            ymod = -1;
            break;
        case 4: //right
            xmod = 1;
            ymod = 0;
            break;
        case 5: //downright
            xmod = 1;
            ymod = 1;
            break;
        case 6: //down
            xmod = 0;
            ymod = 1;
            break;
        case 7: //downleft
            xmod = -1;
            ymod = 1;
            break;
    }

    int endFlag = 0;
    

    //check if where we just were was in_room, if so print . else print #.

    if(game->monsters[monster].pc == 1){
        if(in_room(game->dungeon, game->monsters[monster].location) == 1){
            game->dungeon->dungeon[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x]] = '.';
        }else{
            game->dungeon->dungeon[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x]] = '#';
        }
        game->monsters[monster].location[dim_x] += xmod;
        game->monsters[monster].location[dim_y] += ymod;
        game->dungeon->dungeon[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x]] = '@';
        game->dungeon->pc[dim_x] += xmod;
        game->dungeon->pc[dim_y] += ymod;
        dijkstra_no_weight(game->dungeon, game->map_no_tunnel);
        dijkstra(game->dungeon, game->map_tunnel);

    } else {
        if(in_room(game->dungeon, game->monsters[monster].location) == 1){
            game->dungeon->dungeon[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x]] = '.';
        }else{
            game->dungeon->dungeon[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x]] = '#';
        }
        
        endFlag = 0;
        game->monsters[monster].location[dim_x] += xmod;
        game->monsters[monster].location[dim_y] += ymod;
        if(game->dungeon->dungeon[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x]] == '@'){
                endFlag = 1;
        }
        if(game->monsters[monster].characteristics & 0x4) {
            game->dungeon->dungeon[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x]] = 'T';
        }else{
            game->dungeon->dungeon[game->monsters[monster].location[dim_y]][game->monsters[monster].location[dim_x]] = 'N';
        }
    }

    if(endFlag == 1){
        print_dungeon(game->dungeon);
        end_game(game, 0);
    }
    
    return 0;
    
}

int print_monsters(game_t* game){

    int i =0;


    for(i=0; i<game->nummon; i++){
        printf("Monster %d is at %d, %d with speed %d. Next turn is %d\n", i+1, game->monsters[i].location[dim_x], game->monsters[i].location[dim_y], game->monsters[i].speed, game->monsters[i].turn);
        printf("With Characteristics %d\n", game->monsters[i].characteristics);
        if(game->monsters[i].characteristics & 0x1){
            printf("Intelligence ");
        }
        if(game->monsters[i].characteristics & 0x2){
            printf("Telepathy ");
        }
        if(game->monsters[i].characteristics & 0x4){
            printf("Tunneling Ability ");
        }
        if(game->monsters[i].characteristics & 0x8){
            printf("Erratic Behavior ");
        }
        printf("\n");
    
    }

     printf("Player is at %d, %d with speed %d. Next turn is %d\n", game->monsters[i].location[dim_x], game->monsters[i].location[dim_y], game->monsters[i].speed, game->monsters[i].turn);

    return 0;

}

int init_game(dungeon_t* dungeon, game_t* game, int nummon){
    game->nummon = nummon;
    game->dungeon = dungeon;
    game->over = 0;
    generate_monsters(game);
    //print_monsters(game);
    dijkstra_no_weight(game->dungeon, game->map_no_tunnel);
    dijkstra(game->dungeon, game->map_tunnel);
    
    //printf("%d Monsters\n", nummon);
    return 0;
}

int generate_monsters(game_t* game){

    heap_init(&game->turnheap, monster_turn_comp, NULL);

    srand(time(NULL));

    game->monsters = malloc(sizeof (monster_t) * (game->nummon + 1));
    
    int i = 0;
    pair_t trypair;

    for(i =0; i<game->nummon; i++){
        game->monsters[i].speed = (rand() % 16) + 5;

        trypair[dim_x] = (rand() % (DUN_WIDTH - 1)) + 1;
        trypair[dim_y] = (rand() % (DUN_HEIGHT - 1)) + 1;
        while(in_room(game->dungeon, trypair) != 1){
            trypair[dim_x] = (rand() % (DUN_WIDTH - 1)) + 1;
            trypair[dim_y] = (rand() % (DUN_HEIGHT - 1)) + 1;
        }
        game->monsters[i].location[dim_x] = trypair[dim_x];
        game->monsters[i].location[dim_y] = trypair[dim_y];
        game->monsters[i].turn = 1000/game->monsters[i].speed;
        game->monsters[i].num = i;
        game->monsters[i].pc = 0;
        game->monsters[i].characteristics = (rand() % 16);
        heap_insert(&game->turnheap, &game->monsters[i]);
        if(game->monsters[i].characteristics & 0x4) {
            game->dungeon->dungeon[game->monsters[i].location[dim_y]][game->monsters[i].location[dim_x]] = 'T';
        }else{
            game->dungeon->dungeon[game->monsters[i].location[dim_y]][game->monsters[i].location[dim_x]] = 'N';
        }
    }

        game->monsters[i].speed = 10;
        game->monsters[i].location[dim_x] = GAMEDUN->pc[dim_x];
        game->monsters[i].location[dim_y] = GAMEDUN->pc[dim_y];
        game->monsters[i].turn = 1000/game->monsters[i].speed;
        game->monsters[i].num = i;
        game->monsters[i].pc = 1;
        heap_insert(&game->turnheap, &game->monsters[i]);



    //print_monsters(game);

    

    return 0;

}

int update_game(game_t* game){

    //int move = 6;
    //printf("PC at %d, %d\n", GAMEDUN->pc[dim_x], GAMEDUN->pc[dim_y]);


    monster_t* current = heap_remove_min(&game->turnheap);
    //printf("WANT TO MOVE %d\n", current->num);
    monster_move(game, current->num, check_fastest(game, current->num));
    current->turn = current->turn + 1000/current->speed;
    //printf("CURRENT MONSTER SPEED IS %d\n", current->speed);
    heap_insert(&game->turnheap, current);

    //print_monsters(game);



    return 0;

}

int end_game(game_t* game, int reason){

    game->over = 1;

    if(reason == 0){
        printf("You lose\n");
    }else if(reason == 1){
        printf("You win\n");
    }else if(reason == 2){
        printf("Out of time\n");
    }

    free(game->monsters);

    return 0;

}
