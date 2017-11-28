#include "header.h"
int tRegreso;

/* gcc consumer.c -o cliente */
/* ./cliente tRegreso */

void a_consumer(int tRegreso, char* program) {
    int semid;
    key_t key;
    
    if( (key = ftok("/dev/null", 125)) == (key_t) -1 ) {
        perror(program);
        exit(-1);
    }
    if( (semid = semget(key, 5, 0666)) < 0 ){
        perror(program);
        exit(-1);
    }
    
    while(1) {
        if( semctl(semid, QSPACES, GETVAL, 0) > 0 ) {
            //Hay lugares disponibles
            printf("\tCliente %i entró a la barbería\n", getpid());
            //Revisa si está vacío
            if( semctl(semid, FULL, GETVAL, 0) == 0 ) {
                //Está vacío
                sem_signal(semid, FULL, 1);
                printf("\tCliente %i desperó al barbero\n", getpid());
                //es atendido
                sem_wait(semid, MUTEX, 1);
            } else {
                //Se forma en la fila
                sem_wait(semid, QSPACES, 1);
                sem_signal(semid, QPEOPLE, 1);
                //espera ser atendido
                sem_wait(semid, MUTEX, 1);
                sem_signal(semid, QSPACES, 1);
                sem_wait(semid, QPEOPLE, 1);
            }
            printf("\tCliente %i fue atendido.\n", getpid());
            if( semctl(semid, QPEOPLE, GETVAL, 0) == 0 ) {
                sem_signal(semid, EMPTY, 1);
                sem_wait(semid, FULL, 1);
            }
            sem_signal(semid, MUTEX, 1);
        } else {
            printf("\tCliente %i no pudo entrar a la barbería.\n", getpid());
        }
        sleep(tRegreso);
        
    }
    
    exit(0);
}

int main(int argc, char* argv[]) {
    int tRegreso, pid, i;
    int n = 6;
    
    if(argc != 2) {
        fprintf(stderr, "usage: %s tRegreso\n", argv[0]);
        return -1;
    }
    tRegreso = atoi(argv[1]);
    if(tRegreso < 1) {
        fprintf(stderr, "%s: tRegreso debe ser un número entero mayor a 0\n", argv[0]);
        return -1;
    }
    
    for(i = 0; i < n; i++) {
        if( (pid = fork()) < 0 ){
            perror(argv[0]);
            return -1;
        } else if(pid == 0){
            a_consumer(tRegreso, argv[0]);
        } else {
            //do nothing
        }
    }
    
    return 0;
}