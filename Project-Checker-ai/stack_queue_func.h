#ifndef _STACK_QUEUE_FUNC_H_
#define _STACK_QUEUE_FUNC_H_
#include "marco_struct.h"

bool InitLinkStack(LinkStack* S);
bool pushStack(LinkStack* S, struct NodeMovement plan);
struct NodeMovement getTop(LinkStack* S);
struct NodeMovement popStack(LinkStack* S);
bool StackEmpty(LinkStack* S);
void DestroyStack(LinkStack* S);



void InitLinkQueue(LinkQueue* Q);
bool QueueEmpty(const LinkQueue* Q);
int QueueItemCount(const LinkQueue* Q);
bool pushQueue(struct NodeMovement plan, LinkQueue* Q);
struct NodeMovement popFrontQueue(LinkQueue* Q);
struct NodeMovement popRearQueue(LinkQueue* Q);
void DestroyQueue(LinkQueue* Q);
void copyQueue(LinkQueue* copy, LinkQueue* original);

#endif 
