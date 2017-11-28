#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//IPC en general
#include <sys/ipc.h>
//semáforos 
#include <sys/sem.h>
//a ver si así funciona semctl
#include <sys/types.h>

#define MUTEX 0
#define QSPACES 1
#define QPEOPLE 2
#define EMPTY 3
#define FULL 4

int sem_wait(int semid, int semnum, unsigned int val) {
	struct sembuf op;
	
	op.sem_num = semnum;
	op.sem_op = -val;
	op.sem_flg = 0; 
	return semop(semid, &op, 1);
}

int sem_signal(int semid, int semnum, unsigned int val) {
	struct sembuf op;
	
	op.sem_num = semnum;
	op.sem_op = val;
	op.sem_flg = 0; 
	return semop(semid, &op, 1);
}

#endif