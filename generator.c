//1.04 Version!!

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "heap.h"
#include "generator.h"
#include "macendian.h"
#include "coordinates.h"
#include "color.h"

const int room_x_min = 4;
const int room_x_max = 10;
const int room_y_min = 3;
const int room_y_max = 9;

int in_room(dungeon_t* dungeon, pair_t location){
        int r = 0;
        int flag = 0;
        for(r=0; r<dungeon->num_rooms; r++){
                    if(location[dim_x]>=DUN_ROOMS[r].x_pos && location[dim_x]<(DUN_ROOMS[r].x_pos + DUN_ROOMS[r].x_len) && 
                    location[dim_y]>=DUN_ROOMS[r].y_pos && location[dim_y]<(DUN_ROOMS[r].y_pos + DUN_ROOMS[r].y_len)){
                        flag = 1;
                    }
                }
                if(flag == 1) return 1;
                else return 0;
    }


void generate_dungeon(dungeon_t* dungeon, stair u_stairs[1], stair d_stairs[1], int nummon)
{
    populate_dungeon(dungeon);
    create_rooms(dungeon);
    
    dungeon->num_rooms = MAX_ROOMS;

    int curr_room;

    for(curr_room=1; curr_room<6; curr_room++){
        connect_rooms(dungeon, curr_room, find_closest_room(dungeon, curr_room));
    }

    //Throws some stairs in there.
    int r1 = rand()%6;
    int r2 = rand()%6;

    while(r1==r2){
        r2 = rand()%6;
    }



    dungeon->down_stair[dim_x] = DUN_ROOMS[r1].x_pos;
    dungeon->down_stair[dim_y] = DUN_ROOMS[r1].y_pos;

    DUN_ARR[DUN_ROOMS[r1].y_pos][DUN_ROOMS[r1].x_pos] = '>';

    dungeon->up_stair[dim_x] = DUN_ROOMS[r2].x_pos;
    dungeon->up_stair[dim_y] = DUN_ROOMS[r2].y_pos;

    DUN_ARR[DUN_ROOMS[r2].y_pos][DUN_ROOMS[r2].x_pos] = '>';

    dungeon->pc[dim_x] = DUN_ROOMS[0].x_pos;
    dungeon->pc[dim_y] = DUN_ROOMS[0].y_pos;

    //this is basically a hack.
    //I used the cetner of the rooms as their position to avoid a bias in the room placement.
    //This reverts that to fix the in_room method.
    int r = 0;
    for(r=0; r<dungeon->num_rooms; r++){
        DUN_ROOMS[r].x_pos = DUN_ROOMS[r].x_pos - DUN_ROOMS[r].x_len/2;
        DUN_ROOMS[r].y_pos = DUN_ROOMS[r].y_pos - DUN_ROOMS[r].y_len/2;
    }

    

    DUN_ARR[dungeon->pc[dim_y]][dungeon->pc[dim_x]] = '@';

    

    make_hard(dungeon);
}

void populate_dungeon(dungeon_t* dungeon)
{
    int i,j=0;

    for (i=0; i<DUN_HEIGHT; i++){
        for(j=0; j<DUN_WIDTH; j++){
            if(i!=0 && j!=0 && i!=20 && j!=79) DUN_ARR[i][j] = ROCK;
            else if(i==20) DUN_ARR[i][j] = BORDER;
            else if(i==0) DUN_ARR[i][j] = BORDER;
            else DUN_ARR[i][j] = BORDER;
        }
    }
}

void print_dungeon(dungeon_t* dungeon)
{
    int i,j=0;

    for (i=0; i<DUN_HEIGHT; i++){
        for(j=0; j<DUN_WIDTH; j++){
            if(DUN_ARR[i][j] == '.') printf(BG_GREY);
            else if (DUN_ARR[i][j] == ' ') printf(BG_DARK_GREY);
            else if (DUN_ARR[i][j] == 'X') printf(BG_BLACK);
            else if (DUN_ARR[i][j] == '#') printf(BG_BROWN);
            else if (DUN_ARR[i][j] == '@') printf(C_BLUE);
            else if (DUN_ARR[i][j] == 'N') printf(C_GREEN);
            else if (DUN_ARR[i][j] == 'T') printf(C_RED);
            fprintf(stdout, "%c", DUN_ARR[i][j]);
            printf("\033[0m");
        }
        fprintf(stdout, "\n");
    }
}

