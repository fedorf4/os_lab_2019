CC=gcc
CFLAGS=-I.
all : client server

MultModul.o: MultModul.h
	$(CC) -o MultModul.o -c MultModul.c $(CFLAGS)
	
client : MultModul.o MultModul.h
	$(CC) -o client MultModul.o client.c -lpthread $(CFLAGS)
	
server: MultModul.o MultModul.h
	$(CC) -o server MultModul.o server.c -lpthread $(CFLAGS)
	
clean:
	rm -f client server MultModul.o