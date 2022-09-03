//
// Created by z434 on 9/2/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

#define FALSE 0
#define EXTINCT 0
#define TRUE 1
#define STAY ray[8]
#define FIGHT 0x32

#define ROW_BACKWARD ((row-1 +25)%25)
#define COLUMN_BACKWARD ((column-1+25)%25)
#define ROW_FORWARD ((row+1 +25)%25)
#define COLUMN_FORWARD ((column+1+25)%25)

#define CURRENT_PLAYER game_table[row][column]

//intent is to initialize the game board in a 2d grid
void board_init(int grid_size, int game_table[grid_size][grid_size],struct board_elements* players)// we'll key this back in if we decide to play w/ dynamic alloc:, int m, int n)
{
    printf("in board init\n");
    srand(time(NULL)); //seeding RNG

    for (int row = 0; row< grid_size; row++)
    {
        for (int column = 0; column < grid_size; column++) {

            enum board_contents fill_space = rand() % 5;

            if (board_content_regulator( players, fill_space) == TRUE)
            {
                game_table[row][column] = fill_space;

                printf("%d loop, element %d, value %d\n", row, column,fill_space);
            }
            else
            {
                printf("%d loop, element %d\n", row, column);
                game_table[row][column] = empty;
            }
        }
    }
    printf("out board init\n");
}


// function to ensure that the board is initialized in a way that is mostly playable.
int board_content_regulator (struct board_elements* players, enum board_contents fill_space) {
    printf("in board reg\n");
    switch (fill_space)
    {
        case empty:
           return TRUE;
        case stone:
            if (players->stone_count >= 51)
                return FALSE;
                players->stone_count++;
        case food:
            if ( players->food_count >= 51)
                return FALSE;
            players->food_count ++;
        case rabbit:
            if ( players->rabbit_count >=25)
                return FALSE;
            players->rabbit_count++;
        case lion:
            if ( players->lion_count >= 25)
                return FALSE;
            players->lion_count++;
            break;
        default: return FALSE;
    }
    printf("out board reg\n");
    return TRUE;
}

int game_over(struct board_elements* players)
{
    if ((&players->rabbit_count) == EXTINCT || (&players->lion_count) == EXTINCT)
    {return TRUE;}
    return FALSE;
}

void gameplay(int grid_size, int game_table[grid_size][grid_size],struct board_elements* players)
{
    printf("in game play\n");
    for (int row = 0; row< grid_size; row++)
    {
        printf("in loop 1\n");
        for (int column = 0; column < grid_size; column++)
        {
                printf("in loop 2\n");
                int row_forward = ROW_FORWARD, row_backward = ROW_BACKWARD,
                column_forward = COLUMN_FORWARD, column_backward = COLUMN_BACKWARD;
                int ray[9] =
                {
                    &game_table[row_backward][column_backward],
                    &game_table[row_backward][column],
                    &game_table[row_backward][column_forward],
                    &game_table[row][column_backward],
                    &game_table[row][column_forward],
                    &game_table[row_forward][column_backward],
                    &game_table[row_forward][column],
                    &game_table[row_forward][column_forward],
                    game_table[row][column]
                };
                printf("in ray init\n");
                if (game_table[row][column] == rabbit) {
                    int decision;
                    decision = play_rabbit(game_table, &players, ray);
                    if (decision >= FIGHT && decision < FIGHT + 8) {
                        int survival = rand() % 70;
                        if (survival > 50) {
                            ray[decision - FIGHT] = rabbit;
                        } else
                            players->rabbit_count--;
                    }//fight
                    decision = rabbit;
                    CURRENT_PLAYER = empty;
                }

                if (*CURRENT_PLAYER == lion) {
                    int decision;
                    decision = play_lion(game_table, &players, ray);
                    if (decision >= FIGHT && decision < FIGHT + 8) {
                        int survival = rand() % 80;
                        if (survival > 20) {
                            ray[decision - FIGHT] = lion;
                        } else
                            players->lion_count--;
                    }//fight
                    decision = lion;
                    CURRENT_PLAYER = empty;
                }
        }
    }
    printf("out game play\n");
}

int play_rabbit(int game_table[25][25],struct board_elements* players, int* ray)
{
    for (int i = 0; i<7;i++)
    {
        switch (ray[i])
        {
            case food:
                return ray[i]; //goto
            case rabbit:
                for (int row = 0; row< 25; row++)
                {
                    int found =0;

                    for (int column = 0; column < 25; column++)
                    {
                        if (game_table[row][column]==empty)
                        {
                            game_table[row][column] = rabbit;
                            found = 1;
                            players->rabbit_count++;
                            break;
                        }
                    }
                    if (found == 1)
                        break;
                }
                return 0; // make sure that both rabbits stay in the same place
            case empty:
                return ray[i];//goto
            case lion:
                return FIGHT+i;
            case stone:
                return STAY;
            default:
                return ray[rand()%7];
        }
    }
}

int play_lion(int game_table[25][25],struct board_elements* players, int* ray)
        {
            for (int i = 0; i<7;i++)
            {
                switch (ray[i])
                {
                    case rabbit:
                        return FIGHT+i;
                    case lion:
                        for (int row = 0; row< 25; row++)
                        {
                            int found =0;

                            for (int column = 0; column < 25; column++)
                            {
                                if (game_table[row][column]==empty)
                                {
                                    game_table[row][column] = rabbit;
                                    found = 1;
                                    players->rabbit_count++;
                                    break;
                                }
                            }
                            if (found == 1)
                                break;
                        }
                        return 0; // make sure that both rabbits stay in the same place
                    case food:
                        return ray[i];//goto
                    case empty:
                        return ray[i];//goto
                    case stone:
                        return STAY;
                    default:
                        return ray[rand()%7];
                }
            }

        }