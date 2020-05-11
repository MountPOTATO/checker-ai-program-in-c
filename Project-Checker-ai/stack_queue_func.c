#include "marco_struct.h"
/****************************************栈和队列基本操作的声明***********************************************/
//栈的基本操作
/**************************************
*函数操作：初始化一个链表栈
*前提条件：声明一个栈
*后置条件：栈被成功初始化,返回true
**************************************/
bool InitLinkStack(LinkStack* S) {//初始化链表栈
	S->base = (StackLinkNode*)malloc(sizeof(StackLinkNode));
	if (S->base == NULL)//未成功初始化
		exit(-1);
	S->top = S->base;
	return true;
}



/**************************************
*函数操作：将一个NodeMovement结构体推入栈
*前提条件：声明一个栈
*后置条件：NodeMovement由栈顶推入
		  成功返回true,不成功则退出
**************************************/
bool pushStack(LinkStack* S, struct NodeMovement plan) {//将元素放入链表栈
	if (S->base == NULL)
		exit(-1);
	S->top->Smove = plan;
	StackLinkNode* pnew = (StackLinkNode*)malloc(sizeof(StackLinkNode));
	if (pnew == NULL)
		exit(-1);
	pnew->next = NULL;
	S->top->next = pnew;//连接
	S->top = pnew;
	return true;
}



/**************************************
*函数操作：读取栈顶顶部的NodeMovement
*前提条件：栈不为空（为空则退出）
*后置条件：返回栈顶的NodeMovement
**************************************/
struct NodeMovement getTop(LinkStack* S) {//获得链表栈顶部元素
	if (S->top == S->base) exit(-1);//空栈，退出
	StackLinkNode* p = S->base;//从栈底遍历到栈顶
	while (p->next != S->top) {
		p = p->next;
	}
	return p->Smove;
}



/**************************************
*函数操作：将栈顶NodeMovement取出
*前提条件：栈不为空（为空则退出）
*后置条件：返回栈顶的NodeMovement
		  栈状态更新为取出该走法单元后的状态
**************************************/
struct NodeMovement popStack(LinkStack* S) {//顶部元素出栈，返回顶部元素
	if (S->top == S->base) exit(-1);//空栈，退出
	StackLinkNode* p = S->base;//从栈底遍历到栈顶
	while (p->next != S->top) {
		p = p->next;
	}
	struct NodeMovement plan = p->Smove;
	StackLinkNode* temp = p->next;
	p->next = NULL;
	S->top = p;
	free(temp);
	temp = NULL;
	return plan;
}



/**************************************
*函数操作：判断栈是否为空栈
*前提条件：声明一个栈
*后置条件：为空返回true，不为空返回false
**************************************/
bool StackEmpty(LinkStack* S) {
	if (S->base == NULL) exit(-1);//不存在栈时退出
	return (S->top == S->base) ? true : false;
}




/**************************************
*函数操作：摧毁一个空栈（释放首尾指针）
*前提条件：栈为空
*后置条件：指针被释放
**************************************/
void DestroyStack(LinkStack* S) {//清空栈后摧毁栈
	free(S->base);
	S->base = NULL;
	S->top = NULL;
}



//队列的基本操作
/**************************************
*函数操作：初始化一个双向队列
*前提条件：队列已声明
*后置条件：队列被初始化
**************************************/
void InitLinkQueue(LinkQueue* Q) {//初始化双向队列
	Q->front = Q->rear = NULL;
	Q->count = 0;
}



/**************************************
*函数操作：判断队列是否为空
*前提条件：队列被声明
*后置条件：为空返回true，不为空返回false
**************************************/
bool QueueEmpty(const LinkQueue* Q) {//判断双向队列是否为空
	return Q->count == 0;
}



/**************************************
*函数操作：计算队列有几项
*前提条件：队列存在
*后置条件：返回队列中NodeMovement的个数
**************************************/
int QueueItemCount(const LinkQueue* Q) {//数队列一共几项
	return Q->count;
}




/**************************************
*函数操作：将NodeMovement由栈顶推入队列中
*前提条件：队列存在
*后置条件：成功，返回true；否则退出
**************************************/
bool pushQueue(struct NodeMovement plan, LinkQueue* Q) {//将元素推入队列中
	QueueLinkNode* pnew;
	//满，退出
	pnew = (QueueLinkNode*)malloc(sizeof(QueueLinkNode));
	if (pnew == NULL)
		exit(-1);
	pnew->next = NULL;
	if (QueueEmpty(Q))//为空时 衔接到队列首端
		Q->front = pnew;
	else //不为空时，衔接到队列尾端
		Q->rear->next = pnew;
	Q->rear = pnew;
	pnew->Qmove = plan;
	(Q->count)++;
	return true;
}




/**************************************
*函数操作：推出队列底部的NodeMovement（front方向）
*前提条件：队列不为空
*后置条件：返回底部NodeMovement元素
		  队列状态更新为推出后的状态
**************************************/
struct NodeMovement popFrontQueue(LinkQueue* Q) {//提取出双向队列底部元素
	QueueLinkNode* pt;
	if (QueueEmpty(Q))
		exit(-1);
	pt = Q->front;
	Q->front = Q->front->next;
	struct NodeMovement plan = pt->Qmove;
	free(pt);
	(Q->count)--;
	if (Q->count == 0)
		Q->rear = NULL;
	return plan;
}


/**************************************
*函数操作：推出队列顶部的NodeMovement（rear方向）
*前提条件：队列不为空
*后置条件：返回顶部NodeMovement元素
		  队列状态更新为推出后的状态
**************************************/
struct NodeMovement popRearQueue(LinkQueue* Q) {//提取出双向队列顶部元素
	if (QueueEmpty(Q))
		exit(-1);

	if (Q->front == Q->rear) {
		struct NodeMovement plan = Q->front->Qmove;
		Q->front = NULL;
		Q->rear = NULL;
		Q->count = 0;
		return plan;
	}
	else {
		QueueLinkNode* pt;
		pt = Q->front;
		while (pt->next != Q->rear)
			pt = pt->next;
		struct NodeMovement plan = Q->rear->Qmove;
		QueueLinkNode* temp = pt->next;
		pt->next = NULL;
		Q->rear = pt;
		free(temp);
		(Q->count)--;
		if (Q->count == 0)
			Q->rear = NULL;
		return plan;
	}

}



/**************************************
*函数操作：清空队列并释放首尾指针
*前提条件：队列已声明
*后置条件：队列被清空，首尾指针释放
**************************************/
void DestroyQueue(LinkQueue* Q) {//摧毁队列清空内存
	while (Q->front != NULL) {
		Q->rear = Q->front->next;
		free(Q->front);
		Q->front = Q->rear;
	}
}



/**************************************
*函数操作：将原本的队列（original）
		  复制到新队列（copy）中
*前提条件：original存在
*后置条件：获得一个一致的队列
**************************************/
void copyQueue(LinkQueue* copy, LinkQueue* original) {//将origin队列复制到copy队列
	QueueLinkNode* p = original->front;
	copy->count = 0;
	while (p->next != NULL) {
		pushQueue(p->Qmove, copy);
		p = p->next;
	}
	pushQueue(p->Qmove, copy);
	copy->rear = p;
	p->next = NULL;
	copy->count = original->count;
}

