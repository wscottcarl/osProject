#include "customer.h"
#include <string.h>
#include <stdio.h>

/********** public functions **********************/

void Customer_init(Customer *self, char *job) {
	// job will take form: "name,offset,mech,oil"
	//printf("Init'd string: %s\n", job);
	char *tmp = strtok(job, ",");
	self->name = tmp;
	tmp = strtok(NULL, ",");
	self->offset = atoi(tmp);
	tmp = strtok(NULL, ",");
	self->mechanicTime = atoi(tmp);
	tmp = strtok(NULL, ",");
	self->oilTime = atoi(tmp);
	return;
}

char *getName(Customer *self) {
	return self->name;
}

int getOffsetTime(Customer *self) {
	return self->offset;
}

int getOilTime(Customer *self) {
	return self->oilTime;
}

int getMechanicTime(Customer *self) {
	return self->mechanicTime;
}


