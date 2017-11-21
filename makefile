CC=gcc
CFLAGS=-I. -lpthread
DEPS = scanner.h customer.h queue.h
OBJ = main.o scanner.o customer.o queue.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONE: clean

clean: 
	rm $(OBJ)
