//
// Created by z434 on 9/2/2022.
//

#ifndef HW1_GAME_H
#define HW1_GAME_H
enum board_contents {
    empty = 0, stone = 10, food = 11, rabbit = 12, lion = 13
};

struct board_elements {
    int stone_count;
    int food_count;
    int rabbit_count;
    int lion_count;
};

int board_content_regulator(struct board_elements *players, enum board_contents *fill_space);

void board_init(int grid_size, int game_table[grid_size][grid_size],
                struct board_elements *players);// we'll key this back in if we decide to play w/ dynamic alloc:, int m, int n)

int game_over(struct board_elements *players);

void gameplay(int grid_size, int game_table[grid_size][grid_size], struct board_elements *players);

int *play_rabbit(int grid_size, int game_table[grid_size][grid_size],
                 struct board_elements *players, int **ray, int *interaction_status);

int *play_lion(int grid_size, int game_table[grid_size][grid_size],
               struct board_elements *players, int **ray, int *interaction_status);

void print_grid(int grid_size, int game_table[grid_size][grid_size]);

int *move_away(int **ray);

int death_from_hunger(int grid_size, int game_table[grid_size][grid_size],int row, int column,
                       struct board_elements *players, int **ray);
int enact_fight(int grid_size, int game_table[grid_size][grid_size],int row, int column,
                struct board_elements *players, int **ray, int* interaction_status,
                int* decision);
int enact_mating(int grid_size, int game_table[grid_size][grid_size],int row, int column,
                 struct board_elements *players, int **ray, int* interaction_status,
                 int* decision);

//debug tools
void display_player_surroundings_entities (int grid_size, int game_table[grid_size][grid_size],int row, int column);
void display_player_surroundings_coords (int grid_size, int game_table[grid_size][grid_size],int row, int column);
void true_entity_counter(int grid_size, int game_table[grid_size][grid_size],int row, int column);
void apparent_entity_counter(int grid_size, int game_table[grid_size][grid_size],int row, int column,
                             struct board_elements *players);

#endif //HW1_GAME_H
