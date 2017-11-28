#include "11 03 header.h"

#define SIZE 4

int tablero[4][4];
int intentos[4][4];
int i, j;

void initIntentos(){
    for (i = 0; i < SIZE; i++){
        intentos[i][0] = 0;
        intentos[i][1] = 0;
        intentos[i][2] = 0;
        intentos[i][3] = 0;
    }
}

void imprimeIntentos(){
    printf("Tu tablero:\t\tTus intentos\n");
    for(i = 0; i < SIZE; i++){
        printf("\t");
        for(j = 0; j < SIZE; j++){
            if(tablero[i][j]){
                printf("x|");
            } else {
                printf(" |");
            }
        }
        printf("\t\t");
        for(j = 0; j < SIZE; j++){
            if(intentos[i][j] == 1){
                printf("x|");
            } else if(intentos[i][j] == 2){
                printf("o|");
            }else {
                printf(" |");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void setTable(){
    int x, y;
    srand(getpid());
    //asegurar inicializar en 0
    for (i = 0; i < SIZE; i++){
        tablero[i][0] = 0;
        tablero[i][1] = 0;
        tablero[i][2] = 0;
        tablero[i][3] = 0;
    }
    
    for (i = 0; i < SIZE; i++){
        x = (rand() % 4);
        y = (rand() % 4);
        
        while(tablero[x][y]){
            x = (rand() % 4);
            y = (rand() % 4);
        }
        tablero[x][y] = 1;
    }
}

void printTable(){
    printf("Tu tablero:\n");
    for(i = 0; i < SIZE; i++){
        printf("\t");
        for(j = 0; j < SIZE; j++){
            if(tablero[i][j]){
                printf("x|");
            } else {
                printf(" |");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

int main(int argc, char* argv[]) {
    int sfd, port, columna, reply;
    struct sockaddr_in server_info;
    char retro[15];
    int fila = 0;
    int golpes = 0;
    
    if (argc != 3) {
	    printf("usage: %s ip port\n", argv[0]);
	    return -1;
	}
	
	port = atoi(argv[2]);
	if (port < 5000) {
		printf("%s: The port must be greater than 5000.\n", argv[0]);
		return -1;
	}
	
	if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket");
		return -1;
	}
	
	setTable();
	printTable();
	initIntentos();
	
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(argv[1]);
	server_info.sin_port = htons(port);
	if ( connect(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
		perror("connect");
		return -1;
	}
	
	write(sfd, &fila, sizeof(fila));
	read(sfd, &retro, sizeof(retro));
	printf("%s\n", retro);
	/*write(sfd, &fila, sizeof(fila));
	read(sfd, &retro, sizeof(retro));
	printf("%s\n", retro);*/
	do {
	    read(sfd, &reply, sizeof(reply));
	    if(reply == CONTINUE){
	        printf("Ingrese la fila (0 - 3):\n");
	        scanf("%i", &fila);
	        while(fila < 0 || fila > 4){
	            printf("La fila solo puede ser de 0 - 3\nIngrese la fila (0 - 3):\n");
	            scanf("%i", &fila);
	        }
	        write(sfd, &fila, sizeof(fila));
	        printf("Ingrese la columna (0 - 3):\n");
	        scanf("%i", &columna);
	        while(columna < 0 || columna > 3){
	            printf("La columna solo puede ser de 0 - 3\nIngrese la columna (0 - 3):\n");
	            scanf("%i", &columna);
	        }
	        write(sfd, &columna, sizeof(columna));
	        //resultado
	        read(sfd, &reply, sizeof(reply));
	        switch(reply){
	            case HIT:
	                printf("Hit!\n");
	                intentos[fila][columna] = 1;
	                imprimeIntentos();
	            break;
	            case MISSED:
	                printf("Missed :(\n");
	                intentos[fila][columna] = 2;
	                imprimeIntentos();
	            break;
	            case END:
	                printf("Felicidades! Ganaste!\n");
	                intentos[fila][columna] = 1;
	                imprimeIntentos();
	            break;
	            default:
	                printf("error :V\n");
	            break;
	        }
	    } else if (reply == ANSWER) {
	        read(sfd, &fila, sizeof(fila));
	        read(sfd, &columna, sizeof(columna));
	        if(tablero[fila][columna]){
	            printf("Te golpearon\n");
	            tablero[fila][columna] = 0;
	            printTable();
	            fila = HIT;
	            if(++golpes == SIZE){
	                printf("Perdiste :(\n\n");
	                fila = END;
	                reply = END;
	            }
	        } else {
	            printf("Salvada! (ﾉ◕ヮ◕)ﾉ*:・ﾟ✧\n");
	            fila = MISSED;
	        }
	        write(sfd, &fila, sizeof(fila));
	    } else {
	        //do nothing
	    }
	} while( reply != END );
	//} while (1);
	close(sfd);
	
	return 0;
}