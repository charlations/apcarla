#include "header.h"

/* gcc producers.c -o pizzeria */
/* ./pizzeria & */

int main(int argc, char* argv[]) {
    int semid;
    key_t key;
    
    if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		return -1;
	}
	
	if ( (key = ftok("/dev/null", 125)) == (key_t) -1 ) {
		perror(argv[1]);
		exit(-1);
	}
	
	if ( (semid = semget(key, 3, 0666)) < 0 ) {
		perror(argv[1]);
		exit(-1);
	}
	
	while(1) {
	    printf(" All Night Long Pizza is selling pizza");
	    sem_wait(semid, EMPTY, 1);
	    sem_signal(semid, FULL, 1);
	}
	
	return 0;
}