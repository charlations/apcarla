#include "header.h"

/* gcc barberia.c -o barberia */
/* ./barberia cantSillas */

int main(int argc, char* argv[]){
    int semid, sillas;
    key_t key;
    
    if(argc != 2) {
        fprintf(stderr, "usage: %s cantSillas\n", argv[0]);
        return -1;
    }
    sillas = atoi(argv[1]);
    if(sillas <= 0) {
        fprintf(stderr, "%s: cantSillas debe ser un número entero mayor a 0\n", argv[0]);
        return -1;
    }
    
    if( (key = ftok("/dev/null", 125)) == (key_t) -1 ) {
        perror(argv[0]);
        return -1;
    }
    if( (semid = semget(key, 5, 0666 | IPC_CREAT)) < 0 ){
        perror(argv[0]);
        return -1;
    }
    
    semctl(semid, MUTEX, SETVAL, 1);
    semctl(semid, QSPACES, SETVAL, sillas);
    semctl(semid, QPEOPLE, SETVAL, 0);
    semctl(semid, EMPTY, SETVAL, 1);
    semctl(semid, FULL, SETVAL, 0);
    
    return 0;
}