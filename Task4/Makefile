CC = gcc
CFLAGS = -g -O2

all: Task4

Task4: echo pwd ls ln wc cat

echo: echo.c
	$(CC) $(CFLAGS) -o echo echo.c

pwd: pwd.c
	$(CC) $(CFLAGS) -o pwd pwd.c

ls: ls.c
	$(CC) $(CFLAGS) -o ls ls.c

ln: ln.c
	$(CC) $(CFLAGS) -o ln ln.c

wc: wc.c
	$(CC) $(CFLAGS) -o wc wc.c

cat: cat.c
	$(CC) $(CFLAGS) -o cat cat.c

clean:
	rm -f echo pwd ls ln wc cat

.PHONY: all clean