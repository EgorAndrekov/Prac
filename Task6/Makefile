CC = gcc
CFLAGS = -g -O2 -std=gnu11

all: l p r

l: light.c
	$(CC) $(CFLAGS) -o l light.c

p: pedestrian.c
	$(CC) $(CFLAGS) -o p pedestrian.c

r: remove.c
	$(CC) $(CFLAGS) -o r remove.c

clean:
	rm -f l p r

.PHONY: all clean