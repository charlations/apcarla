#include "header.h"
#include <string.h>
/* gcc client.c -o client */
/* ./client */

int main(int argc, char* argv[]) {
    int sfd, cmd_send, code_received, msg_size, input;
    int port;
    char addr[NAME_MAX + 1];
    char reply[SIZE];
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
	printf("port = %i\tip = %s\n", port, DEFAULT_IP);
	//printf("pasó connect\n");
	//addr = (char*) malloc((NAME_MAX + 1) * sizeof(char) );
	//reply = (char*) malloc(SIZE * sizeof(char) );
	
	read(sfd, &code_received, sizeof(int));
	read(sfd, &msg_size, sizeof(int));
	read(sfd, &reply, msg_size * sizeof(char));
	do {
	    printf("Escribe la orden (cat = 1, ls = 2, ^C = 3):\n");
	    scanf("%i%*c", &input);
	    switch(input){
	        case 1:
	            cmd_send = GET;
	            printf("Escribe el nombre del archivo a abrir:\n");
	            scanf("%s", addr);
	            //printf(".%s.\n", addr); //delimitar que se vea dónde empieza y termina
	        break;
	        case 2:
	            cmd_send = LIST;
	            printf("Escribe el directorio de donde enlistar los archivos:\n");
	            scanf("%s", addr);
	            printf("%s\n", addr); //delimitar que se vea dónde empieza y termina
	        break;
	        case 3:
	            cmd_send = BYE;
	            strcpy(addr, "Adios");
	            //printf(".%s.\n", addr); //delimitar que se vea dónde empieza y termina
	        break;
	        default:
	            cmd_send = 0;
	            strcpy(addr, "Comando inexistente");
	            //printf(".%s.\n", addr); //delimitar que se vea dónde empieza y termina
	        break;
	    }
	    
	    //printf("..%s..\n", addr); //delimitar que se vea dónde empieza y termina
	    msg_size = strlen(addr);
        write(sfd, &cmd_send, sizeof(int));
        write(sfd, &msg_size, sizeof(int));
        write(sfd, &addr, msg_size * sizeof(char));
        printf("Ciente: Envió el mensaje\n");
        printf("envio msg_size: %i\n", msg_size);
        //Recibe respuesta:
    	read(sfd, &code_received, sizeof(int));
    	printf("Cliente - code_received: %i\n", code_received);
        read(sfd, &msg_size, sizeof(int));
        printf("Cliente - msg_size: %i\n", msg_size);
    	read(sfd, &reply, msg_size * sizeof(char));
    	printf("Cliente: Recibió el mensaje\n");
    	printf("Cliente - read: %s\n", reply);
    	printf("%s\n", reply);
	} while(cmd_send != BYE);
    
    return 0;
}