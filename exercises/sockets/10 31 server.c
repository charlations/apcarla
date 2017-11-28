#include "10 31 header.h"
#include <string.h>

/* gcc 10\ 31\ header.h 10\ 31\ server.c -o server */
/* ./server */

void serves_client(int nsfd) {
    int number_sent, number_guess;
    int guess, answer;
    
    srand(getpid()); 
    //srand(time(0));
    do {
        guess = 0;
        number_guess = (rand() % 100) + 1;
        printf("PID = %i <-> number to guess = %i\n", getpid(), number_guess);
        while (!guess) {
            /* Todos los read son bloqueantes!!! */
            read(nsfd, &number_sent, sizeof(number_sent));
            if (number_sent < number_guess) {
                answer = SMALLER;
            } else if (number_sent > number_guess) {
                answer = BIGGER;
            } else {
                answer = EQUAL;
                guess = 1;
            }
            write(nsfd, &answer, sizeof(answer));
        }
        read(nsfd, &answer, sizeof(answer));
    } while (answer == CONTINUE);
    close(nsfd);
}

void server(char* ip, int port, char* program) {
    int sfd, nsfd, pid;
    /* internet cualquiera del protocolo TCP/IP (por ser '_in', si no, podría 
        ser de cualquier protocolo) */
    struct sockaddr_in server_info, client_info;
    
                      /* AF_UNIX es una estructura virtual, no hay una arquitectura
                        de conexión como tal, mientras que AF_INET sí tiene una 
                        arquitectura de conexion (si se conecta a la red) */
                                /* SOCK_STREAM es el protocolo de comunicación de
                                    acknowledge, espera confirmación, hay turnos.
                                    Quiero algo / No quiero nada VS Si te voy a
                                    dar el servicio / No te voy a dar nada. 
                                    SOCK_DGRAM es el protocolo que no ocupa 
                                    confirmación (obvio solo el handshake) */
    if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror(program);
        exit(-1);
    }
    
    server_info.sin_family = AF_INET; /* internet tal cual que siempre conocemos */
    server_info.sin_addr.s_addr = inet_addr(ip); /* ahorita es un entero de 32 bits porque es ipv4 
                                                    como un IP puede ser de caracteres ('0.f.a.3'),
                                                    una dirección web ('murillo.qro.itesm.mx'), o
                                                    cosas por el estilo, 'inet_addr' lo convierte
                                                    a los 32 bits en número.
                                                    */
    server_info.sin_port = htons(port);         /* Big Endian e Little Endian. Si tienen los bits de
                                                    un entero se pueden leer de izq a der o de der a 
                                                    izq. Little Endian le de derecha a izquierda
                                                    Son trucos para hacer a su mínima expresión 
                                                    cada función que quieran hacer (hacer que corra 
                                                    más rápido.) 'htons' [Hardware TO Network System]
                                                    quita cualquier conflicto convirtiendo las 
                                                    traducciones a una forma neutra de UNIX. La máquina
                                                    ya sabe cómo interpretar los datos.
                                                    */
        /* liga un socket file descriptor al sockaddr (dirección ip), pero de forma genérica, entonces
            se le hace un cast de 'sockaddr_in' a un 'sockaddr' */
    if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
        perror(program);
        exit(-1);
    }
    
    /* activar lo que acabamos de ligar */
    listen(sfd, 1);
    while (1) {
        int len = sizeof(client_info);
                    /* si no hay qué aceptar, se queda esperando hasta que le llegue una petición */
        if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ){
            perror(program);
            exit(-1);
        }
        /* En una estructura de servicio, el socket de escucha SOLO puede escuchar. Al escuchar una
            petición, el socket de escuchar CREA UN NUEVO SOCKET de comunicación exclusivo con ese
            cliente (nsfd)  */
        /** CONCURRENTE - puede atender más de un cliente a la vez
         * if ( (pid = fork()) < 0 ){
         *      perror(program);
         *  } else if (pid == 0) {
         *     close(sfd) //el proceso hijo no requiere escuchar, por eso cierra el canal de escucha
         *     serves_client(nsfd);
         *     exit(0);
         * } else {
         *     close(nsfd);
         * }
         *  CONCURRENTE
        **/ 
        serves_client(nsfd); /* ITERATIVO */
    }
}

/* copiado de server.c */
int main(int argc, char* argv[]) {
	char ip[15];	
	int port;
	
	strcpy(ip, DEFAULT_IP);
	port = DEFAULT_PORT;
	if (argc == 3) {
		if (strcmp(argv[1], "-d") == 0) {
			strcpy(ip, argv[2]);
		} else if (strcmp(argv[1], "-p") == 0) {
			port = atoi(argv[2]);
			if (port < 5000) {
        		printf("%s: The port must be greater than 5000.\n", argv[0]);
        		return -1;
        	}
		} else {
			printf("usage: %s [-d dir] [-p port]\n", argv[0]);
			return -1;
		}
	} else if (argc == 5) {
		if (strcmp(argv[1], "-d") == 0) {
			strcpy(ip, argv[2]);
			if (strcmp(argv[3], "-p") == 0) {
				port = atoi(argv[4]);
				if (port < 5000) {
            		printf("%s: The port must be greater than 5000.\n", argv[0]);
            		return -1;
            	}		
			} else {
				printf("usage: %s [-d dir] [-p port]\n", argv[0]);
				return -1;
			}			
		} else if (strcmp(argv[1], "-p") == 0) {
			port = atoi(argv[2]);
			if (port < 5000) {
        		printf("%s: The port must be greater than 5000.\n", argv[0]);
        		return -1;
        	}		
			if (strcmp(argv[3], "-d") == 0) {
				strcpy(ip, argv[4]);
			} else {
				printf("usage: %s [-d dir] [-p port]\n", argv[0]);
				return -1;
			}
		} else {
			printf("usage: %s [-d dir] [-p port]\n", argv[0]);
			return -1;
		}
	} else if (argc != 1) {
		printf("usage: %s [-d dir] [-p port]\n", argv[0]);
		return -1;
	}
	
	server(ip, port, argv[0]);
	
	return 0;
}	