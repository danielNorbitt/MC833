#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define IP "127.0.0.1"
#define MAX_PROFILE_INFO 51
#define MAX_SKILLS 201

#define BUFFER_LEN (MAX_PROFILE_INFO * 6) + MAX_SKILLS

void take_argument(char*);

void show_menu();

int connect_server();

int send_request(char cmd, char* arg, char* ip, int port);

void add_user();