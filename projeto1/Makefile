CC = gcc

CFLAGSCLIENT = -g -Wall

CFLAGSSERVER = -g -Wall -arch x86_64 -arch arm64 -Os -DDSQLITE_ENABLE_SESSION -DSQLITE_ENABLE_FTS4 -DSQLITE_ENABLE_RTREE -DSQLITE_DQS=0 -DSQLITE_ENABLE_COLUMN_METADATA

build: client server

clean: clean-client clean-server

client: client/src/client.c
	$(CC) $(CFLAGSCLIENT) -o client/bin/client client/src/client.c

run-client: client
	./client/bin/client

clean-client:
	rm -rf client/bin/*

server: server/src/server.c server/src/database.c
	$(CC) $(CFLAGSSERVER) -o server/bin/server server/src/server.c server/src/database.c -l sqlite3

database: server/src/database.c
	$(CC) $(CFLAGSSERVER) server/src/database.c -l sqlite3 -o server/bin/database

run-sever: server 
	./server/bin/server

clean-server:
	rm -rf server/bin/*