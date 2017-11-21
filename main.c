#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "scanner.h"
#include "customer.h"
#include "queue.h"

Queue parkingLot = { .first=NULL, .last=NULL, .maxSize=0, .currSize=0 };
Queue oilLot = { .first=NULL, .last=NULL, .maxSize=0, .currSize=0 };

pthread_mutex_t mutex;
pthread_mutex_t oilMutex;
pthread_cond_t  oilWaiting;
pthread_cond_t  mechanicWaiting;
pthread_cond_t  customerWaiting;
pthread_cond_t  customerWaitingOil;
pthread_cond_t  customerDone;

void *job(void *parkingLot);
void *mechanicThread(void *parkingLot);
void *oilThread(void *args);

int mechanicActive = 0;

int main() {
	Customer c, e;
	char *line;
	int lotSize, customerThreads, i, prevOffset;
	Queue customers;
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&oilMutex, NULL);
	pthread_cond_init(&mechanicWaiting, NULL);
	pthread_cond_init(&customerWaiting, NULL);
	pthread_cond_init(&customerWaitingOil, NULL);

//	Handle the file read all at once
	FILE *fp = fopen("jobcard","r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open input file\n");
	}
//	Read the lot size
	lotSize = readInt(fp);
	Queue_init(&customers, 0);
// 	Finish the line
	line = readLine(fp);
//	Read the first job
	line = readLine(fp);
// 	Create a customer from first job, enqueue it, then loop until EOF
	while(!feof(fp)) {
		Customer *tmp = (Customer*)malloc(sizeof(Customer));
		Customer_init(tmp, line);
		enqueue(&customers, *tmp);
		line = readLine(fp);
	}
	fclose(fp);
// 	Finished reading file!

// 	Initialize the mechanic parking lot with the lotSize
	Queue_init(&parkingLot, lotSize+1);
//	Initialize enough ids for the customers, mechanic, and oil threads
	pthread_t threads[customers.currSize+2];
	pthread_create(&threads[0], NULL, mechanicThread, NULL);
	pthread_create(&threads[1], NULL, oilThread, NULL);

//	Process the queue, spawning Customer threads with a sleep
//	between each calculated by the offset
	customerThreads = customers.currSize;
	Customer *arg = dequeue(&customers);
	prevOffset = 0;
	for(i=2;i<customerThreads+2;i++) {
		sleep(getOffsetTime(arg) - prevOffset);
		pthread_create(&threads[i], NULL, job, arg);
		prevOffset=getOffsetTime(arg);
		if(!queueIsEmpty(&customers)){
			arg = dequeue(&customers);
		}
	}

	for(i=2;i<customerThreads+2;i++) {
		pthread_join(threads[i], NULL);
	}
//	pthread_join(threads[0], NULL);
	return 1;
}

void *job(void *args) {
	
	Customer *customer = args;
	pthread_mutex_lock(&mutex);
	printf("Customer %s - (MA) arrival\n", getName(args));
	if(queueIsFull(&parkingLot)){
		printf("Customer %s - (MZ) leaves busy car maintenance shop\n", getName(args));
		pthread_mutex_unlock(&mutex);
		return (void*) 1;
	}
	else {
		enqueue(&parkingLot, *customer);
	}
	if(mechanicActive != 0) {
		printf("Customer %s - (MB1) is waiting for mechanic\n", getName(args));
		pthread_cond_wait(&mechanicWaiting, &mutex);
		pthread_cond_signal(&customerWaiting);
		printf("waited and signaled\n");
		printf("Customer %s - (MB2) is no longer waiting for mechanic; notified by %s\n", getName(args), getName(peek(&parkingLot)));
	}
	else {
		pthread_cond_signal(&customerWaiting);
	}
	pthread_mutex_unlock(&mutex);
	
	pthread_mutex_lock(&mutex);
	// enqueue the customer into the 
	pthread_mutex_unlock(&mutex);

	return (void*) 0;
}

void *mechanicThread(void *args) {
	while(1) {
		Customer *servicing;
		pthread_mutex_lock(&mutex);
		pthread_cond_signal(&mechanicWaiting);
		if(queueIsEmpty(&parkingLot)){
			pthread_cond_wait(&customerWaiting, &mutex);
		}
		else {
			pthread_cond_signal(&mechanicWaiting);
		}
		servicing = peek(&parkingLot);
		mechanicActive = 1;
		pthread_mutex_unlock(&mutex);

		printf("Customer %s - (MC) is being serviced by the mechanic for %d second(s)\n",getName(servicing), getMechanicTime(servicing));
		sleep(getMechanicTime(servicing));

		pthread_mutex_lock(&mutex);
		mechanicActive = 0;
		dequeue(&parkingLot);
		if(!queueIsEmpty(&parkingLot)) {
			printf("Customer %s - (MC1) notifying customer %s that they are next for mechanic\n",
				 getName(servicing), getName(peek(&parkingLot)));
			pthread_cond_signal(&mechanicWaiting);
		}
		printf("Customer %s - (MD) is done with mechanic\n", getName(servicing));
		if(getOilTime(servicing) == 0) {
			printf("Customer %s - (ME) is leaving shop\n", getName(servicing));
		}
		else {
			pthread_mutex_lock(&oilMutex);
			enqueue(&oilLot, *servicing);
			pthread_cond_signal(&customerWaitingOil);
			pthread_mutex_unlock(&oilMutex);
		}
		printf("unlocking\n");
		pthread_mutex_unlock(&mutex);
	}
}

void *oilThread(void *args) {
	while(1) {
		Customer *servicing;
		pthread_mutex_lock(&oilMutex);
		pthread_cond_wait(&customerWaitingOil, &oilMutex);
		if(oilLot.currSize >= 1) {
			printf("Customer %s - (MBL1) is waiting for oil change tech \n", getName(servicing));
			pthread_cond_wait(&customerWaitingOil, &oilMutex);
			printf("Customer %s - (MBL2) is waiting for oil change tech; notified by %s\n", 
				getName(servicing), getName(peek(&oilLot)));
		}
		servicing = dequeue(&oilLot);
		pthread_mutex_unlock(&oilMutex);

		printf("Customer %s - (MCL) is being serviced by the oil change tech for %d second(s)\n", getName(servicing), getOilTime(servicing));
		sleep(getOilTime(servicing));
	

		pthread_mutex_lock(&oilMutex);
		if(queueIsEmpty(&oilLot)) {
			printf("Customer %s - (MCL1) notifying customer %s that they are next for oil change tech\n", 
					getName(servicing), getName(peek(&oilLot)));
			pthread_cond_signal(&oilWaiting);
		} 
		pthread_mutex_unlock(&oilMutex);
		printf("Customer %s - (ML) is done with oil change tech \n", getName(servicing));
		return (void*) 0;
	}
}
