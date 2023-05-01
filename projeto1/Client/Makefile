CC = gcc

CFLAGS  = -g -Wall

all: client run

client: src/client.c
	$(CC) $(CFLAGS) -o bin/client src/client.c

run: bin/client
	./bin/client

clean:
	rm -rf bin/*