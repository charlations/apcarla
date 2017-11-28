#include "header.h"
int slices;
int servings;

/* gcc consumers.c -o students */
/* ./students slices */

void a_consumer(char* program){
    int semid;
    key_t key;
    
    if ( (key = ftok("/dev/null", 125)) == (key_t) -1 ){
        perror("ftok");
        exit(-1);
    }
    if ( (semid = semget(key, 3, 0666)) < 0 ){
        /* ojo que aquí no se pone | IPC_CREAT porque consmidores y productores asumen que
            éstos ya fueron creados */
        perror(program);
        exit(-1);
    }
    
    while(1) {
        sem_wait(semid, MUTEX, 1);
        if(slices == 0) {
            printf("No more pizza, buying...\n");
            sem_signal(semid, EMPTY, 1);
            sem_wait(semid, FULL, 1);
            slices = servings;
        }
        printf("Student %i eating pizza\n", getpid());
        slices--;
        sem_signal(semid, MUTEX, 1);
        sleep(2);
    }
    exit(0);
}

int main(int argc, char* argv[]){
    int pid, i;
    
    if(argc != 2){
        fprintf(stderr, "usage: %s slices\n", argv[0]);
        return -1;
    }
    slices = atoi(argv[1]);
    if(slices < 1){
        fprintf(stderr, "%s: The slices must be a positive number greater than zero.\n", argv[0]);
        return -1;
    }
    servings = slices;
    
    a_consumer(argv[0]);
    /*            //2 estudiantes
    for(i = 0; i < 2; i++){
        if( (pid = fork()) < 0 ){
            perror(argv[0]);
            return -1;
        } else if(pid == 0){
            a_consumer(argv[0]);
        } else {
            //do nothing
        }
    }*/
    
    return 0;
}