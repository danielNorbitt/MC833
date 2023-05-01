#include "client.h"

int main(){

    while(1){
            printf("Lista de Comandos\n");
            printf("\t1 - Cadastrar um novo perfil\n");
            printf("\t2 - Listar todas as pessoas (email e nome) formadas em um determinado curso\n");
            printf("\t3 - Listar todas as pessoas (email e nome) que possuam uma determinada habilidade\n");
            printf("\t4 - Listar todas as pessoas (email, nome e curso) formadas em um determinado ano\n");
            printf("\t5 - Listar todas as informações de todos os perfis\n");
            printf("\t6 - Retornar informações de um perfil\n");
            printf("\t7 - Remover um perfil\n");
            printf("\t8 - Sair do programa\n");

            printf("\nComando: ");
            char command = getchar();

            //Clear input buffer to prevent overflow
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 

            /* Directs to the right function if it needs to take an extra argument */
            char arg[BUFFER_LEN - 1] = {'\0'};
            switch (command){
                case '1':
                    printf("Passe todas informações do perfil:");
                    // take_argument(arg);
                    break;
                case '2':
                    printf("Curso que deseja buscar: ");
                    take_argument(arg);
                    break;
                case '3':
                    printf("Habilidade que deseja buscar: ");
                    take_argument(arg);
                    break;
                case '4':
                    printf("Ano que deseja buscar: ");
                    break;
                case '5':
                    // take_argument(arg);
                    break;
                case '6':
                    printf("Email do perfil que deseja consultar: ");
                    break;
                case '7':
                    printf("Email do perfil que deseja remover: ");
                    take_argument(arg);
                    break;
                case '8':
                    printf("Programa terminado.\n");
                    return 0;
                    break;
                default:
                    printf("Comando inválido, tente novamente.\n\n");
                    continue;
            }

            send_request(command, arg, IP, PORT);

            printf("Pressione enter para continuar.");
            while ((c = getchar()) != '\n' && c != EOF);
        }

    return 0;
}

void take_argument(char *arg) {
    printf("aaaa\n");
    fgets(arg, MAX_PROFILE_INFO - 1, stdin);
    int len = strlen(arg);
	if(len <= 1) {
		return;
	}
	arg[len - 1] = '\0';
}

/*Function that connects to the server via TCP connection, it needs the ip and port number*/
int connect_server(char *ip, int port){
	int sock_fd = 0; //socket file descriptor
	struct sockaddr_in serv_addr; //socket address structure
	
	//PF_INET: socket uses internet IPv4
    //SOCK_STREAM: TCP socket
    //third parameter 0: chooses the proper protocol for the given type
	if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket creation error\n");
	}else {
        printf("Socket creation sucessefuly\n");
    }

	/*Fills server socket address structure */
    bzero(&serv_addr, sizeof(serv_addr)); // fill with zeros first
    serv_addr.sin_family = AF_INET;      //address family AF_INET, for IPv4
    serv_addr.sin_port = htons (port);   //fill port number and convert to network byte order

	//Converts the string ip received to numeric binary form and fills the server socket address structure
	if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
		printf("Invalid address|Address not supported\n");
		return -1;
	}
	
	//If it succeeded in connecting to the server it returns the socket descriptor
	if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Connection Failed \n");
		return -1;
	}else{
		return sock_fd;
	}
}

int send_request(char cmd, char* arg, char* ip, int port){
	char msg[BUFFER_LEN] = {'\0'};
	bzero(msg, sizeof(msg));

	msg[0] = cmd;
	memcpy(&msg[1], arg, BUFFER_LEN - 1);

	int sock = 0;
	sock = connect_server(ip, port);
	if(sock == -1) {
        printf("Fail to connect to the server");
        return 1;
    }

    printf("%s\n",msg);
    
	send(sock, msg, BUFFER_LEN, 0);

	char buffer[BUFFER_LEN] = {0};

	/* Keeps receiving from recv() until detects end of message character: EOT */
	printf("Server Response:\n");
	int end_message = 0;
	while(end_message == 0){
		if(recv(sock, buffer, BUFFER_LEN, 0) < 0) {
			printf("Error in receiving data.\n");
			return 1;
		}

		for(int i = 0; i < BUFFER_LEN; i++) {
			if(buffer[i] != 0x04){
				printf("%c", buffer[i]);
			} else {
				end_message = 1; //0x04 EOT -End of Transmission special character
			}
		}

		bzero(buffer, sizeof(buffer)); //clear buffer with zeros
	}
	return 0;
}