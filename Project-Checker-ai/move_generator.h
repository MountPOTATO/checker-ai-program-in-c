#ifndef _MOVE_GENERATOR_H_
#define _MOVE_GENERATOR_H_
#include "stack_queue_func.h"


bool analyseMove(NodeCord nd, int side, int eat_recur_mark[][BOARD_SIZE], LinkStack* pMove, int(*given_map)[BOARD_SIZE]);//对当前坐标nd分析走法
bool analyseEat(NodeCord nd, int side, int judge_mode, int eat_recur_mark[][BOARD_SIZE], LinkStack* pEat, int(*given_map)[BOARD_SIZE]);//对当前坐标nd分析吃法
int  search_possible_plan(int side, int(*given_map)[BOARD_SIZE], LinkQueue psb_move[]);

#endif