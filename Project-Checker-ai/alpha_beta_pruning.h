#ifndef _ALPHA_BETA_PRUNING_H_
#define _ALPHA_BETA_PRUNING_H_
#include "evaluate_map.h"

LinkQueue alpha_beta(int(*map)[BOARD_SIZE], int adepth, double alpha, double beta, int side);
struct Command aiTurn(int board[][BOARD_SIZE]);

#endif