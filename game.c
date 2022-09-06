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
#define STAY 0x16
#define FIGHT 0x32
#define MATE 0x48

#define ROW_BACKWARD ((row-1 +grid_size)%grid_size)
#define COLUMN_BACKWARD ((column-1+grid_size)%grid_size)
#define ROW_FORWARD ((row+1 +grid_size)%grid_size)
#define COLUMN_FORWARD ((column+1+grid_size)%grid_size)

#define CURRENT_PLAYER game_table[row][column]

//intent is to initialize the game board in a 2d grid
void board_init(int grid_size, int game_table[grid_size][grid_size], struct board_elements *players) {
    srand(time(NULL)); //seeding RNG

    for (int row = 0; row < grid_size; row++) {
        for (int column = 0; column < grid_size; column++) {

            enum board_contents fill_space = rand() % 14;

            if (board_content_regulator(players, &fill_space) == TRUE) {
                game_table[row][column] = fill_space;
            } else {
                game_table[row][column] = empty;
            }
        }
    }
}


// function to ensure that the board is initialized in a way that is mostly playable.
int board_content_regulator(struct board_elements *players, enum board_contents *fill_space) {
    //  printf("in board content regulator\n");
    if (*fill_space < 10)
        *fill_space = empty;
    switch (*fill_space) {
        case empty:
            return TRUE;
        case stone:
            if (players->stone_count >= 51) {
                return FALSE;
            }
            players->stone_count++;
            break;
        case food:
            if (players->food_count >= 51) {
                return FALSE;
            }
            players->food_count++;
            break;
        case rabbit:
            if (players->rabbit_count >= 25) {
                return FALSE;
            }
            players->rabbit_count++;
            break;
        case lion:
            if (players->lion_count >= 25) {
                return FALSE;
            }
            players->lion_count++;
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

int game_over(struct board_elements *players) {
    if ((players->rabbit_count) == EXTINCT || (players->lion_count) == EXTINCT) { return TRUE; }
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

void gameplay(int grid_size, int game_table[grid_size][grid_size], struct board_elements *players) {
    printf("Coming up top\n");
    for (int row = 0; row < grid_size; row++) {
        //    printf("First loop\n");
        for (int column = 0; column < grid_size; column++) {
            //      printf("Second Loop\n");

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

                    //mandating death from hunger
                    int death_from_hunger = rand() % 100;
                    if (death_from_hunger > 80) {
                        game_table[0][0] = empty;
                        players->rabbit_count--;
                        printf("Rabbit died from hunger\n");
                        continue;
                    }

                    //initializing decision structure
                    int interaction_status = 0;
                    int *decision;

                    decision = play_rabbit(grid_size, game_table[grid_size][grid_size], players, ray,
                                           &interaction_status);

                    if (interaction_status == FIGHT) {
                        int survival = rand() % 70;
                        if (survival > 50) {
                            *ray[*decision] = rabbit;
                            printf("Rabbit won the fight and moved to %d\n", *ray[*decision]);
                        } else
                            players->rabbit_count--;
                        printf("Rabbit lost the fight\n");
                    }//fight

                    if (interaction_status == MATE) {
                        decision = move_away(ray);
                        if (decision == ray[8])
                        {
                            players->rabbit_count--;
                            game_table[row][column] = empty;
                            break;
                        }
                    }

                    *decision = rabbit;


                    if (interaction_status != STAY)
                        game_table[row][column] = empty;
                }


            if (game_table[row][column] == lion) {

                //dying from hunger
                int death_from_hunger = rand() % 100;
                if (death_from_hunger > 80) {
                    game_table[0][0] = empty;
                    players->lion_count--;
                    printf("Lion died from hunger\n");
                    continue;
                }

                int interaction_status = 0;
                int *decision;
                decision = play_lion(grid_size, game_table, players, ray, &interaction_status);


                if (*decision == FIGHT) {
                    int survival = rand() % 80;
                    if (survival > 20) {
                        *ray[*decision] = lion;
                        printf("Lion won the fight and moved to %d\n", *ray[*decision]);
                    } else
                        players->lion_count--;
                    printf("Lion lost the fight\n");
                }
                //fight
                if (interaction_status == MATE) {
                    decision = move_away(ray);
                    if (decision == ray[8])
                    {
                        players->lion_count--;
                        game_table[row][column] = empty;
                        break;
                    }
                }

                *decision = lion;


                if (interaction_status != STAY)
                    game_table[row][column] = empty;
            }

        }
    }
    printf("Rabbit count is currently: %d\n"
           " Lion count is currently: %d\n",
           players->rabbit_count,
           players->lion_count);
}

int *play_rabbit(int grid_size, int game_table[grid_size][grid_size],
                 struct board_elements *players, int **ray, int *interaction_status) {
    for (int i = 0; i < 7; i++) {
        switch (*ray[i]) {
            case food:
                printf("Rabbit ate food at %p\n", ray[i]);
                return ray[i]; //goto

            case rabbit:
                printf("Rabbit found a mate!\n");
                *interaction_status = MATE;

                if (players->rabbit_count > 35) {
                    *interaction_status = STAY;
                    break;
                }
                int found = 0;
                for (int row = 0; row < grid_size; row++) {

                    while (found != 1) {
                        int rand_spot_x = rand() % grid_size;
                        int rand_spot_y = rand() % grid_size;
                        printf("broke here 1");

                        if (game_table[rand_spot_x][rand_spot_y] <= 11 )
                        {
                            printf("broke here 2");
                            found = 1;
                            game_table[rand_spot_y][rand_spot_y] = rabbit;

                            printf("New baby rabbit\n");
                            players->rabbit_count++;
                            break;
                        }
                    }

                }
            case empty:
                printf("Rabbit moved to %p\n", ray[i]);
                return ray[i];//goto
            case lion:
                printf("Rabbit chose to fight\n");
                *interaction_status = FIGHT;
                return ray[i];
            case stone:
                printf("Rabbit stayed at %p\n", ray[i]);
                *interaction_status = STAY;
                return ray[8];
        }

    }
    return ray[rand() % 7];
}

int *play_lion(int grid_size, int game_table[grid_size][grid_size],
               struct board_elements *players, int **ray, int *interaction_status) {
    for (int i = 0; i < 7; i++) {
        switch (*ray[i]) {
            case rabbit:
                printf("Lion chose to fight\n");
                *interaction_status = FIGHT;
                return ray[i];

            case lion:
                printf("Lion found a mate!\n");
                *interaction_status = MATE;

                if (players->lion_count > 35) {
                    *interaction_status = STAY;
                    break;
                }
                int found = 0;
                for (int row = 0; row < grid_size; row++) {

                    while (found != 1) {
                        int rand_spot_x = rand() % grid_size;
                        int rand_spot_y = rand() % grid_size;
                        printf("broke here 3");
                        if (game_table[rand_spot_x][rand_spot_y] >=11 )
                        {
                            printf("broke here 4");
                            found = 1;
                            game_table[rand_spot_y][rand_spot_y] = lion;

                            printf("New baby lion\n");
                            players->lion_count++;
                            break;
                        }
                    }

                }
            case food:
                printf("Lion ate food at %p\n", ray[i]);
                return ray[i];//goto
            case empty:
                printf("Lion moved to %p\n", ray[i]);
                return ray[i];//goto
            case stone:
                printf("Lion stayed at %p\n", ray[i]);
                *interaction_status = STAY;
                return ray[8];
        }
    }
    return ray[rand() % 7];
}

void print_grid(int grid_size, int game_table[grid_size][grid_size]) {
    for (int row = 0; row < grid_size; row++) {
        //    printf("First loop\n");
        for (int column = 0; column < grid_size; column++) {
            printf("%*d ", 4, game_table[row][column]);
        }
        printf("\n");
    }
}

int *move_away(int **ray) {
    for (int i = 0; i < 7; i++) {
        if (*ray[i] != rabbit
            && *ray[i] != lion
            && *ray[i] != stone) {
            return ray[i];
        }

    }
    printf("An animal died of shock.");
    return ray[8];
}