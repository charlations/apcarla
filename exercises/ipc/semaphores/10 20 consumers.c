#include "10 20 header.h"

void a_consumer(char* program){
    int i, k, semid;
    key_t key;
    
    if ( (key = ftok("/dev/null", 125)) == (key_t) -1 ){
        perror("ftok");
        exit(-1);
    }
    if ( (semid = semget(key, 4, 0666)) < 0 ){
        /* ojo que aquí no se pone | IPC_CREAT porque consmidores y productores asumen que
            éstos ya fueron creados */
        perror(program);
        exit(-1);
    }
    
    srand( (getpid()) );
    for(k = 0; k < 10; k++){
        printf("Consumer %i trying to acces the buffer.\n", getpid());
        sem_wait(semid, CONSUMERS, 1);
        printf("Consumer %i accesing the buffer.\n", getpid());
        
        i = (rand() % 5) + 1;
        printf("Consumer %i trying to get %i product(s) - items = %i\n", getpid(), i, semctl(semid, ITEMS, GETVAL, 0));
        /* está usando semctl para CONSULTAR el valor del semáforo ITEMS. Pueden haber múltiples
            consultas siempre y cuando haya una sola lectura y éstas no interfieran */
        sem_wait(semid, ITEMS, i);
        /* si pido 5 y solo hay 3, me va a dar esos primeros 3 y luego va a esperar a que se liberen esos otros 2 */
        printf("Consumer %i took their products.\n", getpid());
        /* avisarle a SPACES que ahora hay _tantos_ espacios vacíos */
        sem_signal(semid, SPACES, i);
        printf("Consumer %i is going to sleep.\n", getpid());
        sleep(rand() % 10 + 1);
    }
    exit(0);
}

int main(int argc, char* argv[]){
    int amount, pid, i;
    
    if(argc != 2){
        fprintf(stderr, "usage: %s amount\n", argv[0]);
        return -1;
    }
    amount = atoi(argv[1]);
    if(amount < 1){
        fprintf(stderr, "%s: The amount must be a positive number greater than zero.\n", argv[0]);
        return -1;
    }
    for(i = 0; i < amount; i++){
        if( (pid = fork()) < 0 ){
            perror(argv[0]);
            return -1;
        } else if(pid == 0){
            a_consumer(argv[0]);
        } else {
            //do nothing
        }
    }
    
    return 0;
}