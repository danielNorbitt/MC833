CC = gcc

CFLAGS = -g -Wall -arch x86_64 -arch arm64 -Os -DDSQLITE_ENABLE_SESSION -DSQLITE_ENABLE_FTS4 -DSQLITE_ENABLE_RTREE -DSQLITE_DQS=0 -DSQLITE_ENABLE_COLUMN_METADATA

all: server run

server: src/server.c src/database.c
	$(CC) $(CFLAGS) -o bin/server src/server.c src/database.c -l sqlite3

database: src/database.c
	$(CC) $(CFLAGS) src/database.c -l sqlite3 -o bin/database

run: bin/server
	./bin/server

clean:
	rm -rf bin/*