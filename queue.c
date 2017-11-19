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
		printf("Queue is empty\n");
		self->currSize++;
		self->first = ptr;
		self->last  = ptr;
	}
	else if (self->maxSize == 0 || self->currSize < self->maxSize) {
		printf("Adding to non-empty queue\n");
	self->last->next = ptr;
		self->last = ptr;
		self->currSize++;
		f = getCustomer(self->first);
		printf("Enqueue'd first after: %s\n", getName(&f));
		l = getCustomer(self->last);
		printf("Enqueue'd last after: %s\n", getName(&l));
	}
	else {
		printf("Attempted to enqueue %s, but queue is too long\n",
			getName(&data));
		printQueue(self);
	}
}

Customer dequeue(Queue *self) {
	printf("Dequeuing\n");
	Customer tmp = getCustomer(self->first);
	printf("Got customer: %s\n", getName(&tmp));
	self->first = self->first->next;
	printf("Set new first\n");
	self->currSize--;
	printf("Set size\n");
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
