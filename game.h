//
// Created by z434 on 9/2/2022.
//

#ifndef HW1_GAME_H
#define HW1_GAME_H
enum board_contents {empty = 0, stone = 1, food = 2, rabbit = 3, lion = 4};

int board_content_regulator (int* empty_count, int* stone_count,
                             int* food_count, int* rabbit_count, int* lion_count, enum board_contents fill_space);

void board_init(int** game_table[]);// we'll key this back in if we decide to play w/ dynamic alloc:, int m, int n)




#endif //HW1_GAME_H
