#include <stdbool.h>
#include "stack_queue_func.h"
//�����������ɺ���
/***********************************************************
	analyseEat
	���ܣ�����ĳ�����괦����sideһ����������û��һ�����Գ��ӵ����ӵ�
		 ���У�����true
	��������˵����side
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
			&& y + 2 * yd[q]>-1 && y + 2 * yd[q] < BOARD_SIZE) {//δ�����������̷�Χ��
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
				//ѹ��ȥ�Է�ջ
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
				&& y + yd[q]>-1 && y + yd[q] < BOARD_SIZE) {//δ�����������̷�Χ��
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
//�Ե�ǰ������з������ѿ��е��߷���Է�������м���psb_move��
int  search_possible_plan(int side, int(*given_map)[BOARD_SIZE], LinkQueue psb_move[]) {
	LinkStack Eat_Step, Move_Step;//��һ�����ӵĳԷ�ջ �߷�ջ
	LinkStack* pEat = &Eat_Step, * pMove = &Move_Step;
	LinkQueue Generating_Plan;//�����ɿ����������߷�����
	NodeCord Cordcur;//��ǰ�Ľ������
	NodeMovement SEEcur;//��ǰ��SEE�������
	int nQ = 1, i, n;
	//�Ե�ǰ���ݽṹ��һϵ�г�ʼ��
	InitLinkStack(&Eat_Step);
	InitLinkStack(&Move_Step);
	InitLinkQueue(&Generating_Plan);
	for (int q = 0; q <= 19; q++)
		InitLinkQueue(&psb_move[q]);
	int mode_eat = 0, mode_move = 0;//�жϵ�ǰ�߷������ʣ�mode_eat==1ʱ��ǰΪ�Է���mode_eat==0&&mode_move==1ʱ��ǰΪ�Է�,����Ϊ0ʱû���߷�
	//�������̵Ĳ���
	for (i = 0; i < BOARD_SIZE; i++) {
		for (n = (i + 1) % 2; n < BOARD_SIZE; n += 2) {
			if ((side == WHITE && (given_map[i][n] == WHITE || given_map[i][n] == WHITE_KING)) ||
				(side == BLACK && (given_map[i][n] == BLACK || given_map[i][n] == BLACK_KING))) {//�������˵�ǰ���ӷ���������ɫ
				//Ϊ��һ��������긳ֵ
				int first;
				int first_state = 0;
				Cordcur.x = i;
				Cordcur.y = n;
				//��ճԷ�ջ,��մ����ɿ����еĶ���,����ʼ��	
				InitLinkStack(&Eat_Step);
				InitLinkQueue(&Generating_Plan);
				int  EatRecurMark[BOARD_SIZE][BOARD_SIZE] = { 0 };//���ӻ��ݱ������
				//��һ������û�гԷ�ʱ�������߷���ѹ������ջ������ѭ��������������whileѭ��
				if (mode_eat == 0 && analyseEat(Cordcur, side, 1, EatRecurMark, pEat, given_map) == false) {//��ǰ�ڵ㲻�ɳ�ʱ
					bool c = analyseMove(Cordcur, side, EatRecurMark, pMove, given_map);//��֮ǰû�п��гԷ�ʱĬ���ȷ����߷������������߷��洢���߷�ջMoveStep��
					if (c == true) mode_move = 1;
					continue;
				}
				//������ڳ����߷������߷�ѹ��δ��ɵ��߷�ջEat_Step��ͬʱ���MoveStep�߷�ջ���˺���ʹ��
				while (true) {
					first_state++;
					if (first_state == 1) {
						first = given_map[i][n];
						given_map[i][n] = EMPTY;
					}
					//����߷�ջ
					mode_eat = 1;
					if (analyseEat(Cordcur, side, 0, EatRecurMark, pEat, given_map) == true) {//������ǰ�ɳ��߷������ѿ��е��߷�ѹ����Eat_Step��
					//��δ����߷�ջ��ѡһ��ִ��
						SEEcur = popStack(&Eat_Step);
						EatRecurMark[SEEcur.eat.x][SEEcur.eat.y] = 1;
						pushQueue(SEEcur, &Generating_Plan);
						//�ڵ���£���һ�η����ɳ��߷�ʱ�ӳ��Ӻ�����괦��ʼ����
						Cordcur = SEEcur.end;
					}
					else {//��ǰ�ڵ�û�гԷ��߷�ʱ
						int q;
						if (QueueItemCount(&Generating_Plan) > QueueItemCount(&psb_move[nQ])) {//����ɳԵ���������֮ǰ�ɳԵ������������¸���psb����move
							for (q = 1; q < 20; q++)
								InitLinkQueue(&psb_move[q]);
							nQ = 1;
							psb_move[nQ].count = 0;
							copyQueue(&psb_move[nQ], &Generating_Plan);
						}
						else if (QueueItemCount(&Generating_Plan) == QueueItemCount(&psb_move[nQ])) { //����ɳԵ���������֮ǰ�ɳԵ��������򴢴�
							++nQ;
							psb_move[nQ].count = 0;
							copyQueue(&psb_move[nQ], &Generating_Plan);
						}
						//���ݿ����߷�
						while (QueueItemCount(&Generating_Plan) != 0) {
							SEEcur = popRearQueue(&Generating_Plan);
							EatRecurMark[SEEcur.eat.x][SEEcur.eat.y] = 2;
								//�ҵ���֧�߷�	
								//�����ǰ�������ݽṹ����Ϊ���� SEEcur��ndstart��Eat_Step�����з���ndstart���
							if (StackEmpty(&Eat_Step) == true && QueueEmpty(&Generating_Plan) == true)
								break;
							if (StackEmpty(&Eat_Step) == false) {//ջ��Ϊ��
								if ((SEEcur.start.x) == getTop(&Eat_Step).start.x &&
									(SEEcur.start.y) == getTop(&Eat_Step).start.y) {//�з�֧��
									SEEcur = popStack(&Eat_Step);
									EatRecurMark[SEEcur.eat.x][SEEcur.eat.y] = 1;
									pushQueue(SEEcur, &Generating_Plan);
									Cordcur = SEEcur.end;
									break;
								}
								else //û�з�֧��ʱ
									continue;
							}
							else {//stackΪ��ʱ,�������
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
				//������ѭ��
				given_map[i][n] = first;
			}
		}
	}
	if (mode_eat == 0 && mode_move == 0)//���ؿ�����������Ŀ�����²����ӣ�����0
		return 0;
	else {
		int q;
		if (QueueEmpty(&psb_move[1]) == true) {//��û�п��еĳԷ�����,��Move_Stepȫ���������
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
