#include "10 24 header.h"

/* gcc 10\ 24\ buffer.c -o buffer */

int main(int argc, char* argv[]){
    int semid, shmid, i;
    unsigned short final_values[3];
    key_t key; /* <sys/ipc.h> */
    
    if (argc != 1) {
        fprintf(stderr, "usage: %s\n", argv[0]);
        return -1;
    }
    
    if( (key = ftok("/dev/null", 65)) == (key_t) -1 ) { //un -1 de tipo key_t declara error
        perror(argv[0]);
        return -1;
    }
    
    /* primero se verifica que se puedan inicializat los semáforos correctamente, y si se
       puede, hasta entonces se inicializa la shared memory (shmget y shmctl) */
    if( (semid = semget(key, 3, 0666 | IPC_CREAT)) < 0 ) {
        perror(argv[0]);
        return -1;
    }
    
    semctl(semid, MUTEX, SETVAL, 1);    /* solo puede entrar 1 a la vez */
    semctl(semid, EMPTY, SETVAL, SIZE);  /* poner el tamaño del arreglo para decir que hay SIZE
                                          lugares vacíos */
    semctl(semid, FULL, SETVAL, 0);     /* poner 0 para decir que no hay lugares llenos */
    
    semctl(semid, 0, GETALL, final_values);
    for (i = 0; i < 3; i++) {
        printf("%3i", final_values[i]);
    }
    printf("\n");
    
    if( (shmid = shmget(key, sizeof(Buffer), 0666 | IPC_CREAT)) < 0 ) {
        /* esta memoria NO PERTENECE A NINGUN PROCESO, sino al SYSTEMA OPERATIVO. 
            si no la puede crear, no se puede seguir trabajando, por lo que se
            necesitan borrar los semáforos */
        semctl(semid, 0, IPC_RMID, 0);
        perror("shm");
        return -1;
    }
    
    return 0;
}