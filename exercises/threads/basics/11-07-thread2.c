/**
* To compile: gcc 11-07-thread2.c -lpthread
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define LIMIT 10

void* task(void* param) {
	int i, limit;
	
	limit = *( (int *) param ); /* Como yo ya sé que la el parámetro es un entero,
	                                le puedo hacer un casteo. Y para no trabajar
	                                con apuntadores, lo paso a una variable. */
	for (i = 1; i <= limit; i++) {
		printf("PID = %i - THREAD %i - i = %i\n", getpid(), (int) pthread_self(), i);
	}
	sleep( 1 );
	return ( (void*) 0);
}

int main(int arg, char* argv[]) {
	pthread_t pthread_id[10];
	int i;
	void* ret;
	
	printf("PID = %i - creating threads\n", getpid());
	for (i = 0; i < 10; i++) {
	    int limit = (i + 1) * 5; /* se le está dandon un límite diferente. El primer 
	                            thread despliega 5, el segundo 10, el tercero
	                            15, hasta llegar a 50. */
	   /* es una condición de carrera: cuántas threads se generan con limit antes
	    de que limit cambie. */
		pthread_create(&pthread_id[i], 
			            NULL, task, (void *) &limit); /* El límite es un bloque
			                                        de memoria que luego es 
			                                        interpretado como un número
			                                        (void*) es un bloque de 
			                                        memoria, entonces puede mandar
			                                        sin ningún problema cualquier
			                                        tipo de variable bajo ese
			                                        parámetro */
	}
	for (i = 0; i < 10; i++) {
		pthread_join(pthread_id[i], &ret);
	}
	printf("PID = %i - all threads are terminated\n", getpid());
	return 0;
}
