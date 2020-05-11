#include "alpha_beta_pruning.h"

#pragma warning(disable:4996)
int board[BOARD_SIZE][BOARD_SIZE] = { 0 };
int me_flag;
int other_flag;
/***************************************************************************************
	IO˵����
	����������ָ�
	1.START ָ��ڿ���ʱ���룺START AI������meflag��1Ϊ�ڷ���2Ϊ�׷���
	  �磺
	  START 1 
	  Ϊ��ʼ�����棬�ڷ�ΪAI��

	  Ĭ�Ϻڷ�������

    2.PLACE ָ�Ϊ�Լ����ӣ�PLACE �������� ÿһ����Ӧ������
	  �磺PLACE 3 6,3 4,5 2,3
	  ��ʾ���ӿ������� ��6,3����4,5������2,3�����˵з������ӣ�
	  �磺PLACE 2 1,0 2,1
	  ��ʾ���Ӵ�1,0�ߵ�2,1

	3.TURNָ�����TURNָ���AI����

****************************************************************************************/

//��ӡ����
void printBoard() {
	printf("    0 1 2 3 4 5 6 7\n");
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		printf(" %d ", i);
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == EMPTY) {
				if ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))
					printf("%c%c", '\xA1', '\xF6');//������32���ذ������ɫ
				else
					printf("  ");
			}
			else if (board[i][j] == BLACK) {
				printf("��");
			}
			else if (board[i][j] == WHITE) {
				printf("��");
			}
			else if (board[i][j] == BLACK_KING) {//�������á��ڡ���ʾ
				printf("��");
			}
			else if (board[i][j] == WHITE_KING) {//�������á��ס���ʾ
				printf("��");
			}
		}
		printf("\n");
	}
	printf("\n");
}


//ָ���
void start(int flag) {
	memset(board, 0, sizeof(board));
	int i, j;
	for (i = 0; i < 3; i++)//��ʼ������״̬������
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
		//�Է������¼���غϼ�¼��
		if (abs(cmd.x[i] - cmd.x[i + 1]) == 2) {//�����϶�
			x_mid = (cmd.x[i] + cmd.x[i + 1]) / 2;
			y_mid = (cmd.y[i] + cmd.y[i + 1]) / 2;
			board[x_mid][y_mid] = EMPTY;
		}
	}
	//����
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

//IOѭ������ѭ��ֱ�����Ľ�����
void loop() {
	char tag[10] = { 0 };
	char buffer[MAX_BYTE + 1] = { 0 };

	struct Command command;
	while (TRUE) {
		memset(tag, 0, sizeof(tag));
		memset(buffer, 0, sizeof(buffer));
		scanf("%s", tag);//ƽ̨����ָ��

		if (strcmp(tag, START) == 0) {
			scanf("%d", &me_flag);//�����ҷ�������ɫ���ҷ�Ϊ1ʱΪ���壬�ҷ�Ϊ2ʱΪ���壨�������֣�
			other_flag = (me_flag == WHITE) ? BLACK : WHITE;
			start(me_flag);//��ʼ��������
			printf("AI is ready!\n");//��ӦOK
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
