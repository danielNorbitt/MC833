#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include "database.h"

#define MAX 4096
#define PORT 8080
#define LISTENMAX 5 
#define SA struct sockaddr

void send_response(int connfd, ListProfile *profile_list);
Profile get_profile_info(char *profile_info);
void router(int connfd, sqlite3* database);