// COMPE271 Project 1 Zaid Yasin
#include <stdio.h>

#include "game.h"
#define FALSE 0
#define EXTINCT 0
#define TRUE 1

int main()
{
    int game_table[25][25];
    struct board_elements players = {0,0,0,0,0};

    board_init(25, game_table, &players);

    while (game_over(&players)==FALSE)
    {
        gameplay(25,game_table[25][25],&players);
    }
    printf("That's all folks!");
return 0;
};

