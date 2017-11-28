#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> /* para el atoi*/
#include <limits.h> /* para NAME_MAX */
#include <sys/wait.h> /* para el wait */
#include <signal.h>
#include <fcntl.h> /* file control: para el random */

#define SIZE 1000

/************************************ CHILD0 ************************************/
/** SOLO PARA FUTURAS REFERENCIAS:
 * se puede poner código vacío para asegurar que esté corriendo bien el programa.
 * Un ejemplo de éste es el siguiente (se pone en ámbos hijos):
 *
 * | void handler_child0(int sig) {
 * |     printf("PID = %i - SIGNAL %i RECIEVED\n", getpid(), sig);
 * | }
 * | void child0(char* prefix, char* program){
 * |     printf("PID = %i\tPPID = %i\n", getpid(), getppid());
 * |     exit(0);
 * | }
 *
**/
void handler_child0(int sig) {
    printf("\tPID = %i - SIGNAL %i RECIEVED\n", getpid(), sig);
}

/* copiado de signals/make_data.c */
void write_numbers(char* prefix, int iter, char* program) { 
    int fd, i, num, *data;
    char filename[NAME_MAX + 1];
	
	srand( 12345 );
	data = (int*) malloc(SIZE * sizeof(int));
	for (i = 0; i < SIZE; i++) {
		data[i] = ( rand()%100 ) + 1;
	}
	
	sprintf(filename, "%s0%i.dat", prefix, iter);
	if ( (fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror(program);
		return; //se puede dejarlo así :V
	}
	write(fd, data, SIZE * sizeof(int));
	close(fd);
	
	
	free(data);
}

void child0(char* prefix, char* program){
    int i;
    signal(SIGUSR1, handler_child0);
    i = 0;
    /*los hijos solo se destruyen cuando acaba TODO el programa... entonces
      cada proceso necesita un contador */
    while (i < 3){
        pause();
        write_numbers(prefix, i, program);
        kill(getppid(), SIGUSR1);
        i++;
    }
    
    exit(0);
}

/************************************ CHILD1 ************************************/
void handler_child1(int sig) {
    printf("\t\tPID = %i - SIGNAL %i RECIEVED\n", getpid(), sig);
}
void child1(char* in, char* out, char* program){
    int i;
    signal(SIGUSR2, handler_child1);
    i = 0;
    /*los hijos solo se destruyen cuando acaba TODO el programa... entonces
      cada proceso necesita un contador */
    while (i < 3){
        pause();
        printf("\t\tPID = %i - DOING MY TASK\n", getpid());
        kill(getppid(), SIGUSR2);
        i++;
    }
    
    exit(0);
}

/************************************ PARENT ************************************/
void handler(int sig) {
    printf("PID = %i - SIGNAL %i RECIEVED\n", getpid(), sig);
}

int main(int argc, char* argv[]){
    int timeout, pids[2], i;
    char *inprefix, *outprefix;
    
    if(argc != 4){
        fprintf(stderr, "Usage: %s timeout in_prefix out_prefix\n", argv[0]);
        return -1;
    }
    timeout = atoi(argv[1]);
    if(timeout < 1){
        fprintf(stderr, "%s: The timeout mush be a positive integer\n", argv[0]);
    }
    
    inprefix = argv[2];
    outprefix = argv[3];
    
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    
    for (i = 0; i < 2; i++){
        //[empieza nota]
        if ( (pids[i] = fork()) < 0 ) {
            perror(argv[0]);
            exit(-1);
        } else if (pids[i] == 0) {
            switch(i) {
                case 0: child0(inprefix, argv[0]); break;
                case 1: child1(inprefix, outprefix, argv[0]); break;
            }
        } else {
            //do nothing
        }
    }
    
    sleep(1); /* darle tiempo a los hijos instalen su manejador para estar
     listos para recibir señales*/
    int cont = 0;
    while(cont < 3) {
        kill(pids[0], SIGUSR1);
        printf("PID = %i waiting...\n", getpid());
        pause(); /* espera CUALQUIER señal */
        kill(pids[1], SIGUSR2);
        printf("PID = %i waiting...\n", getpid());
        pause();
        cont++;
    }
    
    /*para asegurar que salga del proceso padre */
    
    while(i > 0) {
        wait(NULL);
        i--;
    }
    
    return 0;
}

/*
    [nota]
    //no se hizo:
    
    if( (pids[i] = fork()) < 0 ) {
        perror(argv[0]);
        exit(-1);
    } else if (pids[i] == 0) {
        process_child(inprefix, argv[0]);
    } else {
        //do nothing
    }
    if( (pids[i] = fork()) < 0 ) {
        perror(argv[0]);
        exit(-1);
    } else if (pids[i] == 0) {
        process_child(inprefix, argv[0]);
    } else {
        //do nothing
    }
    
    //porque entonces, si el child no se le pone exit, luego el hijo va a salir 
    //del primer if y crear un hijo. Para evitar ésto se puede hacer igual:
    
    if( (pids[i] = fork()) < 0 ) {
        perror(argv[0]);
        exit(-1);
    } else if (pids[i] == 0) {
        process_child(inprefix, argv[0]);
    } else {
        if( (pids[i] = fork()) < 0 ) {
            perror(argv[0]);
            exit(-1);
        } else if (pids[i] == 0) {
            process_child(inprefix, argv[0]);
        } else {
            //do nothing
        }
    }
    
    //pero Peter no quiso porque se ve feo... :P
*/