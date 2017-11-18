#include <stdio.h>
#include "scanner.h"
#include "customer.h"
#include "queue.h"

int main() {
	Customer c;
	FILE *fp = fopen("jobcard","r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open input file\n");
	}
//	Read the lot size
	int lot_size = readInt(fp);
// 	Finish the line
	char *firstLine = readLine(fp);
//	Read the first job
	char *firstJob = readLine(fp);
// 	Create a customer from first job
	Customer_init(&c, firstJob);
// 	Loop through file creating customers until EOF
/*	while(firstJob != EOF) {
		char *firstJob = readLine(fp);
		Customer_init(&c, firstJob);
	}*/
	fclose(fp);
	
	printf("Name: %s\n", getName(&c));
	printf("Offset: %d\n", getOffsetTime(&c));
	printf("MechTime: %d\n", getMechanicTime(&c));
	printf("Oiltime: %d\n", getOilTime(&c));

	Queue customers;
	Queue_init(&customers, 0);
	printf("Queue init'd\n");
	enqueue(&customers, c);
	enqueue(&customers, c);
//	printf("Enqueued\n");
	printQueue(&customers);
	return 1;
}
