#include "generator.h"
#include "map.h"
#include "game.h"

#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char** argv)
{
     
    game_t game;
    dungeon_t dungeon;
    int seed = time(NULL);
    stair up_stairs[1];
    stair down_stairs[1];
    int nummon = 10;


    if(argc != 1 && argc != 2 && argc != 3){
        fprintf(stderr, "Usage: %s <optional seed, --save, --load>\n", argv[0]);
        return -1;
    }
    if(argc == 2){
        if (!strcmp(argv[1], "--save")) {
            srand(seed);
            fprintf(stdout, "Using seed %d\n", seed);
            generate_dungeon(&dungeon, up_stairs, down_stairs, nummon);
            print_dungeon(&dungeon);
            print_room_array(&dungeon);
            saver(&dungeon, up_stairs, down_stairs);
            return 0;
            //dijkstra(&dungeon);
            //dijkstra_no_weight(&dungeon);
        } else if (!strcmp(argv[1], "--load")) {
            loader(&dungeon);
            print_dungeon(&dungeon);
            print_room_array(&dungeon);
            return 0;
            //dijkstra(&dungeon);
            //dijkstra_no_weight(&dungeon);
        } else {
            char* p;
            seed = strtol(argv[1], &p, 10);
            srand(seed);
            fprintf(stdout, "Using seed %d\n", seed);
            generate_dungeon(&dungeon, up_stairs, down_stairs, nummon);
            //print_dungeon(&dungeon);
            saver(&dungeon, up_stairs, down_stairs);
        }
    }else if(argc == 3){
        if ((!strcmp(argv[1], "--save") && !strcmp(argv[2], "--load")) || (!strcmp(argv[1], "--load") && !strcmp(argv[2], "--save"))){
            loader(&dungeon);
            //print_dungeon(&dungeon);
            //dijkstra(&dungeon);
            //dijkstra_no_weight(&dungeon);
            saver(&dungeon, up_stairs, down_stairs);
        } if (!strcmp(argv[1], "--nummon")) {
            srand(seed);
            fprintf(stdout, "Using seed %d\n", seed);
            generate_dungeon(&dungeon, up_stairs, down_stairs, atoi (argv[2]));
            //saver(&dungeon, up_stairs, down_stairs);
            //loader(&dungeon);
            nummon = atoi (argv[2]);
            //print_dungeon(&dungeon);
            //dijkstra(&dungeon);
            //dijkstra_no_weight(&dungeon);
        }
    } else {
        srand(seed);
        generate_dungeon(&dungeon, up_stairs, down_stairs, nummon);
        //print_dungeon(&dungeon);
        //dijkstra(&dungeon);
        //dijkstra_no_weight(&dungeon);
    }

    int turns = 0;

    init_game(&dungeon, &game, nummon);

    while(game.over != 1){
        usleep(250000);
        printf("Turn %d\n", turns);
        print_dungeon(&dungeon);
        turns++;
        update_game(&game);
    }


    return 0;

}