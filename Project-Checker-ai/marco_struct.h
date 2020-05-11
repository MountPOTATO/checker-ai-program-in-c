#ifndef _MARCO_STRUCT_H_
#define _MARCO_STRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#pragma warning(disable:4996)
#define BOARD_SIZE 8 //棋盘大小：8*8
//棋盘坐标状态信息：空、黑棋、白棋、黑王、白王
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BLACK_KING 3
#define WHITE_KING 4

#define NEB_SPOT (given_map[x + xd[q]][y + yd[q]])
//bool
#define TRUE 1
#define FALSE 0
//alpha-beta剪枝初始界值
#define INFINITY 10000

#define MAX_STEP 15
#define MAX_BYTE 10000
//输入指示
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"

#define ON 1
#define OFF 0

struct Command {//最终输出的结构体：由于作业要求统一，故最终需要将自己定义的结构体转换为该结构体
	int x[MAX_STEP];
	int y[MAX_STEP];
	int numStep;
};

//走法信息结构体
typedef struct NodeCord {//坐标
	int x;
	int y;
}NodeCord;

typedef struct NodeMovement {//走法单元，包括start点坐标，eat点坐标，end点坐标
	NodeCord start;
	NodeCord eat;
	NodeCord end;
}NodeMovement;

//走法存储结构体：栈，双向队列
typedef struct StackLinkNode {//链表栈单元
	struct NodeMovement Smove;
	struct StackLinkNode* next;
}StackLinkNode;

typedef struct LinkStack {//链表栈
	struct StackLinkNode* top;
	struct StackLinkNode* base;
}LinkStack;

typedef struct QueueLinkNode {//双向队列节点
	struct NodeMovement Qmove;
	struct QueueLinkNode* next;
}QueueLinkNode;
typedef struct LinkQueue {//链表双向队列
	QueueLinkNode* front;//指向队列首项的指针
	QueueLinkNode* rear;//指向队列尾项的指针
	int count;//队列中的项数
	int eval;//当前队列的估值
}LinkQueue;

extern int board[BOARD_SIZE][BOARD_SIZE];
extern int me_flag;
extern int other_flag;

#endif