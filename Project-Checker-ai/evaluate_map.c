#include "move_generator.h"

void get_temp_map(LinkQueue* possible_move, int(*temp_map)[BOARD_SIZE], int(*now_map)[BOARD_SIZE]) {
	NodeMovement temp;
	int q, w;
	//初始化temp_map棋盘
	for (q = 0; q <= BOARD_SIZE - 1; q++) {
		for (w = (q + 1) % 2; w <= BOARD_SIZE - 1; w += 2) {
			temp_map[q][w] = now_map[q][w];
		}
	}
	QueueLinkNode* p = possible_move->front;
	int n;
	for (n = 1; n <= possible_move->count; n++) {
		temp = p->Qmove;
		temp_map[temp.end.x][temp.end.y] = temp_map[temp.start.x][temp.start.y];
		temp_map[temp.start.x][temp.start.y] = EMPTY;
		temp_map[temp.eat.x][temp.eat.y] = EMPTY;
		p = p->next;
	}
	//变王判定
	int j;
	for (j = 1; j <= BOARD_SIZE - 1; j += 2) {
		if (temp_map[0][j] == BLACK)
			temp_map[0][j] = BLACK_KING;
	}
	for (j = 0; j <= BOARD_SIZE - 2; j += 2) {
		if (temp_map[BOARD_SIZE - 1][j] == WHITE)
			temp_map[BOARD_SIZE - 1][j] = WHITE_KING;
	}//得到最终的temp_map
}

//估值函数区：全局变量，评估指标包括：子数，自由度（可以走的空格数），成王程度（距离变王行的距离
//中心度（在棋盘中心的程度），防御力（棋子相连阵法成型度），威胁度(可能被吃的程度）
int whitefree = 0, blackfree = 0, whitecount = 0, blackcount = 0;
int	whitetoking = 0, blacktoking = 0, blackcentre = 0, whitecentre = 0, blackdef = 0, whitedef = 0;
int blackthreat = 0, whitethreat = 0;
void rating_map(int temp_map[][BOARD_SIZE]) {  //棋子本身价值与成王度

	int i, j;
	whitefree = 0; blackfree = 0; whitecount = 0; blackcount = 0; whitetoking = 0; blacktoking = 0;
	blackcentre = 0; whitecentre = 0; blackdef = 0; whitedef = 0; blackthreat = 0; whitethreat = 0;
	int xd[4] = { -1,-1,1,1 };
	int yd[4] = { 1,-1,-1,1 };
	int threat_mark[BOARD_SIZE][BOARD_SIZE] = { 0 };
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = (i + 1) % 2; j < BOARD_SIZE; j += 2)
			if (temp_map[i][j] % 2 == 1)//黑子
			{
				if (temp_map[i][j] == BLACK_KING) {//黑王
					blackcount = blackcount + 7;//对于王的吃子后的优势暂时以5分优势代替，即算作兵棋本身价值的1/3
					blacktoking = blacktoking + BOARD_SIZE - 1 - i;
					if (temp_map[i + 1][j + 1] == 0) blackfree++;//黑王自由度计算
					if (temp_map[i + 1][j - 1] == 0) blackfree++;
					if (temp_map[i - 1][j + 1] == 0) blackfree++;
					if (temp_map[i - 1][j - 1] == 0) blackfree++;
				}
				else {
					//对于普通黑色棋子的价值计算
					blackcount = blackcount + 5;
					blacktoking = blacktoking + BOARD_SIZE - 1 - i;
					if (i <= 8 && j <= 8 && temp_map[i - 1][j + 1] == 0) blackfree++;//普通黑棋自由度计算
					if (i <= 8 && j <= 10 && j >= 1 && temp_map[i - 1][j - 1] == 0) blackfree++;

				}
				if (temp_map[i - 1][j + 1] % 2 == 1) blackdef++;//黑棋防御力计算
				if (temp_map[i + 1][j + 1] % 2 == 1) blackdef++;
				if (i == 3 && j % 2 == 0)  blackcentre++;//黑棋中心度计算
				if (i == 4 && j % 2 == 1)  blackcentre++;
				int q;
				for (q = 0; q < 4; q++) {//黑棋受威胁度计算
					if (
						((temp_map[i + xd[q]][j + yd[q]] % 2 == 0 && temp_map[i + xd[q]][j + yd[q]] != 0)
							|| threat_mark[i + xd[q]][j + yd[q]] == 1)
						&& temp_map[i - xd[q]][j - yd[q]] == EMPTY) {
						threat_mark[i - xd[q]][j - yd[q]] == 1;
						blackthreat++;
					}
				}


			}
			else if (temp_map[i][j] % 2 == 0 && temp_map[i][j] != EMPTY) {
				if (temp_map[i][j] == 4) {
					whitecount = whitecount + 7;
					whitetoking = whitetoking + i;
					if (temp_map[i + 1][j + 1] == EMPTY) whitefree++;//白王自由度计算
					if (temp_map[i + 1][j - 1] == EMPTY) whitefree++;
					if (temp_map[i - 1][j + 1] == EMPTY) whitefree++;
					if (temp_map[i - 1][j - 1] == EMPTY) whitefree++;
				}
				else {
					whitecount = whitecount + 5;
					whitetoking = whitetoking + i;
					if (temp_map[i + 1][j - 1] == 0) whitefree++;//白棋自由度计算
					if (temp_map[i + 1][j + 1] == 0) whitefree++;
				}
				if (temp_map[i - 1][j + 1] == 2 || temp_map[i - 1][j + 1] == 4) whitedef++;//白棋防御力计算
				if (temp_map[i + 1][j + 1] == 2 || temp_map[i - 1][j + 1] == 4) whitedef++;
				if (i == 3 && j % 2 == 0)  whitecentre++;//白棋中心度计算
				if (i == 4 && j % 2 == 1)  whitecentre++;
				int q;
				for (q = 0; q < 4; q++) {//白棋受威胁度计算
					if (
						(temp_map[i + xd[q]][j + yd[q]] % 2 == 1 || threat_mark[i + xd[q]][j + yd[q]] == 1)
						&& temp_map[i - xd[q]][j - xd[q]] == EMPTY) {
						threat_mark[i - xd[q]][j - xd[q]] == 1;
						whitethreat++;
					}
				}
			}
}

int eval(int color, int temp_map[][BOARD_SIZE]) {
	int  evaluation = 0, evaluationBLACK = 0, evaluationWHITE = 0;
	rating_map(temp_map);
	evaluationBLACK = blackcount * 10 + blacktoking + blackcentre * 1 + blackfree * 3 + blackdef * 3 - blackthreat * 10;
	evaluationWHITE = whitecount * 10 + whitetoking + whitecentre * 1 + whitefree * 3 + whitedef * 3 - whitethreat * 10;
	evaluation = evaluationBLACK - evaluationWHITE;
	if (color == BLACK) {//白色方
		return evaluation;
	}
	if (color == WHITE) {//黑色方
		return -evaluation;
	}
	return evaluation;
}