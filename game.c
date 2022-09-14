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
#define STAY 500UL
#define FIGHT 1000UL
#define MATE 1500UL

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

            if (board_content_regulator(players, &fill_space) == TRUE)
                game_table[row][column] = fill_space;
             else {
                game_table[row][column] = empty;
            }
        }
    }//
}


// function to ensure that the board is initialized in a way that is mostly playable.
int board_content_regulator(struct board_elements *players, enum board_contents *fill_space) {
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


void gameplay(int grid_size, int game_table[grid_size][grid_size], struct board_elements *players) {
    for (int row = 0; row < grid_size; row++) {
        for (int column = 0; column < grid_size; column++) {

            int *current_player_surroundings[] =
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

#if 1
            /* this code is a relic of the debugging process, i choose to leave it as a message
            * written in blood on the walls of this project*/
                  if (CURRENT_PLAYER == lion || CURRENT_PLAYER == rabbit)
                  {
                      display_player_surroundings_coords (grid_size, game_table, row,column);
                      display_player_surroundings_entities (grid_size, game_table, row,column);
                  }
#endif
            if (death_from_hunger(grid_size, game_table, row, column, players, current_player_surroundings)) {
                continue;
            }

            int interaction_status = 0;
            int *decision=0;

            if (interaction_status == FIGHT) {
                enact_fight(grid_size, game_table, row, column,
                            players, current_player_surroundings, &interaction_status, decision);
                continue;
            }
            if (interaction_status == MATE) {
                enact_mating(grid_size, game_table, row, column,
                             players, current_player_surroundings, &interaction_status, decision);
                continue;
            }
            //  printf("If this message is preceded by died hunger, that's a bad sign\n");

            if ((game_table[row][column]) == rabbit) {
                decision = play_rabbit(grid_size, game_table, players, current_player_surroundings,
                                       &interaction_status);
                *decision = rabbit;
            }

            if (game_table[row][column] == lion) {
                decision = play_lion(grid_size, game_table, players, current_player_surroundings, &interaction_status);
                *decision = lion;
            }

            if (interaction_status != STAY)
                game_table[row][column] = empty;

#if 1
            printf("Interaction status: %d\n", interaction_status);
            true_entity_counter( grid_size,  game_table, row,  column);
            apparent_entity_counter( grid_size,  game_table, row,  column,
           players);

#endif
        }
    }


}

int *play_rabbit(int grid_size, int game_table[grid_size][grid_size],
                 struct board_elements *players, int **current_player_surroundings, int *interaction_status) {

    for (int target_surrounding_position = 0; target_surrounding_position < 7; target_surrounding_position++) {

        switch (*current_player_surroundings[target_surrounding_position]) {
            case food:
                printf("Rabbit ate food at %p\n",
                       current_player_surroundings[target_surrounding_position]);

                return current_player_surroundings[target_surrounding_position]; //goto

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

                        if (game_table[rand_spot_x][rand_spot_y] >=11 )
                        {
                            found = 1;
                            game_table[rand_spot_x][rand_spot_y] = rabbit;

                            printf("New baby rabbit\n");
                            players->rabbit_count++;
                            break;
                        }
                    }

                }
            case empty:
                printf("Rabbit moved to %p\n",
                       current_player_surroundings[target_surrounding_position]);
                return current_player_surroundings[target_surrounding_position];//goto
            case lion:
                printf("Rabbit chose to fight\n");
                *interaction_status = FIGHT;
                return current_player_surroundings[target_surrounding_position];
            case stone:
                printf("Rabbit stayed at %p\n",
                       current_player_surroundings[target_surrounding_position]);
                *interaction_status = STAY;
                return current_player_surroundings[8];
        }

    }
    return current_player_surroundings[rand() % 7];
}

int *play_lion(int grid_size, int game_table[grid_size][grid_size],
               struct board_elements *players, int **current_player_surroundings, int *interaction_status) {
    for (int target_surrounding_position = 0; target_surrounding_position < 7; target_surrounding_position++) {
        switch (*current_player_surroundings[target_surrounding_position]) {
            case rabbit:
                printf("Lion chose to fight\n");
                *interaction_status = FIGHT;
                return current_player_surroundings[target_surrounding_position];

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
                        if (game_table[rand_spot_x][rand_spot_y] >=11 )
                        {
                            found = 1;
                            game_table[rand_spot_x][rand_spot_y] = lion;

                            printf("New baby lion\n");
                            players->lion_count++;
                            break;
                        }
                    }

                }
            case food:
                printf("Lion ate food at %p\n", current_player_surroundings[target_surrounding_position]);
                return current_player_surroundings[target_surrounding_position];//goto
            case empty:
                printf("Lion moved to %p\n", current_player_surroundings[target_surrounding_position]);
                return current_player_surroundings[target_surrounding_position];//goto
            case stone:
                printf("Lion stayed at %p\n", current_player_surroundings[target_surrounding_position]);
                *interaction_status = STAY;
                return current_player_surroundings[8];
        }
    }
    return current_player_surroundings[rand() % 7];
}

void print_grid(int grid_size, int game_table[grid_size][grid_size]) {
    for (int row = 0; row < grid_size; row++) {
        for (int column = 0; column < grid_size; column++) {
            switch(game_table[row][column]) {
                case rabbit:
                    printf(" R", 3);
                    break;
                case lion:
                    printf(" L", 3);
                    break;
                case stone:
                    printf(" S", 3);
                    break;
                case food:
                    printf(" F", 3);
                    break;
                default:
                    printf("  ", 3);
                    break;
            }
        }
        printf("\n");
    }
}

