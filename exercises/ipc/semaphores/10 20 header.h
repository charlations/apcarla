#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h> /* mecanismos básicos de comunicación */
#include <sys/sem.h> /* semáforos */

/* Creación de etiquetas para hacerlo más fácil de leer */
#define PRODUCERS   0
#define CONSUMERS   1
#define ITEMS       2
#define SPACES      3
/* esto igual asegura que solo haya uno de cada tipo de semáforo */

/* decrece */                       /* asegura que el valor sea positivo */
int sem_wait(int semid, int semnum, unsigned int val){
    /* es una estructura sembuf */
    struct sembuf op;
    
    /* campos default de una struc sembuf */
    op.sem_num = semnum; //número del semáforo a usar (PRODUCERS, CONSUMERS, ITEMS O SPACES)
    op.sem_op = -val;    //valor con qué modificar (si es positivo, usualmente es un signal, si es neg, wait)
    op.sem_flg = 0;      /*bandera que se le puede asignar, como un nowait para decir que si no consigue los
                            recursos que necesita, en vez de esperar a que se liberen, salga un error diciendo
                            que no consiguió los recursos */
    return semop(semid, &op, 1);
    /* regresa la localidad en donde está el conjunto de operaciones que quiero realizar.
     el 1 es para decir que es una sola operación la que quiero realizar */
}
/* Se puede igual hacer una operación a múltiples semáforos. En ese caso en vez de un semid, se da un arreglo
    de esctucturas y en lugar de &op se da el nombre del arreglo de estructuras y en vez de 1, se da el número
    de operaciones a hacer*/

/* incrementa */
int sem_signal(int semid, int semnum, unsigned int val){
    struct sembuf op;
    
    op.sem_num = semnum;
    op.sem_op = val;
    op.sem_flg = 0;
    return semop(semid, &op, 1);
}

#endif