CC=gcc
CFLAGS=-Wall -pedantic

main : main.c
	$(CC) $(CFLAGS) -o main main.c  
