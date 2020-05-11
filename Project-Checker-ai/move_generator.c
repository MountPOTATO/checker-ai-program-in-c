#include <stdbool.h>
#include "stack_queue_func.h"
//可行招数生成函数
/***********************************************************
	analyseEat
	功能：分析某个坐标处属于side一方的棋子有没有一个可以吃子的落子点
		 若有，返回true
	函数参数说明：side
				judge_mode

************************************************************/
bool analyseEat(NodeCord nd, int side, int judge_mode, int eat_recur_mark[][BOARD_SIZE], LinkStack* pEat, int(*given_map)[BOARD_SIZE]) {
	bool Finded = false;
	int q = 0;
	int xd[4] = { 1,-1,-1,1 };
	int yd[4] = { 1,1,-1,-1 };
	struct NodeMovement eat_movement;
	int x = nd.x, y = nd.y;

	for (q = 0; q < 4; q++) {
		if (x + 2 * xd[q] > -1 && x + 2 * xd[q]<BOARD_SIZE
			&& y + 2 * yd[q]>-1 && y + 2 * yd[q] < BOARD_SIZE) {//未来落子在棋盘范围内
			if (((side == BLACK && (NEB_SPOT == WHITE || NEB_SPOT == WHITE_KING)) ||
				(side == WHITE && (NEB_SPOT == BLACK || NEB_SPOT == BLACK_KING)))
				&& eat_recur_mark[x + xd[q]][y + yd[q]] == 0 && given_map[x + 2 * xd[q]][y + 2 * yd[q]] == EMPTY) {
				if (judge_mode == 1) {
					Finded = true;
					break;
				}
				eat_movement.start.x = x;
				eat_movement.start.y = y;
				eat_movement.end.x = x + 2 * xd[q];
				eat_movement.end.y = y + 2 * yd[q];
				eat_movement.eat.x = x + xd[q];
				eat_movement.eat.y = y + yd[q];
				//压进去吃法栈
				pushStack(pEat, eat_movement);
				Finded = true;
			}
		}
	}
	return Finded;
}