int create_rooms(dungeon_t* dungeon)
{


    DUN_ROOMS = malloc(MAX_ROOMS * sizeof (room_t));

    int room_x_len, room_y_len, x_center, y_center, room_x_pos, room_y_pos;
    int success = 0, i, j = 0;

    while (success < MAX_ROOMS){
        room_x_len = (rand() % (room_x_max-room_x_min+1)) 
                        + room_x_min;
        room_y_len = (rand() % (room_y_max-room_y_min+1)) 
                        + room_y_min;
        x_center = room_x_len/2;
        y_center = room_y_len/2;
        room_x_pos = (rand() % (DUN_WIDTH-(room_x_len-x_center))) 
                        + (x_center+1);
        room_y_pos = (rand() % (DUN_HEIGHT-(room_y_len-y_center))) 
                        + (y_center+1);

        for(i=room_x_pos - x_center - 1; i<room_x_pos + 1 + room_x_len - x_center; i++){
            for(j=room_y_pos - y_center - 1; j<room_y_pos + room_y_len - y_center + 1; j++){
                if(DUN_ARR[j][i] != ROCK) goto jump;
            }
        }

        DUN_ROOMS[success].x_pos = room_x_pos;
        DUN_ROOMS[success].y_pos = room_y_pos;
        DUN_ROOMS[success].x_len = room_x_len;
        DUN_ROOMS[success].y_len = room_y_len;

        for(i=room_x_pos - x_center; i<room_x_pos + room_x_len - x_center; i++){
            for(j=room_y_pos - y_center; j<room_y_pos + room_y_len - y_center; j++){
                DUN_ARR[j][i] = GROUND;
            }
        }

        success++;

        jump:
            ; //I don't actually want to do anything here. I just want to be here.

    }

    return 0;
}

void print_room_array(dungeon_t* dungeon)
{
    int i = 0;

    for(i=0; i<6; i++){
        printf("Size: X=%d Y=%d. Position: X=%d Y=%d\n",
        DUN_ROOMS[i].x_len, DUN_ROOMS[i].y_len, DUN_ROOMS[i].x_pos, DUN_ROOMS[i].y_pos);
    }
}



int connect_rooms(dungeon_t* dungeon, int r1, int r2)
{
    int x_dist = DUN_ROOMS[r1].x_pos - DUN_ROOMS[r2].x_pos;
    int y_dist = DUN_ROOMS[r1].y_pos - DUN_ROOMS[r2].y_pos;
    int curr_x = DUN_ROOMS[r1].x_pos;
    int curr_y = DUN_ROOMS[r1].y_pos;

    int rando;

    while(x_dist!=0 || y_dist != 0){
        rando = rand();
        if((rando % 2) == 0 && x_dist!=0){
            if(x_dist > 0){
                curr_x--;
            }else{
                curr_x++;
            }
        }else if((rando % 2) == 0 && y_dist!=0){
            if(y_dist > 0){
                curr_y--;
            }else{
                curr_y++;
            }
        }else if((rando % 2) == 1 && y_dist!=0){
            if(y_dist > 0){
                curr_y--;
            }else{
                curr_y++;
            }
        }else if((rando % 2) == 1 && x_dist!=0){
            if(x_dist > 0){
                curr_x--;
            }else{
                curr_x++;
            }
        }
        x_dist = curr_x - DUN_ROOMS[r2].x_pos;
        y_dist = curr_y - DUN_ROOMS[r2].y_pos;
        if(DUN_ARR[curr_y][curr_x] == ROCK) DUN_ARR[curr_y][curr_x] = CORRIDOR;
    }
    return 0;
}

int find_closest_room(dungeon_t* dungeon, int curr_room)
{
    if(curr_room == 0){
        return -1;
    }else if(curr_room == 1){
        return 0;
    }

    int i;
    int new_dist;
    int closest = 0;
    int smallest_dist = pow(pow(DUN_ROOMS[0].x_pos - DUN_ROOMS[curr_room].x_pos, 2) + 
                            pow(DUN_ROOMS[0].y_pos - DUN_ROOMS[curr_room].y_pos, 2), .5);

    for(i=1; i<curr_room; i++){
        new_dist = pow(pow(DUN_ROOMS[i].x_pos - DUN_ROOMS[curr_room].x_pos, 2) + 
                        pow(DUN_ROOMS[i].y_pos - DUN_ROOMS[curr_room].y_pos, 2), .5);
        if(new_dist < smallest_dist){
            smallest_dist = new_dist;
            closest = i;
        }
    }
    return closest;
}

