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

#define MAX 4000 // Size of buffer
#define PORT 8080
#define LISTENMAX 5 
#define SA struct sockaddr

void send_response(int connfd, ListProfile *profile_list) {
	char response[MAX] = {};

	char temp[MAX];

	if (profile_list->count == 0 || profile_list == NULL){
		sprintf(temp, "No profile found.\n%c",0x04);
		send(connfd,temp,19,0);
		return;
	}

	for (int i = 0; i < profile_list->count; i++){
		sprintf(temp, "Email: %s\n", profile_list->list[i].email);
		strcat(response,temp);
		sprintf(temp, "FirstName: %s LastName: %s\n",profile_list->list[i].first_name,profile_list->list[i].last_name);
		strcat(response,temp);

        if (profile_list->list[i].city[0] != '\0'){
            sprintf(temp, "City: %s\n", profile_list->list[i].city);
            strcat(response,temp);
        }
        if (profile_list->list[i].year[0] != '\0'){
            sprintf(temp, "Course: %s\n", profile_list->list[i].course);
            strcat(response,temp);
        }

		if (profile_list->list[i].year[0] != '\0'){
            sprintf(temp, "Course: %s\n",profile_list->list[i].year);
            strcat(response,temp);
        }

        if (profile_list->list[i].skills[0] != '\0') {
            sprintf(temp, "Skills: %s\n", profile_list->list[i].skills);
            strcat(response,temp);
        }

		if(i == profile_list->count - 1){
			sprintf(temp,"%c", 0x04);
			strcat(response,temp);
		}
	}

	printf("%s",response);
	
	send(connfd,response, strlen(response),0);

	free(profile_list->list);
	free(profile_list);
}

Profile get_profile_info(char *profile_info) {
	Profile profile;
	printf("%s\n",profile_info);
	char *ptr = strtok(profile_info, "\n");
	printf("aki%s\n",ptr);
	strcpy(profile.email,ptr);
	ptr = strtok(profile_info, "\n");
	strcpy(profile.first_name,ptr);
	ptr = strtok(profile_info, "\n");
	strcpy(profile.last_name,ptr);
	ptr = strtok(profile_info, "\n");
	strcpy(profile.city,ptr);
	ptr = strtok(profile_info, "\n");
	strcpy(profile.course,ptr);
	ptr = strtok(profile_info, "\n");
	strcpy(profile.year,ptr);
	ptr = strtok(profile_info, "\n");
	strcpy(profile.skills,ptr);
	return profile;
}

// Interpret the client command and return to response to client
void router(int connfd, sqlite3* database) { 

	//Start chatting with client
	char buffer[MAX]; 

	// Reset buffer
	bzero(buffer, MAX); 

	// read the message from client and copy it in buffer 
	read(connfd, buffer, sizeof(buffer));

	printf("Command from client : %c\n", buffer[0]); 

	char parameter[MAX_PROFILE_INFO];
	char profile_info[MAX];
	char temp[MAX];
	Profile profile;

	switch (buffer[0]){
	case '1':
		memcpy(&profile_info, &buffer[1], MAX);
		profile_info[MAX_PROFILE_INFO-1] = '\0';
		profile = get_profile_info(profile_info);
		if (add_profile(database,profile)){
			sprintf(temp, "Profile removed sucessuly.\n%c",0x04);
			send(connfd,temp,sizeof(temp),0);
		}
		else {
			sprintf(temp, "Profile removed sucessuly.\n%c",0x04);
			send(connfd,temp,sizeof(temp),0);
		}
		break;
	case '2':
		memcpy(&parameter, &buffer[1], MAX_PROFILE_INFO);
		parameter[MAX_PROFILE_INFO-1] = '\0';
		send_response(connfd,find_by_course(database, parameter));
		break;
	case '3':
		memcpy(&parameter, &buffer[1], MAX_PROFILE_INFO);
		parameter[MAX_PROFILE_INFO-1] = '\0';
		send_response(connfd,find_by_skill(database, parameter));
		break;
	case '4':
		memcpy(&parameter, &buffer[1], MAX_PROFILE_INFO);
		parameter[MAX_PROFILE_INFO-1] = '\0';
		send_response(connfd,find_by_year(database, parameter));
		break;
	case '5':
		send_response(connfd,get_all(database));
		break;
	case '6':
		memcpy(&parameter, &buffer[1], MAX_PROFILE_INFO);
		parameter[MAX_PROFILE_INFO-1] = '\0';
		send_response(connfd,find_by_email(database, parameter));
		break;
	case '7':
		memcpy(&parameter, &buffer[1], MAX_PROFILE_INFO);
		parameter[MAX_PROFILE_INFO-1] = '\0';
		if (remove_by_email(database, parameter)){
			sprintf(temp, "Profile removed sucessuly.\n%c",0x04);
			send(connfd,temp,sizeof(temp),0);
		}
		else {
			sprintf(temp, "Can't remove the profile.\n%c",0x04);
			send(connfd,temp,sizeof(temp),0);
		}
		break;
	default:
		printf("Command not found\n");
		break;
	}
} 

int main() { 
	
	sqlite3 *database = init_database("src/database.db");	
	
    int sockedfd, connfd, len_cli; 
	struct sockaddr_in servaddr, cli; 
	pid_t childpid;

    // Create and verify Socket 
	sockedfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockedfd == -1) { 
		printf("Socket creation fail...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 

    // Clean memory and set socket parameters
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

    // Binding socket 
	if ((bind(sockedfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("Socket fail to bind...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binding...\n"); 

    // Starting listening
	if ((listen(sockedfd, LISTENMAX)) != 0) { 
		printf("Fail to listen...\n"); 
		exit(0); 
	} 
	else
		printf("Server is listening..\n"); 

    for(;;) {
        len_cli = sizeof(cli); 

        // Accept the client 
        connfd = accept(sockedfd, (SA*)&cli, &len_cli); 

		if ((childpid = fork()) == 0) {
			close(sockedfd); /* close listening socket */
            router(connfd, database); /* process the request */
            exit (0);
		}
        close(connfd); 
    }

	return 0;
} 