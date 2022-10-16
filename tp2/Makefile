CC = gcc
CFLAGS = -Wall -Wextra -Wuninitialized
LDFLAGS = -lm -g

prog: mt19937ar.c
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf prog
