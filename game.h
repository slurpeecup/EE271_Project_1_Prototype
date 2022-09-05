//
// Created by z434 on 9/2/2022.
//

#ifndef HW1_GAME_H
#define HW1_GAME_H
enum board_contents {empty = 0, stone = 10, food = 11, rabbit = 12, lion = 13};

struct board_elements
{
    int stone_count;
    int food_count;
    int rabbit_count;
    int lion_count;
};

int board_content_regulator (struct board_elements* players, enum board_contents* fill_space);

void board_init(int grid_size, int game_table[grid_size][grid_size],struct board_elements* players);// we'll key this back in if we decide to play w/ dynamic alloc:, int m, int n)

int game_over(struct board_elements* players);

void gameplay(int grid_size, int game_table[grid_size][grid_size],struct board_elements* players);

int* play_rabbit(int grid_size, int game_table[grid_size][grid_size],
                struct board_elements* players, int** ray, int* interaction_status);
int* play_lion(int grid_size, int game_table[grid_size][grid_size],
               struct board_elements* players, int** ray, int* interaction_status);


#endif //HW1_GAME_H
