#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h> /* shared memory */

#define SIZE 10     /* de la memoria */
#define MUTEX 0     /* semáforo 0 */
#define EMPTY 1     /* semáforo para detener a los consumidores cuando no hay recursos */
#define FULL 2      /* semáforo para detener a los productores cuando no hay recursos consumidos */

typedef struct buffer {
    int data[SIZE];
    int next;
} Buffer; /* la estructura se llama "struct buffer", estoy declarando un alias "Buffer" para
             no tener que declararlo como "struct buffer" cada vez que lo uso */
/* Ejemplos de usos de alias:
 * typedef unsigned char uchar;
 * typedef long long int lli;
*/

/* copiado de 10 20 header.h */
/* decrece */                       /* asegura que el valor sea positivo */
int sem_wait(int semid, int semnum, unsigned int val){
    /* es una estructura sembuf */
    struct sembuf op;
    
    /* campos default de una struc sembuf */
    op.sem_num = semnum;
    op.sem_op = -val;
    op.sem_flg = 0;
    return semop(semid, &op, 1);
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