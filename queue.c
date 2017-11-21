#include "queue.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/********** public functions **********************/
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
	Node *ptr = (Node *)malloc(sizeof(Node));
	ptr->data = data;
	ptr->next = NULL;
	Customer f, l;
	if(queueIsEmpty(self)) {
		self->currSize++;
		self->first = ptr;
		self->last  = ptr;
	}
	else if (self->maxSize == 0 || self->currSize < self->maxSize) {
		self->last->next = ptr;
		self->last = ptr;
		self->currSize++;
		f = getCustomer(self->first);
		l = getCustomer(self->last);
	}
	else {
		printf("Attempted to enqueue %s, but queue is too long\n",
			getName(&data));
		printQueue(self);
	}
}

Customer *dequeue(Queue *self) {
	Customer *tmp = &(self->first->data);
	self->first = self->first->next;
	self->currSize--;
	return tmp;	
}

Customer *peek(Queue *self) {
	return &(self->first->data);
}

Customer getCustomer(Node *self) {
	return self->data;
}

int queueIsEmpty(Queue *self) {
	if(self->currSize == 0) {
		return 1;
	}
	return 0;
}

int queueIsFull(Queue *self) {
	if(self->maxSize == 0) { return 0; }
	else if(self->currSize == self->maxSize) { return 1; }
	else { return 0; }
}