int *move_away(int **current_player_surroundings) {
    for (int target_surrounding_position = 0; target_surrounding_position < 7; target_surrounding_position++) {
        if (*current_player_surroundings[target_surrounding_position] != rabbit
            && *current_player_surroundings[target_surrounding_position] != lion
            && *current_player_surroundings[target_surrounding_position] != stone) {
            return current_player_surroundings[target_surrounding_position];
        }

    }
    printf("An animal died of shock.");
    return current_player_surroundings[8];
}

int death_from_hunger(int grid_size, int game_table[grid_size][grid_size],int row, int column,
                       struct board_elements *players, int **current_player_surroundings)
{
    if (game_table[row][column] == lion) {
        //dying from hunger
        int death_from_hunger = rand() % 100;
        if (death_from_hunger > 80) {
            game_table[row][column] = empty;
            players->lion_count--;
            printf("Lion died from hunger\n");
            return TRUE;
        }
    }
    if ((game_table[row][column]) == rabbit) {
        //mandating death from hunger
        int death_from_hunger = rand() % 100;
        if (death_from_hunger > 80) {
            players->rabbit_count--;
            game_table[row][column] = empty;
            printf("Rabbit died from hunger\n");
            return TRUE;
        }
    }
    return FALSE;
}
int enact_fight(int grid_size, int game_table[grid_size][grid_size],int row, int column,
                      struct board_elements *players, int **current_player_surroundings, int* interaction_status,
                              int* decision)
{
    if (*interaction_status != FIGHT)
    {
        return FALSE;
    }
    if (game_table[row][column] == lion) {
        int survival = rand() % 80;
        if (survival > 20) {
            *decision = lion;
            printf("Lion won the fight and moved to %p\n", current_player_surroundings[*decision]);
        } else {
            players->lion_count--;
            game_table[row][column] = empty;
            printf("Lion lost the fight\n");
        }
    }

///////////////////////////////////////////////////////////////

    if (game_table[row][column] == rabbit)
    {
        int survival = rand() % 70;
        if (survival > 50) {
            *decision = rabbit;
            printf("Rabbit won the fight and moved to %p\n", current_player_surroundings[*decision]);
        } else {
            players->rabbit_count--;
            game_table[row][column] = empty;
            printf("Rabbit lost the fight\n");
        }
    }//fight

    return TRUE;
}

int enact_mating(int grid_size, int game_table[grid_size][grid_size],int row, int column,
                 struct board_elements *players, int **current_player_surroundings, int* interaction_status,
                 int* decision)
{
if (*interaction_status != MATE)
{return FALSE;}
    if (game_table[row][column] == lion) {
        decision = move_away(current_player_surroundings);
        if (decision == current_player_surroundings[8])
        {
            players->lion_count--;
            game_table[row][column] = empty;
            return TRUE;
        }
    }
    ///////////////////////////////////////////////////////////////
    if (game_table[row][column] == rabbit) {
        decision = move_away(current_player_surroundings);
        if (decision == current_player_surroundings[8])
        {
            players->rabbit_count--;
            game_table[row][column] = empty;
            return TRUE;
        }
    }
}

//pure debug tool

void display_player_surroundings_coords (int grid_size, int game_table[grid_size][grid_size],int row, int column)
{
    char player = '0';
    if (game_table[row][column] == lion)
    {
        player = 'L';
    }
    if (game_table[row][column] == rabbit)
    {
        player = 'R';
    }
    printf("Current %c Surroundings\n"
            "[%d][%d] [%d][%d] [%d][%d]\n"
                  "[%d][%d] [%d][%d] [%d][%d]\n"
                  "[%d][%d] [%d][%d] [%d][%d]\n",

                 player, ROW_BACKWARD,COLUMN_BACKWARD,ROW_BACKWARD,column, ROW_BACKWARD, COLUMN_FORWARD,
                  row, COLUMN_BACKWARD, row, column, row, COLUMN_FORWARD,
                  ROW_FORWARD, COLUMN_BACKWARD, ROW_FORWARD, column, ROW_FORWARD,COLUMN_FORWARD);
}

void display_player_surroundings_entities (int grid_size, int game_table[grid_size][grid_size],int row, int column)
{

    char player = '0';
    if (game_table[row][column] == lion)
    {
        player = 'L';
    }
    if (game_table[row][column] == rabbit)
    {
        player = 'R';
    }

    printf("Current %c Surroundings\n"
           "[%d] [%d] [%d]\n"
           "[%d] [%c] [%d]\n"
           "[%d] [%d] [%d]\n",

           player, game_table[ROW_BACKWARD][COLUMN_BACKWARD],game_table[ROW_FORWARD][column],game_table[ROW_BACKWARD][COLUMN_FORWARD],
           game_table[row][COLUMN_BACKWARD],player,game_table[row][COLUMN_FORWARD],
           game_table[ROW_FORWARD][COLUMN_BACKWARD],game_table[ROW_FORWARD][column],game_table[ROW_FORWARD][COLUMN_FORWARD]);

}

void true_entity_counter(int grid_size, int game_table[grid_size][grid_size],int row, int column)
{
    int lions = 0;
    int rabbits = 0;
    for (int row = 0; row < grid_size; row++) {
        for (int column = 0; column < grid_size; column++)
        {
            if (game_table[row][column] == lion) lions++;
            if (game_table[row][column] == rabbit) rabbits++;

        }
    }
    printf("True count Lions | Rabbits: %d | %d\n",lions, rabbits);
}

void apparent_entity_counter(int grid_size, int game_table[grid_size][grid_size],int row, int column,
                         struct board_elements *players)
{
    int lions = 0;
    int rabbits = 0;

    lions =  players->lion_count;
    rabbits =  players->rabbit_count;

   printf("Apparent count Lions | Rabbits: %d | %d\n",lions, rabbits);
}