void make_hard(dungeon_t* dungeon)
{
    int i,j=0;

    for (i=0; i<DUN_HEIGHT; i++){
        for(j=0; j<DUN_WIDTH; j++){
            switch(DUN_ARR[i][j]){
                case ' ':
                    DUN_HARD[i][j] = 254;
                    break;
                case '.':
                    DUN_HARD[i][j] = 0;
                    break;
                case 'X':
                    DUN_HARD[i][j] = 255;
                    break;
                case '#':
                    DUN_HARD[i][j] = 0;
                    break;
            }

        }
    }
    
}

uint32_t file_size(){
    uint32_t ret = 32;
    return ret; 
}

int loader(dungeon_t* dungeon)
{

    char* home = getenv("HOME");

    char* path;

    path = malloc(strlen(home) + strlen(FILE_NAME) + 1);

    strcpy(path, home);
    strcat(path, FILE_NAME);
    printf("%s\n", path);
    

    FILE* f;

    if(!(f = fopen(path, "r"))){
        fprintf(stderr, "Fail to open\n");
        return -1;
    }

    free(path);

    char file_type[13];
    uint32_t file_version; 
    uint32_t file_size; 
    uint8_t x_pos;
    uint8_t y_pos;
    uint16_t room_num;
    uint8_t room_x_pos;
    uint8_t room_y_pos;
    uint8_t room_x_len;
    uint8_t room_y_len;
    uint16_t up_stair;
    uint16_t down_stair;
    uint8_t stair_x_pos;
    uint8_t stair_y_pos;


    //FILE TYPE
    if(fread(file_type, 12, 1, f) != 1){
        fprintf(stderr, "Fail to read ft\n");
        return -2;
    }
    file_type[12] = '\0';
    //printf("%s\n", file_type);

    //FILE VERSION
    if(fread(&file_version, sizeof(file_version), 1, f) != 1){
        fprintf(stderr, "Fail to read fv\n");
        return -2;
    }
    file_version = be32toh(file_version);
    //printf("File Version: %d\n", file_version);

    //FILE SIZE
    if(fread(&file_size, sizeof(file_size), 1, f) != 1){
        fprintf(stderr, "Fail to read fs\n");
        return -2;
    }
    file_size = be32toh(file_size);
    //printf("File Size: %d\n", file_size);

    //X POS
    if(fread(&x_pos, sizeof(x_pos), 1, f) != 1){
        fprintf(stderr, "Fail to read xp\n");
        return -2;
    }
    //printf("X pos: %d\n", x_pos);

    //Y POS
    if(fread(&y_pos, sizeof(y_pos), 1, f) != 1){
        fprintf(stderr, "Fail to read yp\n");
        return -2;
    }
    //printf("Y pos: %d\n", y_pos);
    
    //HARDNESS
    int i = 0, j = 0;
    for(i = 0; i < DUN_HEIGHT; i++){
        for(j=0; j<DUN_WIDTH; j++){
            if(fread(&DUN_HARD[i][j], 1, 1, f) != 1){
                fprintf(stderr, "Fail to read hard\n");
                return -2;
            }
        }
    }

    //ROOM NUM
    if(fread(&room_num, sizeof(room_num), 1, f) != 1){
        fprintf(stderr, "Fail to read rn\n");
        return -2;
    }
    room_num = htobe16(room_num);

    dungeon->num_rooms = room_num;

    DUN_ROOMS = malloc(dungeon->num_rooms * sizeof (room_t));


    //ROOMS
    //room_t rooms_temp[dungeon->num_rooms];
    //THESE CURRENTLY USE STANDARD X AND Y WHICH IS A TOP LEFT.
    for(i = 0; i<dungeon->num_rooms; i++){
        if(fread(&room_x_pos, sizeof(room_x_pos), 1, f) != 1){
            fprintf(stderr, "Fail to read rxp\n");
            return -2;
        }
        DUN_ROOMS[i].x_pos = room_x_pos;
        if(fread(&room_y_pos, sizeof(room_y_pos), 1, f) != 1){
            fprintf(stderr, "Fail to read ryp\n");
            return -2;
        }
        DUN_ROOMS[i].y_pos = room_y_pos;
        if(fread(&room_x_len, sizeof(room_x_len), 1, f) != 1){
            fprintf(stderr, "Fail to read rxl\n");
            return -2;
        }
        DUN_ROOMS[i].x_len = room_x_len;
        if(fread(&room_y_len, sizeof(room_y_len), 1, f) != 1){
            fprintf(stderr, "Fail to read ryl\n");
            return -2;
        }
        DUN_ROOMS[i].y_len = room_y_len;
    }

    
    
    //UP STAIR
    if(fread(&up_stair, sizeof(up_stair), 1, f) != 1){
        fprintf(stderr, "Fail to read ustairs\n");
        return -2;
    }
    up_stair = be16toh(up_stair);
    //printf("Num up stairs:%d\n", up_stair);
    stair up_stairs_temp[up_stair];
    for(i = 0; i<up_stair; i++){
        if(fread(&stair_x_pos, sizeof(stair_x_pos), 1, f) != 1){
            fprintf(stderr, "Fail to read sxp\n");
            return -2;
        }
        up_stairs_temp[i].x_pos = stair_x_pos;
        if(fread(&stair_y_pos, sizeof(stair_y_pos), 1, f) != 1){
            fprintf(stderr, "Fail to read syp\n");
            return -2;
        }
        up_stairs_temp[i].y_pos = stair_y_pos;
    }

    dungeon->up_stair[dim_x] = up_stairs_temp[0].x_pos;
    dungeon->up_stair[dim_y] = up_stairs_temp[0].y_pos;

    //DOWN STAIR
    if(fread(&down_stair, sizeof(down_stair), 1, f) != 1){
        fprintf(stderr, "Fail to read dstairs\n");
        return -2;
    }
    down_stair = be16toh(down_stair);
    //printf("Num down stairs:%d\n", down_stair);
    stair down_stairs_temp[down_stair];
    for(i = 0; i<down_stair; i++){
        if(fread(&stair_x_pos, sizeof(stair_x_pos), 1, f) != 1){
            fprintf(stderr, "Fail to read sxp\n");
            return -2;
        }
        down_stairs_temp[i].x_pos = stair_x_pos;
        if(fread(&stair_y_pos, sizeof(stair_y_pos), 1, f) != 1){
            fprintf(stderr, "Fail to read syp\n");
            return -2;
        }
        down_stairs_temp[i].y_pos = stair_y_pos;
    }

    dungeon->down_stair[dim_x] = down_stairs_temp[0].x_pos;
    dungeon->down_stair[dim_y] = down_stairs_temp[0].y_pos;

    pair_t location;

    for (i=0; i<DUN_HEIGHT; i++){
        for(j=0; j<DUN_WIDTH; j++){
            if(DUN_HARD[i][j] == 255) DUN_ARR[i][j] = 'X';
            else if (DUN_HARD[i][j] == 0){
                location[dim_x] = j;
                location[dim_y] = i;
                if(in_room(dungeon, location) == 1) DUN_ARR[i][j] = '.';
                else DUN_ARR[i][j] = '#';
            } 
            else DUN_ARR[i][j] = ' ';
        }
    }

    

    DUN_ARR[y_pos][x_pos] = '@';

    dungeon->pc[dim_x] = x_pos;
    dungeon->pc[dim_y] = y_pos;

    DUN_ARR[dungeon->up_stair[dim_y]][dungeon->up_stair[dim_x]] = '<';
    DUN_ARR[dungeon->down_stair[dim_y]][dungeon->down_stair[dim_x]] = '>';
    

    return 0;

}

