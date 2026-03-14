CC = gcc
CFLAGS = -Wall -Wextra -g
TARGETS = s c

all: $(TARGETS)

s: server.c
	$(CC) $(CFLAGS) -o s server.c

c: client.c
	$(CC) $(CFLAGS) -o c client.c

clean:
	rm -f $(TARGETS)

.PHONY: all clean