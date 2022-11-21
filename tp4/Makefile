CC = gcc
CFLAGS = -Wall -Wextra -Wuninitialized
LDFLAGS =

prog: main.c mt19937ar.c affichage.c
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS) -pg
