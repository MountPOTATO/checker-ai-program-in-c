#include <stdio.h>

#define BOARD_SIZE 8 //���̴�С��8*8
//��������״̬��Ϣ���ա����塢���塢����������
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BLACK_KING 3
#define WHITE_KING 4

#define NEB_SPOT (given_map[x + xd[q]][y + yd[q]])
//bool
#define TRUE 1
#define FALSE 0
//alpha-beta��֦��ʼ��ֵ
#define INFINITY 10000

#define MAX_STEP 15
#define MAX_BYTE 10000
//����ָʾ
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"

#define ON 1
#define OFF 0

struct Command {//��������Ľṹ�壺������ҵҪ��ͳһ����������Ҫ���Լ�����Ľṹ��ת��Ϊ�ýṹ��
	int x[MAX_STEP];
	int y[MAX_STEP];
	int numStep;
};

//�߷���Ϣ�ṹ��
typedef struct NodeCord {//����
	int x;
	int y;
}NodeCord;

typedef struct NodeMovement {//�߷���Ԫ������start�����꣬eat�����꣬end������
	NodeCord start;
	NodeCord eat;
	NodeCord end;
}NodeMovement;

//�߷��洢�ṹ�壺ջ��˫�����
typedef struct StackLinkNode {//����ջ��Ԫ
	struct NodeMovement Smove;
	struct StackLinkNode* next;
}StackLinkNode;

typedef struct LinkStack {//����ջ
	struct StackLinkNode* top;
	struct StackLinkNode* base;
}LinkStack;

typedef struct QueueLinkNode {//˫����нڵ�
	struct NodeMovement Qmove;
	struct QueueLinkNode* next;
}QueueLinkNode;
typedef struct LinkQueue {//����˫�����
	QueueLinkNode* front;//ָ����������ָ��
	QueueLinkNode* rear;//ָ�����β���ָ��
	int count;//�����е�����
	int eval;//��ǰ���еĹ�ֵ
}LinkQueue;


int depth;													//alpha_beta��֦���
int board[BOARD_SIZE][BOARD_SIZE] = { 0 };					//ȫ������
int temp_map[BOARD_SIZE][BOARD_SIZE] = { 0 };				//��ʱ���ɵ����
int me_flag;												//�ҷ���1Ϊ�ڣ�2Ϊ��
int other_flag;												//�з���1Ϊ�ڣ�2Ϊ��

//ʱ���¼
int end_times = 0, beginning_times = 0;