#include "alpha_beta_pruning.h"

#pragma warning(disable:4996)
int board[BOARD_SIZE][BOARD_SIZE] = { 0 };
int me_flag;
int other_flag;
/***************************************************************************************
	IO说明：
	可输入三种指令：
	1.START 指令：在开局时输入：START AI方代号meflag（1为黑方，2为白方）
	  如：
	  START 1 
	  为初始化界面，黑方为AI方

	  默认黑方先着子

    2.PLACE 指令：为自己着子：PLACE 己方步数 每一步对应的坐标
	  如：PLACE 3 6,3 4,5 2,3
	  表示棋子可跳两次 从6,3跳到4,5再跳到2,3（吃了敌方两颗子）
	  如：PLACE 2 1,0 2,1
	  表示棋子从1,0走到2,1

	3.TURN指令：输入TURN指令，让AI落子

****************************************************************************************/

//打印棋盘
void printBoard() {
	printf("    0 1 2 3 4 5 6 7\n");
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		printf(" %d ", i);
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == EMPTY) {
				if ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))
					printf("%c%c", '\xA1', '\xF6');//对其中32个地板进行着色
				else
					printf("  ");
			}
			else if (board[i][j] == BLACK) {
				printf("○");
			}
			else if (board[i][j] == WHITE) {
				printf("◎");
			}
			else if (board[i][j] == BLACK_KING) {//黑王：用“黑”表示
				printf("黑");
			}
			else if (board[i][j] == WHITE_KING) {//白王：用“白”表示
				printf("白");
			}
		}
		printf("\n");
	}
	printf("\n");
}


//指令函数
void start(int flag) {
	memset(board, 0, sizeof(board));
	int i, j;
	for (i = 0; i < 3; i++)//初始化棋盘状态，布局
		for (j = 0; j < 8; j += 2)
			board[i][j + (i + 1) % 2] = WHITE;

	for (i = 5; i < 8; i++)
		for (j = 0; j < 8; j += 2)
			board[i][j + (i + 1) % 2] = BLACK;

}
void place(struct Command cmd) {
	int x_temp, y_temp;
	int x_mid, y_mid;
	int i,j; 
	for (i = 0; i < cmd.numStep - 1; i++) {
		x_temp = cmd.x[i];
		y_temp = cmd.y[i];

		board[cmd.x[i + 1]][cmd.y[i + 1]] = board[x_temp][y_temp];
		board[cmd.x[i]][cmd.y[i]] = EMPTY;
		//对方下棋记录（回合记录）
		if (abs(cmd.x[i] - cmd.x[i + 1]) == 2) {//吃子认定
			x_mid = (cmd.x[i] + cmd.x[i + 1]) / 2;
			y_mid = (cmd.y[i] + cmd.y[i + 1]) / 2;
			board[x_mid][y_mid] = EMPTY;
		}
	}
	//变王
	for (j = 1; j <= BOARD_SIZE - 1; j += 2) {
		if (board[0][j] == BLACK)
			board[0][j] = BLACK_KING;
	}
	for (j = 0; j <= BOARD_SIZE - 2; j += 2) {
		if (board[BOARD_SIZE - 1][j] == WHITE)
			board[BOARD_SIZE - 1][j] = WHITE_KING;
	}
}
void turn() {
	struct Command command = aiTurn((int(*)[BOARD_SIZE])board);
	place(command);

	printf("%d", command.numStep);
	int i;
	for (i = 0; i < command.numStep; i++) {
		printf(" %d,%d", command.x[i], command.y[i]);
	}

	printf("\n");
	printBoard();
	printf("\n");

	fflush(stdout);
}

//IO循环（死循环直至博弈结束）
void loop() {
	char tag[10] = { 0 };
	char buffer[MAX_BYTE + 1] = { 0 };

	struct Command command;
	while (TRUE) {
		memset(tag, 0, sizeof(tag));
		memset(buffer, 0, sizeof(buffer));
		scanf("%s", tag);//平台发送指令

		if (strcmp(tag, START) == 0) {
			scanf("%d", &me_flag);//输入我方棋子颜色，我方为1时为黑棋，我方为2时为白棋（黑子先手）
			other_flag = (me_flag == WHITE) ? BLACK : WHITE;
			start(me_flag);//初始布局棋盘
			printf("AI is ready!\n");//响应OK
			printBoard();
			fflush(stdout);
		}
		else if (strcmp(tag, PLACE) == 0) {
			scanf("%d", &command.numStep);
			int i;
			for (i = 0; i < command.numStep; i++) {
				scanf("%d,%d", &command.x[i], &command.y[i]);
			}
			place(command);
			printBoard();
			
		}
		else if (strcmp(tag, TURN) == 0) {
			turn();
		}
	}
}

int main(int argc, char* argv[]) {
	loop();
	return 0;
}
