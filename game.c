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

#define ROW_BACKWARD ((row-1 +20)%20)
#define COLUMN_BACKWARD ((column-1+20)%20)
#define ROW_FORWARD ((row+1 +20)%20)
#define COLUMN_FORWARD ((column+1+20)%20)

#define CURRENT_PLAYER game_table[row][column]

//intent is to initialize the game board in a 2d grid
void board_init(int grid_size, int game_table[grid_size][grid_size],struct board_elements* players)
{
    srand(time(NULL)); //seeding RNG

    for (int row = 0; row< grid_size; row++)
    {
        for (int column = 0; column < grid_size; column++) {

            enum board_contents fill_space = rand() % 14;

            if (board_content_regulator( players, &fill_space) == TRUE)
            {
                CURRENT_PLAYER = fill_space;
            }
            else
            {
                CURRENT_PLAYER = empty;
            }
        }
    }
}


// function to ensure that the board is initialized in a way that is mostly playable.
int board_content_regulator (struct board_elements* players, enum board_contents* fill_space)
        {
    if (*fill_space <10)
        *fill_space = empty;
    switch (*fill_space)
    {
        case empty:
           return TRUE;
        case stone:
            if (players->stone_count >= 51)
            {
                return FALSE;
            }
                players->stone_count++;
            break;
        case food:
            if ( players->food_count >= 51)
            {
                return FALSE;
            }
            players->food_count ++;
            break;
        case rabbit:
            if ( players->rabbit_count >=25)
            {
                return FALSE;
            }
            players->rabbit_count++;
            break;
        case lion:
            if ( players->lion_count >= 25)
            {
                return FALSE;
            }
            players->lion_count++;
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

int game_over(struct board_elements* players)
{
    if ((players->rabbit_count) == EXTINCT || (players->lion_count) == EXTINCT)
    {return TRUE;}
    return FALSE;
}
/*
void gameplay(int grid_size, int game_table[grid_size][grid_size],struct board_elements* players)
{
    for (int row = 0; row< grid_size; row++) {
        for (int column = 0; column < grid_size; column++) {
            printf("%d loop, element %d, value %d\n", row, column,game_table[row][column]);
        }
    }

}
*/

void gameplay(int grid_size, int game_table[grid_size][grid_size],struct board_elements* players) {

    for (int row = 0; row < grid_size; row++) {
        for (int column = 0; column < grid_size; column++)
        {
            int *ray[] =
                    {
                            &game_table[ROW_BACKWARD][COLUMN_BACKWARD],
                            &game_table[ROW_BACKWARD][column],
                            &game_table[ROW_BACKWARD][COLUMN_FORWARD],
                            &game_table[row][COLUMN_BACKWARD],
                            &game_table[row][COLUMN_FORWARD],
                            &game_table[ROW_FORWARD][COLUMN_BACKWARD],
                            &game_table[ROW_FORWARD][column],
                            &game_table[ROW_FORWARD][COLUMN_FORWARD],
                            &CURRENT_PLAYER
                    };

                if ((game_table[row][column]) == rabbit) {

                    //dying from hunger
                    int death_from_hunger = rand() % 100;
                    if (death_from_hunger > 80)
                    {
                        game_table[0][0] = empty;
                        players->rabbit_count--;
                        printf("Rabbit died from hunger\n");
                        break;
                    }


                    int decision;
                    decision = play_rabbit(grid_size,game_table, players, ray);

                    if (decision >= FIGHT && decision < FIGHT + 8) {
                        int survival = rand() % 70;
                        if (survival > 50) {
                            *ray[decision - FIGHT] = rabbit;
                            printf("Rabbit won the fight and moved to %d\n", *ray[decision - FIGHT]);
                        } else
                            players->rabbit_count--;
                        printf("Rabbit lost the fight\n");
                    }//fight
                    decision = rabbit;
                    game_table[row][column] = empty;
                }

                if (game_table[row][column] == lion) {

                    //dying from hunger
                    int death_from_hunger = rand() % 100;
                    if (death_from_hunger > 80)
                    {
                        game_table[0][0] = empty;
                        players->lion_count--;
                        printf("Lion died from hunger\n");
                        break;
                    }

                    int decision;
                    decision = play_lion(grid_size,game_table, players, ray);

                    if (decision >= FIGHT && decision < FIGHT + 8)
                    {
                        int survival = rand() % 80;
                        if (survival > 20)
                        {
                            *ray[decision - FIGHT] = lion;
                            printf("Lion won the fight and moved to %d\n", *ray[decision - FIGHT]);
                        } else
                            players->lion_count--;
                        printf("Lion lost the fight\n");
                    }
                    //fight
                    decision = lion;
                    game_table[row][column] = empty;
                }
                printf("Rabbit count is currently: %d\n"
                       " Lion count is currently: %d\n",
                       players->rabbit_count,
                       players->lion_count);
        }
    }
}
int play_rabbit(int grid_size, int game_table[grid_size][grid_size],
                struct board_elements* players, int** ray)
{
    for (int i = 0; i<7;i++)
    {
        switch (*ray[i])
        {
            case food:
                printf("Rabbit ate food at %d\n", *ray[i]);
                return *ray[i]; //goto
            case rabbit:
                printf("Rabbit found a mate!\n");
                for (int row = 0; row< 20; row++)
                {
                    int found =0;

                    for (int column = 0; column < 20; column++)
                    {
                        if (CURRENT_PLAYER==empty)
                        {
                            CURRENT_PLAYER = rabbit;
                            found = 1;
                            printf("New baby rabbit\n");
                            players->rabbit_count++;
                            break;
                        }
                    }
                    if (found == 1)
                        break;
                }
                return 0; // make sure that both rabbits stay in the same place
            case empty:
                printf("Rabbit moved to %d\n", *ray[i]);
                return *ray[i];//goto
            case lion:
                printf("Rabbit chose to fight\n");
                return FIGHT+i;
            case stone:
                printf("Rabbit stayed at %d\n", *ray[i]);
                return *STAY;
        }

    }
    return *ray[rand()%7];
}

int play_lion(int grid_size, int game_table[grid_size][grid_size],
              struct board_elements* players, int** ray)
        {
            for (int i = 0; i<7;i++)
            {
                switch (*ray[i])
                {
                    case rabbit:
                        printf("Lion chose to fight\n");
                        return FIGHT+i;
                    case lion:
                        printf("Lion found a mate\n");
                        for (int row = 0; row< 20; row++)
                        {
                            int found =0;

                            for (int column = 0; column < 20; column++)
                            {
                                if (game_table[row][column]==empty)
                                {
                                    game_table[row][column] = lion;
                                    found = 1;
                                    printf("New baby lion\n");
                                    players->lion_count++;
                                    break;
                                }
                            }
                            if (found == 1)
                                break;
                        }
                        return 0; // make sure that both rabbits stay in the same place
                    case food:
                        printf("Lion ate food at %d\n", *ray[i]);
                        return *ray[i];//goto
                    case empty:
                        printf("Lion moved to %d\n", *ray[i]);
                        return *ray[i];//goto
                    case stone:
                        printf("Lion stayed at %d\n", *ray[i]);
                        return *STAY;
                }
            }
            return *ray[rand()%7];
        }