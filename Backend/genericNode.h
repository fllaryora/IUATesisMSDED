#ifndef _GENERIC_NODE_H_
#define _GENERIC_NODE_H_
#include "jsonHelper.h"

extern void genericNode(int myIdNodo);

extern void receiveQueue(Queue *queue);
extern void receiveCounter(Counter *counter);
extern void receiveFunction(Function *function);
extern void receiveNormal(Normal *normal);
extern void receiveCombi(Combi *combis);

#endif /* #ifndef _GENERIC_NODE_H_*/

