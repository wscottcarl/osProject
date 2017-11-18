CC=gcc
CFLAGS=-I.
DEPS = scanner.h customer.h queue.h
OBJ = main.o scanner.o customer.o queue.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLGAS)

.PHONE: clean

clean: 
	rm $(OBJ)
