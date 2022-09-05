// COMPE271 Project 1 Zaid Yasin
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#define FALSE 0
#define EXTINCT 0
#define TRUE 1

int main()
{
    srand(time(NULL));
    int game_table[20][20];
    int grid_size = 20;
    struct board_elements players = {0,0,0,0};

    board_init(grid_size, game_table, &players);


 printf("bruh\n");

   do
   {
       gameplay(grid_size, game_table, &players);

   } while (game_over(&players)==FALSE);

    printf("That's all folks!");
return 0;
};

