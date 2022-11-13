CC = gcc
CFLAGS = -Wall -Wextra -Wuninitialized -g
LDFLAGS = -lm

prog: main.c mt19937ar.c
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)
