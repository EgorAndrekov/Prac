CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -g

OBJ=main.o list.o tree.o exec.o

my_shell: $(OBJ)
	$(CC) $(CFLAGS) -o my_shell $(OBJ)

clean:
	rm -f *.o my_shell
