#include "header.h"
/* gcc client.c -o client */
/* ./client */

int main(int argc, char* argv[]) {
    int sfd, cmd_send, code_recieved, msg_size, input;
    int port;
    char* addr;
    char* reply;
    struct sockaddr_in server_info;
    
	port = DEFAULT_PORT;
	
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket\n");
		return -1;
	}
	//printf("pasó socket\n");
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(DEFAULT_IP);
	server_info.sin_port = htons(port);
	if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror("connect\n");
		return -1;
	}
	//printf("pasó connect\n");
	addr = (char*) malloc((NAME_MAX + 1) * sizeof(char) );
	reply = (char*) malloc(SIZE * sizeof(char) );
	
	read(sfd, &msg_size, sizeof(int));
	read(sfd, &code_recieved, sizeof(int));
	read(sfd, &reply, msg_size * sizeof(char));
	do {
	    printf("Escribe la orden (cat = 1, ls = 2, ^C = 3):\n");
	    scanf("%i", &input);
	    switch(input){
	        case 1:
	            cmd_send = GET;
	            printf("Escribe el nombre del archivo a abrir:\n");
	            scanf("%s", addr);
	        break;
	        case 2:
	            cmd_send = LIST;
	            printf("Escribe el directorio de donde enlistar los archivos:\n");
	            scanf("%s", addr);
	        break;
	        case 3:
	            cmd_send = BYE;
	            addr = "Adios";
	        break;
	        default:
	            cmd_send = 0;
	            addr = "Comando inexistente";
	        break;
	    }
	    msg_size = strlen(addr);
        write(sfd, &msg_size, sizeof(msg_size));
        write(sfd, &cmd_send, sizeof(cmd_send));
        write(sfd, &addr, sizeof(addr));
        //Recibe respuesta:
        read(sfd, &msg_size, sizeof(int));
    	read(sfd, &code_recieved, sizeof(int));
    	read(sfd, &reply, msg_size * sizeof(char));
    	printf("%s\n", reply);
	} while(cmd_send != BYE);
    
    return 0;
}