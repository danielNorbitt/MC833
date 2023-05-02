#include "client.h"

int main(){

    while(1){
            printf("Lista de Comandos\n");
            printf("\t1 - Cadastrar um novo perfil\n");
            printf("\t2 - Listar todos perfis com determinado curso\n");
            printf("\t3 - Listar todos perfis que tenham determinada habilidade\n");
            printf("\t4 - Listar todos perfis com determinado ano\n");
            printf("\t5 - Listar todos os perfis\n");
            printf("\t6 - Mostrar um perfil\n");
            printf("\t7 - Remover um perfil\n");
            printf("\t8 - Sair do programa\n");

            printf("\nComando: ");
            char command = getchar();

            //Clear input
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 

            // Get the parameter
            char parameter[MAX_PROFILE_INFO] = {'\0'};
            switch (command){
                case '1':
                    get_profile_info(parameter);
                    break;
                case '2':
                    printf("Curso que deseja buscar: ");
                    get_parameter(parameter);
                    break;
                case '3':
                    printf("Habilidade que deseja buscar: ");
                    get_parameter(parameter);
                    break;
                case '4':
                    printf("Ano que deseja buscar: ");
                    get_parameter(parameter);
                    break;
                case '5':
                    break;
                case '6':
                    printf("Email para a consulta: ");
                    get_parameter(parameter);
                    break;
                case '7':
                    printf("Email para remover: ");
                    get_parameter(parameter);
                    break;
                case '8':
                    printf("Tchau.\n");
                    return 0;
                    break;
                default:
                    printf("Comando inválido.\n");
                    continue;
            }
            send_request(command, parameter, IP, PORT);
        }

    return 0;
}

void get_parameter(char *parameter) {
    fgets(parameter, MAX_PROFILE_INFO, stdin);
    parameter[strcspn(parameter, "\n")] = '\0';
}

void get_profile_info(char* args) {
    char temp[BUFFER_LEN];
    printf("Email: ");
    fgets(temp, MAX_PROFILE_INFO, stdin);
    strcat(args,temp);
    printf("First Name: ");
    fgets(temp, MAX_PROFILE_INFO, stdin);
    strcat(args,temp);
    printf("Last Name: ");
    fgets(temp, MAX_PROFILE_INFO, stdin);
    strcat(args,temp);
    printf("City: ");
    fgets(temp, MAX_PROFILE_INFO, stdin);
    strcat(args,temp);
    printf("Course: ");
    fgets(temp, MAX_PROFILE_INFO, stdin);
    strcat(args,temp);
    printf("Graduation Year: ");
    fgets(temp, MAX_PROFILE_INFO, stdin);
    strcat(args,temp);
    printf("Skills (Use comma to separate): ");
    fgets(temp, MAX_PROFILE_INFO, stdin);
    strcat(args,temp);
}

// Receive the server response back 
void receive_message(int socket) {
    char buffer[BUFFER_LEN];

    bzero(buffer, BUFFER_LEN); 

    printf("Server Response:\n");

    if(read(socket, buffer, BUFFER_LEN) < 0) 
        return;

    for(int i = 0; (buffer[i] != 0x04 && i < BUFFER_LEN) ; i++) {
        printf("%c", buffer[i]);
    }
    return;
}

// Connect to server and send the message
void send_request(char cmd, char* arg, char* ip, int port){
	char msg[BUFFER_LEN] = {'\0'};

	msg[0] = cmd;
	memcpy(&msg[1], arg, BUFFER_LEN - 1);

    int sock_fd = 0; 
	struct sockaddr_in serv_addr;
	
	if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket creation error");
	}else {
        printf("Socket creation sucessefuly\n");
    }

    bzero(&serv_addr, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;      
    serv_addr.sin_port = htons (port);
    
	if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
		printf("Error to connect\n");
		return;
	}

	if (connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Connection Failed \n");
		return;
	}

    printf("Message that client will send: %s\n",msg);
    
	send(sock_fd, msg, BUFFER_LEN, 0);

	receive_message(sock_fd);

}