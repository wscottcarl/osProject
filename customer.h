#ifndef CUSTOMER_H
#define CUSTOMER_H
/*
 * CUSTOMER.h - public interface to customer.c, the customer object
 */
typedef struct {
	char *name;
	int offset;
	int mechanicTime;
	int oilTime;
} Customer;

extern void Customer_init(Customer *self, char*name);
extern char *getName(Customer *self);
extern int getMechanicTime(Customer *self);
extern int getOffsetTime(Customer *self);
extern int getOilTime(Customer *self);
//extern void setOilTime(Customer *self, int time);
//extern void setOffsetTime(Customer *self, int time);
//extern void setMechanicTime(Customer *self, int time);
#endif
