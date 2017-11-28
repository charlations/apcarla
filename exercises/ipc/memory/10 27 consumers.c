/* copiado de 10 24 producers.c con ciertas modificaciones */

#include "10 24 header.h"

/* gcc 10\ 27\ consumers.c -o cons */

void a_consumer(char* program) {
    int semid, shmid, i, j, k;
    Buffer *b;
    key_t key;
    
    if( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
        perror(program);
        exit(-1);
    }
    
    if( (semid = semget(key, 3, 0666)) < 0 ){ //recuperar el id del semáforo
        perror(program);
        exit(-1);
    }
    
    if( (shmid = shmget(key, sizeof(Buffer), 0666)) < 0 ) { /*recuperar el id de
                                                            la memoria compartida */
        perror(program);
        exit(-1);
    }
                /* SHared Memory ATtach 
                 * Un malloc, pero en vez de apuntar a memoria dinámca, toma una
                 * memoria compartida apuntando al bloque de memoria shmid, que
                 * tome desde la posición 0, y con 0 flags
                 */
    b = (Buffer*) shmat(shmid, (void*) 0, 0);
    
    srand( getpid() );
    for(k = 0; k < 10; k++){ //lo voy a hacer 10 veces, no tiene que ver con SIZE
        i = (rand() % 5) + 1; //cuantos items vas a colocar
        printf("Consumer %i trying to put %i items\n", getpid(), i);
        sem_wait(semid, FULL, i); /* ver si hay productos a consumir. Si no 
                                    los hay, espera */
        printf("Consumer %i trying to get the lock over the buffer.\n", getpid());
        /* necesita ser el único que está modificando la memoria. Ser el MUTEX */
        sem_wait(semid, MUTEX, 1);
        printf("Consumer %i got the lock.\n", getpid());
        
        printf("Consumer %i is getting the following products:\n", getpid());
        for ( j = 0; j < i; j++ ){
            printf("%3i", b->data[--b->next]);
            /* la variable es "b->next", por eso el -- se pone antes de eso, no después */
            /* hay un solo b (struct buffer) con un arreglo (que se va a manejar tipo stack)
                que son los espacios de memoria y un apuntador a la primera localidad 
                libre (next). Entonces lo que dice en que en el arreglo data de b, poner en 
                la posición next que tiene b, un número aleatorio del 1 al 3. Después de
                todo eso, next aumenta 1.
                LUEGO, se imprime de b->data[] lo que está en la posición d->next - 1
                (el último lugar ocupado) */
        }
        printf("\n");
        printf("buffer = [");
        for (j = 0; j < b->next; j++){
            printf("%3i", b->data[j]);
        }
        printf("]\n");
        
        sem_signal(semid, MUTEX, 1); /*Regresar el MUTEX de 0 a 1 */
        printf("Consumer %i has released the lock.\n", getpid());
        
        sem_signal(semid, EMPTY, i); /* El companion de sem_wait(semid, FULL, i) */
        
        sleep(rand() % 10 + 1);
    }
    shmdt(b); /* shared memory deattatch: Decir que ya no voy a ocupar la memoria, pero el
                 bloque sigue estando */
    exit(0);
}

/* copiado de semaphores/prooducers.c */
int main(int argc, char* argv[]) {
	int amount, pid, i;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s amount\n", argv[0]);
		return -1;
	}
	
	amount = atoi(argv[1]);
	if (amount < 1) {
		fprintf(stderr, "%s: The amount must be a positive number greater than zero.\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < amount; i++) {
		if ( (pid = fork()) < 0 ) {
			perror(argv[0]);
			return -1;
		} else if (pid == 0) {
			a_consumer(argv[0]);
		} else {
			//do_nothing
		}
	}
	
	return 0;
}
