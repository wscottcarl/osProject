#ifndef QUEUE_H
#define QUEUE_H
#include "customer.h"
/*
 * QUEUE.h - public interface to queue.c, a customer queue 
 */
typedef struct Node {
	Customer data;
	struct Node* next;
} Node;

typedef struct Queue {
	struct Node *first;
	struct Node *last;
	int maxSize;
	int currSize;
} Queue;

extern void Queue_init(Queue *self, int maxSize);
extern void printQueue(Queue *self);
extern void enqueue(Queue *self, Customer data);
extern Customer getCustomer(Node *);
extern Customer dequeue(Queue *self);
//extern char *getName(Customer *self);
//extern int getMechanicTime(Customer *self);
//extern int getOffsetTime(Customer *self);
//extern int getOilTime(Customer *self);
//extern void setOilTime(Customer *self, int time);
//extern void setOffsetTime(Customer *self, int time);
//extern void setMechanicTime(Customer *self, int time);
#endif
