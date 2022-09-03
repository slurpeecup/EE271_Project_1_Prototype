//
// Created by z434 on 9/2/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

#define FALSE 0
#define TRUE 1

//intent is to initialize the game board in a 2d grid
void board_init(int** game_table[])// we'll key this back in if we decide to play w/ dynamic alloc:, int m, int n)
{
    srand(time(NULL)); //seeding RNG

    // keeping track of initialized tiles
    int empty_count = 0, stone_count = 0,
    food_count = 0, rabbit_count = 0, lion_count = 0;

    for (int row = 0; row< 25; row++) {
        for (int column = 0; column < 25; column++) {
            //    printf("%d loop, element %d\n", row, column);
            enum board_contents fill_space = rand() % 5;
            //   printf("%d loop, element %d\n", row, column);
            if (board_content_regulator(&empty_count, &stone_count,
                                        &food_count, &rabbit_count,
                                        &lion_count, fill_space) == TRUE) {
                //    printf("%d loop, element %d\n", row, column);
                printf("%d\n", fill_space);
                //   game_table[row][column] = fill_space;
                //   printf("%d loop, element %d\n", row, column);
                //  } else
                //{
                //  printf("%d loop, element %d\n", row, column);
                game_table[row][column] = empty;
                printf("value within game %d\n", game_table[row][column]);

                }
            }

        }
    }


// function to ensure that the board is initialized in a way that is mostly playable.
int board_content_regulator (int* empty_count, int* stone_count,
         int* food_count, int* rabbit_count, int* lion_count, enum board_contents fill_space) {
    switch (fill_space)
    {
        case empty:
            *empty_count ++;
        case stone:
            if (*stone_count >= 51)
                return FALSE;
                *stone_count++;
        case food:
            if (*food_count >= 51)
                return FALSE;
                *stone_count ++;
        case rabbit:
            if (*rabbit_count >=25)
                return FALSE;
                *rabbit_count++;
        case lion:
            if (*lion_count >= 25)
                return FALSE;
            *rabbit_count++;
            break;
        default: return FALSE;
    }
    return TRUE;
}
