#include "queue.h"
#include <string.h>
#include <stdio.h>

/********** public functions **********************/
void Node_init(Node *self, Customer data){
	printf("Node initing\n");
	self->data = data;
	self->next = NULL;
}

void Queue_init(Queue *self, int maxSize) {
	// job will take form: "name,offset,mech,oil"
	
	self->first = NULL;
	self->last  = NULL;
	self->maxSize = maxSize;
	self->currSize = 0;
}

void printQueue(Queue *self) {
	int i;
	if(queueIsEmpty(self)) { return; }
	Node *curr = self->first;
	printf("[");
	for(i=0;i<=(self->currSize-1);i++) {
		printf("%s, ", getName(&curr->data));
		curr = curr->next;
	}
	printf("]\n");
}

void enqueue(Queue *self, Customer data) {
	Node node;
	Node_init(&node, data);
	if(queueIsEmpty(self)) {
		self->currSize++;
		self->first = &node;
		self->last  = &node;
	}
	else if (self->maxSize == 0 || self->currSize < self->maxSize) {
//		self->last->next = node;
		self->last = &node;
		self->currSize++;
	}
	else {
		printf("Attempted to enqueue %s, but queue is too long\n",
			getName(&data));
		printQueue(self);
	}
	printf("Enqueue'd\n");
}

Customer dequeue(Queue *self) {
	Customer tmp = getCustomer(self->first);
	self->first = self->first->next;
	self->currSize--;
	return tmp;	
}

Customer getCustomer(Node *self) {
	return self->data;
}

// Begin private methods
int queueIsEmpty(Queue *self) {
	if(self->currSize == 0) {
		return 1;
	}
	return 0;
}
