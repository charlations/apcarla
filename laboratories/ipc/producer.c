#include "header.h"

/* gcc producer.c -o barbero */
/* ./barbero & */

int main(int argc, char* argv[]) {
    int semid;
    key_t key;
    
    if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		return -1;
	}
	if( (key = ftok("/dev/null", 125)) == (key_t) -1 ) {
        perror(argv[0]);
        return -1;
    }
    if( (semid = semget(key, 5, 0666)) < 0 ){
        perror(argv[0]);
        return -1;
    }
    
    while(1) {
        sem_wait(semid, EMPTY, 1);
        printf("El barbero se está durmiendo.\n");
        sem_wait(semid, FULL, 1);
    }
    
    
    return 0;
}