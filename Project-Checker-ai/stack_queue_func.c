#include "marco_struct.h"
/****************************************ջ�Ͷ��л�������������***********************************************/
//ջ�Ļ�������
/**************************************
*������������ʼ��һ������ջ
*ǰ������������һ��ջ
*����������ջ���ɹ���ʼ��,����true
**************************************/
bool InitLinkStack(LinkStack* S) {//��ʼ������ջ
	S->base = (StackLinkNode*)malloc(sizeof(StackLinkNode));
	if (S->base == NULL)//δ�ɹ���ʼ��
		exit(-1);
	S->top = S->base;
	return true;
}



/**************************************
*������������һ��NodeMovement�ṹ������ջ
*ǰ������������һ��ջ
*����������NodeMovement��ջ������
		  �ɹ�����true,���ɹ����˳�
**************************************/
bool pushStack(LinkStack* S, struct NodeMovement plan) {//��Ԫ�ط�������ջ
	if (S->base == NULL)
		exit(-1);
	S->top->Smove = plan;
	StackLinkNode* pnew = (StackLinkNode*)malloc(sizeof(StackLinkNode));
	if (pnew == NULL)
		exit(-1);
	pnew->next = NULL;
	S->top->next = pnew;//����
	S->top = pnew;
	return true;
}



/**************************************
*������������ȡջ��������NodeMovement
*ǰ��������ջ��Ϊ�գ�Ϊ�����˳���
*��������������ջ����NodeMovement
**************************************/
struct NodeMovement getTop(LinkStack* S) {//�������ջ����Ԫ��
	if (S->top == S->base) exit(-1);//��ջ���˳�
	StackLinkNode* p = S->base;//��ջ�ױ�����ջ��
	while (p->next != S->top) {
		p = p->next;
	}
	return p->Smove;
}



/**************************************
*������������ջ��NodeMovementȡ��
*ǰ��������ջ��Ϊ�գ�Ϊ�����˳���
*��������������ջ����NodeMovement
		  ջ״̬����Ϊȡ�����߷���Ԫ���״̬
**************************************/
struct NodeMovement popStack(LinkStack* S) {//����Ԫ�س�ջ�����ض���Ԫ��
	if (S->top == S->base) exit(-1);//��ջ���˳�
	StackLinkNode* p = S->base;//��ջ�ױ�����ջ��
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
*�����������ж�ջ�Ƿ�Ϊ��ջ
*ǰ������������һ��ջ
*����������Ϊ�շ���true����Ϊ�շ���false
**************************************/
bool StackEmpty(LinkStack* S) {
	if (S->base == NULL) exit(-1);//������ջʱ�˳�
	return (S->top == S->base) ? true : false;
}




/**************************************
*�����������ݻ�һ����ջ���ͷ���βָ�룩
*ǰ��������ջΪ��
*����������ָ�뱻�ͷ�
**************************************/
void DestroyStack(LinkStack* S) {//���ջ��ݻ�ջ
	free(S->base);
	S->base = NULL;
	S->top = NULL;
}



//���еĻ�������
/**************************************
*������������ʼ��һ��˫�����
*ǰ������������������
*�������������б���ʼ��
**************************************/
void InitLinkQueue(LinkQueue* Q) {//��ʼ��˫�����
	Q->front = Q->rear = NULL;
	Q->count = 0;
}



/**************************************
*�����������ж϶����Ƿ�Ϊ��
*ǰ�����������б�����
*����������Ϊ�շ���true����Ϊ�շ���false
**************************************/
bool QueueEmpty(const LinkQueue* Q) {//�ж�˫������Ƿ�Ϊ��
	return Q->count == 0;
}



/**************************************
*������������������м���
*ǰ�����������д���
*�������������ض�����NodeMovement�ĸ���
**************************************/
int QueueItemCount(const LinkQueue* Q) {//������һ������
	return Q->count;
}




/**************************************
*������������NodeMovement��ջ�����������
*ǰ�����������д���
*�����������ɹ�������true�������˳�
**************************************/
bool pushQueue(struct NodeMovement plan, LinkQueue* Q) {//��Ԫ�����������
	QueueLinkNode* pnew;
	//�����˳�
	pnew = (QueueLinkNode*)malloc(sizeof(QueueLinkNode));
	if (pnew == NULL)
		exit(-1);
	pnew->next = NULL;
	if (QueueEmpty(Q))//Ϊ��ʱ �νӵ������׶�
		Q->front = pnew;
	else //��Ϊ��ʱ���νӵ�����β��
		Q->rear->next = pnew;
	Q->rear = pnew;
	pnew->Qmove = plan;
	(Q->count)++;
	return true;
}




/**************************************
*�����������Ƴ����еײ���NodeMovement��front����
*ǰ�����������в�Ϊ��
*�������������صײ�NodeMovementԪ��
		  ����״̬����Ϊ�Ƴ����״̬
**************************************/
struct NodeMovement popFrontQueue(LinkQueue* Q) {//��ȡ��˫����еײ�Ԫ��
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
*�����������Ƴ����ж�����NodeMovement��rear����
*ǰ�����������в�Ϊ��
*�������������ض���NodeMovementԪ��
		  ����״̬����Ϊ�Ƴ����״̬
**************************************/
struct NodeMovement popRearQueue(LinkQueue* Q) {//��ȡ��˫����ж���Ԫ��
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
*������������ն��в��ͷ���βָ��
*ǰ������������������
*�������������б���գ���βָ���ͷ�
**************************************/
void DestroyQueue(LinkQueue* Q) {//�ݻٶ�������ڴ�
	while (Q->front != NULL) {
		Q->rear = Q->front->next;
		free(Q->front);
		Q->front = Q->rear;
	}
}



/**************************************
*������������ԭ���Ķ��У�original��
		  ���Ƶ��¶��У�copy����
*ǰ��������original����
*�������������һ��һ�µĶ���
**************************************/
void copyQueue(LinkQueue* copy, LinkQueue* original) {//��origin���и��Ƶ�copy����
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

