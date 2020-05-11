#include "evaluate_map.h"

//结合负极大值的alpha-beta搜索，返回分析后所得的可行招
LinkQueue alpha_beta(int(*map)[BOARD_SIZE], int depth, double alpha, double beta, int side) {
	LinkQueue best_value_plan, node;
	InitLinkQueue(&best_value_plan);
	best_value_plan.eval = -INFINITY - 1;
	int i = 0;
	if (depth == 0) {
		node.eval = eval(side, map);
		return node;
	}
	else {
		LinkQueue* psb_move = (LinkQueue*)malloc(20 * sizeof(LinkQueue));
		int c = search_possible_plan(side, map, psb_move);
		if (c == 0) {
			node.eval = eval(side, map);
			return node;
		}
		else {
			if (depth == 8 && c == 1)
				return psb_move[1];
			else {
				for (i = 1; QueueEmpty(&psb_move[i]) == false; i++) {
					int temp_map[BOARD_SIZE][BOARD_SIZE];
					get_temp_map(&psb_move[i], temp_map, map);
					psb_move[i].eval = -alpha_beta(temp_map, depth - 1, -beta, -alpha, 3 - side).eval;
					if (psb_move[i].eval > best_value_plan.eval) {
						copyQueue(&best_value_plan, &psb_move[i]);
						best_value_plan.eval = psb_move[i].eval;
					}
					if (best_value_plan.eval > alpha)
						alpha = best_value_plan.eval;
					if (best_value_plan.eval >= beta) {//剪枝出发条件
						break;
					}
				}
				free(psb_move);
				return best_value_plan;
			}
		}
	}
}


struct Command aiTurn(int board[][BOARD_SIZE]) {
	int beginning_times;
	int end_times;
	beginning_times = clock();

	LinkQueue final;
	final = alpha_beta((int(*)[BOARD_SIZE])board, 8, -INFINITY, INFINITY, me_flag);

	struct Command command;//最终出子
	end_times = clock();
	printf("main fuction:%d ms\n", end_times - beginning_times);

	command.numStep = 1;
	int i = 0;
	while (QueueEmpty(&final) == false) {
		NodeMovement temp = popFrontQueue(&final);
		command.x[i] = temp.start.x;
		command.y[i] = temp.start.y;
		command.x[i + 1] = temp.end.x;
		command.y[i + 1] = temp.end.y;
		i++;
		command.numStep++;
	}
	return command;
}
