CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: Task3

Task3: main.o list.o
	$(CC) $(CFLAGS) -o Task3 main.o list.o

main.o: main.c list.h
	$(CC) $(CFLAGS) -c main.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

clean:
	rm -f *.o Task3

.PHONY: all clean