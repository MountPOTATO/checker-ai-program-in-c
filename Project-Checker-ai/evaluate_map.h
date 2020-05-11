#ifndef _EVALUATE_MAP_H_
#define _EVALUATE_MAP_H_
#include "move_generator.h"
void get_temp_map(LinkQueue* possible_move, int(*temp_map)[BOARD_SIZE], int(*now_map)[BOARD_SIZE]);
void rating_map(int temp_map[][BOARD_SIZE]);
int eval(int color, int temp_map[][BOARD_SIZE]);

#endif
