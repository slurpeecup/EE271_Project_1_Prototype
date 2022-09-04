// COMPE271 Project 1 Zaid Yasin
#include <stdio.h>

#include "game.h"
#define FALSE 0
#define EXTINCT 0
#define TRUE 1

int main()
{
    int game_table[25][25];
    int grid_size = 25;
    struct board_elements players = {0,0,0,0,0};

    board_init(grid_size, game_table, &players);

    while (game_over(&players)==FALSE)
    {
        gameplay(grid_size,game_table[grid_size][grid_size],&players);
    }
    printf("That's all folks!");
return 0;
};