bool analyseMove(NodeCord nd, int side, int eat_recur_mark[][BOARD_SIZE], LinkStack* pMove, int(*given_map)[BOARD_SIZE]) {
	bool Finded = false;
	int xd[4] = { -1,-1,1,1 };
	int yd[4] = { 1,-1,-1,1 };
	int q = 0;
	struct NodeMovement move_movement;
	int x = nd.x, y = nd.y;
	if (side == WHITE && given_map[x][y] == WHITE) {
		for (q = 2; q < 4; q++) {
			if (x + xd[q] > -1 && x + xd[q]<BOARD_SIZE && y + yd[q]>-1 && y + yd[q] < BOARD_SIZE) {
				if (given_map[x + xd[q]][y + yd[q]] == EMPTY && eat_recur_mark[x + xd[q]][y + yd[q]] == 0) {
					move_movement.start.x = x;
					move_movement.start.y = y;
					move_movement.end.x = x + xd[q];
					move_movement.end.y = y + yd[q];
					move_movement.eat.x = 0;
					move_movement.eat.y = 0;
					pushStack(pMove, move_movement);
					Finded = true;

				}
			}
		}
	}
	else if (side == BLACK && given_map[x][y] == BLACK) {
		for (q = 0; q < 2; q++) {
			if (x + xd[q] > -1 && x + xd[q]<BOARD_SIZE && y + yd[q]>-1 && y + yd[q] < BOARD_SIZE) {
				if (given_map[x + xd[q]][y + yd[q]] == EMPTY && eat_recur_mark[x + xd[q]][y + yd[q]] == 0) {
					move_movement.start.x = x;
					move_movement.start.y = y;
					move_movement.end.x = x + xd[q];
					move_movement.end.y = y + yd[q];
					move_movement.eat.x = 0;
					move_movement.eat.y = 0;

					pushStack(pMove, move_movement);
					Finded = true;
				}
			}
		}
	}
	else if (given_map[x][y] == WHITE_KING || given_map[x][y] == BLACK_KING) {
		for (q = 0; q < 4; q++) {
			if (x + xd[q] > -1 && x + xd[q]<BOARD_SIZE
				&& y + yd[q]>-1 && y + yd[q] < BOARD_SIZE) {//未来落子在棋盘范围内
				if (x + xd[q] > -1 && x + xd[q]<BOARD_SIZE && y + yd[q]>-1 && y + yd[q] < BOARD_SIZE
					&& given_map[x + xd[q]][y + yd[q]] == EMPTY && eat_recur_mark[x + xd[q]][y + yd[q]] == 0) {
					move_movement.start.x = x;
					move_movement.start.y = y;
					move_movement.end.x = x + xd[q];
					move_movement.end.y = y + yd[q];
					move_movement.eat.x = 0;
					move_movement.eat.y = 0;
					pushStack(pMove, move_movement);
					Finded = true;
				}
			}
		}
	}
	return Finded;
}
//对当前局面进行分析，把可行的走法或吃法放入队列集合psb_move中
int  search_possible_plan(int side, int(*given_map)[BOARD_SIZE], LinkQueue psb_move[]) {
	LinkStack Eat_Step, Move_Step;//对一个棋子的吃法栈 走法栈
	LinkStack* pEat = &Eat_Step, * pMove = &Move_Step;
	LinkQueue Generating_Plan;//待生成可行招数的走法队列
	NodeCord Cordcur;//当前的结点坐标
	NodeMovement SEEcur;//当前的SEE组合坐标
	int nQ = 1, i, n;
	//对当前数据结构的一系列初始化
	InitLinkStack(&Eat_Step);
	InitLinkStack(&Move_Step);
	InitLinkQueue(&Generating_Plan);
	for (int q = 0; q <= 19; q++)
		InitLinkQueue(&psb_move[q]);
	int mode_eat = 0, mode_move = 0;//判断当前走法的性质：mode_eat==1时当前为吃法，mode_eat==0&&mode_move==1时当前为吃法,两者为0时没有走法
	//遍历棋盘的部分
	for (i = 0; i < BOARD_SIZE; i++) {
		for (n = (i + 1) % 2; n < BOARD_SIZE; n += 2) {
			if ((side == WHITE && (given_map[i][n] == WHITE || given_map[i][n] == WHITE_KING)) ||
				(side == BLACK && (given_map[i][n] == BLACK || given_map[i][n] == BLACK_KING))) {//搜索到了当前落子方的棋子颜色
				//为第一个结点坐标赋值
				int first;
				int first_state = 0;
				Cordcur.x = i;
				Cordcur.y = n;
				//清空吃法栈,清空待生成可行招的队列,并初始化	
				InitLinkStack(&Eat_Step);
				InitLinkQueue(&Generating_Plan);
				int  EatRecurMark[BOARD_SIZE][BOARD_SIZE] = { 0 };//吃子回溯标记棋盘
				//第一个棋子没有吃法时，分析走法并压入走子栈，继续循环，不进入下面while循环
				if (mode_eat == 0 && analyseEat(Cordcur, side, 1, EatRecurMark, pEat, given_map) == false) {//当前节点不可吃时
					bool c = analyseMove(Cordcur, side, EatRecurMark, pMove, given_map);//当之前没有可行吃法时默认先分析走法，并将可行走法存储入走法栈MoveStep中
					if (c == true) mode_move = 1;
					continue;
				}
				//如果存在吃子走法，将走法压入未完成的走法栈Eat_Step，同时清空MoveStep走法栈，此后不再使用
				while (true) {
					first_state++;
					if (first_state == 1) {
						first = given_map[i][n];
						given_map[i][n] = EMPTY;
					}
					//清空走法栈
					mode_eat = 1;
					if (analyseEat(Cordcur, side, 0, EatRecurMark, pEat, given_map) == true) {//分析当前可吃走法，并把可行的走法压入了Eat_Step中
					//从未完成走法栈中选一个执行
						SEEcur = popStack(&Eat_Step);
						EatRecurMark[SEEcur.eat.x][SEEcur.eat.y] = 1;
						pushQueue(SEEcur, &Generating_Plan);
						//节点更新，下一次分析可吃走法时从吃子后的坐标处开始分析
						Cordcur = SEEcur.end;
					}
					else {//当前节点没有吃法走法时
						int q;
						if (QueueItemCount(&Generating_Plan) > QueueItemCount(&psb_move[nQ])) {//如果可吃的子数大于之前可吃的子数，则重新更新psb——move
							for (q = 1; q < 20; q++)
								InitLinkQueue(&psb_move[q]);
							nQ = 1;
							psb_move[nQ].count = 0;
							copyQueue(&psb_move[nQ], &Generating_Plan);
						}
						else if (QueueItemCount(&Generating_Plan) == QueueItemCount(&psb_move[nQ])) { //如果可吃的子数等于之前可吃的子数，则储存
							++nQ;
							psb_move[nQ].count = 0;
							copyQueue(&psb_move[nQ], &Generating_Plan);
						}
						//回溯可行走法
						while (QueueItemCount(&Generating_Plan) != 0) {
							SEEcur = popRearQueue(&Generating_Plan);
							EatRecurMark[SEEcur.eat.x][SEEcur.eat.y] = 2;
								//找到分支走法	
								//如果当前两个数据结构都不为空且 SEEcur的ndstart与Eat_Step顶部招法的ndstart相等
							if (StackEmpty(&Eat_Step) == true && QueueEmpty(&Generating_Plan) == true)
								break;
							if (StackEmpty(&Eat_Step) == false) {//栈不为空
								if ((SEEcur.start.x) == getTop(&Eat_Step).start.x &&
									(SEEcur.start.y) == getTop(&Eat_Step).start.y) {//有分支点
									SEEcur = popStack(&Eat_Step);
									EatRecurMark[SEEcur.eat.x][SEEcur.eat.y] = 1;
									pushQueue(SEEcur, &Generating_Plan);
									Cordcur = SEEcur.end;
									break;
								}
								else //没有分支点时
									continue;
							}
							else {//stack为空时,连续清空
								do {
									SEEcur = popRearQueue(&Generating_Plan);

									EatRecurMark[SEEcur.eat.x][SEEcur.eat.y] = 2;
								} while (QueueEmpty(&Generating_Plan) == false);
								break;
							}

						}
						if (StackEmpty(&Eat_Step) == true && QueueEmpty(&Generating_Plan) == true)
							break;
					}
				}
				//出回溯循环
				given_map[i][n] = first;
			}
		}
	}
	if (mode_eat == 0 && mode_move == 0)//返回可行招数的数目，若下不了子，返回0
		return 0;
	else {
		int q;
		if (QueueEmpty(&psb_move[1]) == true) {//若没有可行的吃法队列,将Move_Step全部存入队列
			nQ = 1;
			for (q = 1; StackEmpty(&Move_Step) == false; q++) {
				NodeMovement temp = popStack(&Move_Step);
				pushQueue(temp, &psb_move[nQ]);
				nQ++;
			}
			nQ--;
		}
		if (StackEmpty(&Move_Step) == false) {
			DestroyStack(&Move_Step);
		}
		DestroyStack(&Eat_Step);
		DestroyQueue(&Generating_Plan);
		return nQ;
	}
}
