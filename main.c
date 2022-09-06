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
    int grid_size = 20;
    int game_table[grid_size][grid_size];
    struct board_elements players = {0, 0, 0, 0};

    board_init(grid_size, game_table, &players);

    while (game_over(&players) == FALSE) {
        print_grid(grid_size, game_table);
        gameplay(grid_size, game_table, &players);
    }

    printf("Simulation over.\n");
    return 0;

};

