#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "customer.h"
#include "queue.h"

int main() {
	Customer c, e;
	char *line;
	int lotSize;
	Queue customers;
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
// 	Create a customer from first job
	while(!feof(fp)) {
		printf("Checking a new job\n");
		Customer *tmp = (Customer*)malloc(sizeof(Customer));
		Customer_init(tmp, line);
		enqueue(&customers, *tmp);
		line = readLine(fp);
	}
	fclose(fp);
// 	Print out queue to make sure it was complete correctly
	printQueue(&customers);

	Customer d = dequeue(&customers);
//	printf("Dequeued\n");
//	printf("Popping: %s\n", getName(&d));
	printQueue(&customers);
	return 1;
}