int saver(dungeon_t* dungeon, stair u_stairs[1], stair d_stairs[1])
{
    char* home = getenv("HOME");

    char* path;
    uint32_t file_version = htobe32(FILE_VERISON); 
    uint32_t file_size_temp = file_size();
    uint32_t file_size = htobe32(file_size_temp); //Getting weird errors if I didn't temp this.
    uint8_t x_pos = 0;
    uint8_t y_pos = 0;
    uint16_t room_num = htobe16(MAX_ROOMS);
    uint8_t room_x_pos = 0;
    uint8_t room_y_pos = 0;
    uint8_t room_x_len = 0;
    uint8_t room_y_len = 0;
    uint16_t up_stairs = htobe16(1);
    uint16_t down_stairs = htobe16(1);
    uint8_t stair_x_pos = 0;
    uint8_t stair_y_pos = 0;

    //printf("ROOMS: %lu", sizeof (DUN_ROOMS));

    path = malloc(strlen(home) + strlen(FILE_NAME) + 1);

    strcpy(path, home);
    strcat(path, FILE_NAME);
    //printf("%s\n", path);

    FILE* f;

    if(!(f = fopen(path, "w"))){
        fprintf(stderr, "Fail to open\n");
        return -1;
    }

    free(path);

    char* file_type = "RLG327-S2019"; 
   
    if(fwrite(file_type, strlen(file_type), 1, f) != 1){
        fprintf(stderr, "Fail to write ft\n");
        return -2;
    }

    //FILE VERSION
    if(fwrite(&file_version, sizeof(file_version), 1, f) != 1){
        fprintf(stderr, "Fail to write fv\n");
        return -2;
    }

    //FILE SIZE
    if(fwrite(&file_size, sizeof(file_size), 1, f) != 1){
        fprintf(stderr, "Fail to write fs\n");
        return -2;
    }

    //X POS
    x_pos = dungeon->pc[dim_x];
    if(fwrite(&x_pos, sizeof(x_pos), 1, f) != 1){
        fprintf(stderr, "Fail to write xp\n");
        return -2;
    }

    //Y POS
    y_pos = dungeon->pc[dim_y];
    if(fwrite(&y_pos, sizeof(y_pos), 1, f) != 1){
        fprintf(stderr, "Fail to write yp\n");
        return -2;
    }

    //HARDNESS
    int i = 0, j = 0;
    for(i = 0; i < DUN_HEIGHT; i++){
        for(j=0; j < DUN_WIDTH; j++){
            if(fwrite(&DUN_HARD[i][j], 1, 1, f) != 1){
                fprintf(stderr, "Fail to write hard\n");
                return -2;
            }
        }
    }

    //ROOM NUM
    if(fwrite(&room_num, sizeof(room_num), 1, f) != 1){
        fprintf(stderr, "Fail to write rn\n");
        return -2;
    }

    //ROOM ARRAY THESE CURRENTLY USE MY X AND Y WHICH IS A PSEUDO CENTROID. (FIXED)
    for(i=0; i<dungeon->num_rooms; i++){
        room_x_pos = 0;
        room_y_pos = 0;
        room_x_len = 0;
        room_y_len = 0;


        room_x_pos = DUN_ROOMS[i].x_pos; // - DUN_ROOMS[i].x_len/2;
        if(fwrite(&room_x_pos, sizeof(room_x_pos), 1, f) != 1){
            fprintf(stderr, "Fail to write rxp\n");
            return -2;
        }

        room_y_pos = DUN_ROOMS[i].y_pos; // - DUN_ROOMS[i].y_len/2;
        if(fwrite(&room_y_pos, sizeof(room_y_pos), 1, f) != 1){
            fprintf(stderr, "Fail to write ryp\n");
            return -2;
        }

        room_x_len = DUN_ROOMS[i].x_len;
        if(fwrite(&room_x_len, sizeof(room_x_len), 1, f) != 1){
            fprintf(stderr, "Fail to write rxl\n");
            return -2;
        }

        room_y_len = DUN_ROOMS[i].y_len;
        if(fwrite(&room_y_len, sizeof(room_y_len), 1, f) != 1){
            fprintf(stderr, "Fail to write ryl\n");
            return -2;
        }


    }

    //STAIR COUNT
    if(fwrite(&up_stairs, sizeof(up_stairs), 1, f) != 1){
            fprintf(stderr, "Fail to write stair\n");
            return -2;
        }

    stair_x_pos = dungeon->up_stair[dim_x];
    stair_y_pos = dungeon->up_stair[dim_y];

    if(fwrite(&stair_x_pos, sizeof(stair_x_pos), 1, f) != 1){
            fprintf(stderr, "Fail to write stair\n");
            return -2;
    }

    if(fwrite(&stair_y_pos, sizeof(stair_y_pos), 1, f) != 1){
            fprintf(stderr, "Fail to write stair\n");
            return -2;
    }

    if(fwrite(&down_stairs, sizeof(down_stairs), 1, f) != 1){
            fprintf(stderr, "Fail to write stair\n");
            return -2;
        }

    stair_x_pos = dungeon->down_stair[dim_x];
    stair_y_pos = dungeon->down_stair[dim_y];

    if(fwrite(&stair_x_pos, sizeof(stair_x_pos), 1, f) != 1){
            fprintf(stderr, "Fail to write stair\n");
            return -2;
    }

    if(fwrite(&stair_y_pos, sizeof(stair_y_pos), 1, f) != 1){
            fprintf(stderr, "Fail to write stair\n");
            return -2;
    }


    fclose(f);

    return 0;
}