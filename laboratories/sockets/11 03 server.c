#include "11 03 header.h"
#include <string.h>

/* gcc 11\ 03\ header.h 11\ 03\ server.c -o server */
/* ./server dir port & */
/* eje: ./server 127.0.0.1 5001 & */

void serves_client(int nsfd1, int nsfd2) {
    /* creo que el cliente debe de crear los 2 jugadores antes y ya de ahí
    vemos cómo se envía todo omg estoy perdida D: */
    int fila, columna;
    int ans = ANSWER;
    int sigue = CONTINUE;
    int turno = 0;
    int input;
    read(nsfd1, &input, sizeof(input));
    write(nsfd1, "Hola, jugador 1", 15 * sizeof(char));
    read(nsfd2, &input, sizeof(input));
    write(nsfd2, "Hola, jugador 2", 15 * sizeof(char));
    /*read(nsfd1, &input, sizeof(input));
    write(nsfd1, "Otra, jugador 1", 15 * sizeof(char));
    read(nsfd2, &input, sizeof(input));
    write(nsfd2, "Otra, jugador 2", 15 * sizeof(char));*/
    while(sigue == CONTINUE){
        if(!turno) {
            write(nsfd1, &sigue, sizeof(sigue));
            read(nsfd1, &fila, sizeof(fila));
            read(nsfd1, &columna, sizeof(columna));
            write(nsfd2, &ans, sizeof(ans));
            write(nsfd2, &fila, sizeof(fila));
            write(nsfd2, &columna, sizeof(columna));
            read(nsfd2, &input, sizeof(input));
            switch(input){
                case MISSED:
                    write(nsfd1, &input, sizeof(input));
                break;
                case HIT:
                    write(nsfd1, &input, sizeof(input));
                break;
                case END:
                    write(nsfd1, &input, sizeof(input));
                    sigue = END;
                break;
                default:
                break;
            }
            turno = !turno;
        } else {
            //copy
            write(nsfd2, &sigue, sizeof(sigue));
            read(nsfd2, &fila, sizeof(fila));
            read(nsfd2, &columna, sizeof(columna));
            write(nsfd1, &ans, sizeof(ans));
            write(nsfd1, &fila, sizeof(fila));
            write(nsfd1, &columna, sizeof(columna));
            read(nsfd1, &input, sizeof(input));
            switch(input){
                case MISSED:
                    write(nsfd2, &input, sizeof(input));
                break;
                case HIT:
                    write(nsfd2, &input, sizeof(input));
                break;
                case END:
                    write(nsfd2, &input, sizeof(input));
                    sigue = END;
                break;
                default:
                break;
            }
            turno = !turno;
        }
        
    }
    
    close(nsfd1);
    close(nsfd2);
}

void server(char* ip, int port, char* program) {
    int sfd, nsfd1, nsfd2, pid;
    struct sockaddr_in server_info, client_info;
                                    /* como no necesito un log ni nada por el 
                                        estilo, puedo solo usar una estructura 
                                        de cliente y sobreescribirla cada vez. 
                                        Si se va a usar la información para algo
                                        más, entonces sí se declaran 2 
                                        estructuras
                                        
                                        PERO NO SÉ HACER LAS 2 COMUNICACIONES 
                                        CON UN SOLO SOCKET, ASI QUE ALV
                                        
                                        ah, ptm ya entendí... hay un solo 
                                        client_info, pero 2 nsfd....... >:C
                                    */
    
    if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("sfd socket");
        exit(-1);
    }
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = inet_addr(ip);
    server_info.sin_port = htons(port);
    
    if( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
        perror("bind sfd");
        exit(-1);
    }
    
    /* TIP: clientes recibe un parámetro de cuántos clientes crea */
    listen(sfd, 1);
    while(1) {
        int len = sizeof(client_info);
        if( (nsfd1 = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ){
            perror("accept first nsfd\n");
            exit(-1);
        }
        printf("First conection accepted\n");
        if( (nsfd2 = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ){
            perror("accept second nsfd\n");
            exit(-1);
        }
        printf("Second conection accepted\n");
        //serves_client(nsdf, 2);
        serves_client(nsfd1, nsfd2);
    }
}

int main(int argc, char* argv[]) {
    char ip[15];
    int port;
    
    if (argc != 3) {
        printf("usage: %s dir port\n", argv[0]);
        return -1;
    }
    strcpy(ip, argv[1]);
    port = atoi(argv[2]);
    
    if(port < 5000) {
        printf("%s: The port must be greater than 5000.\n", argv[0]);
        return -1;
    }
    
    server(ip, port, argv[0]);
    
    return 0;
}