CC=gcc
CFLAGS=-I.

all: process_memory parallel_min_max parallel_sum utils.o find_min_max.o

parallel_sum: parallel_sum.c sum.o utils.o
	@$(CC) -o parallel_sum utils.o sum.o parallel_sum.c -lpthread $(CFLAGS)

process_memory:
	$(CC) process_memory.c -o process_memory $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)
  
sum.o : sum.h sum.c
	$(CC) -o sum.o -c sum.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm utils.o sum.o find_min_max.o process_memory parallel_min_max