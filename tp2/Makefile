CC = gcc
CFLAGS = -Wall -Wextra -Wuninitialized
LDFLAGS = -lm

prog: mt19937ar.c
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)
