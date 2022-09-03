//
// Created by z434 on 9/2/2022.
//

#ifndef HW1_GAME_H
#define HW1_GAME_H
enum board_contents {empty = 0, stone = 1, food = 2, rabbit = 3, lion = 4};

struct board_elements
{
    int empty_count;
    int stone_count;
    int food_count;
    int rabbit_count;
    int lion_count;
};

int board_content_regulator (struct board_elements* players, enum board_contents fill_space);

void board_init(int grid_size, int game_table[grid_size][grid_size],struct board_elements* players);// we'll key this back in if we decide to play w/ dynamic alloc:, int m, int n)

int game_over(struct board_elements* players);

void gameplay(int grid_size, int game_table[grid_size][grid_size],struct board_elements* players);

int play_rabbit(int game_table[25][25],struct board_elements* players, int* ray);
int play_lion(int game_table[25][25],struct board_elements* players, int* ray);


#endif //HW1_GAME_H
