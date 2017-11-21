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
extern Customer *dequeue(Queue *self);
extern Customer *peek(Queue *self);
extern int queueIsEmpty(Queue *self);
extern int queueIsFull(Queue *self);
#endif
