/**
* To compile: gcc garden.c -o garden -lpthread
**/

#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define SIZE 1000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t people_garden = PTHREAD_COND_INITIALIZER;

long int size = 0;

void update_garden() {
    int i, random1, random2;
    for (i = 0; i < 100; i++) {
   		pthread_mutex_lock(&mutex);
   		if (size == SIZE) {
       		pthread_cond_wait(&people_garden, &mutex); 
   		}
   		random1 = (rand() % 30);
		size += random1;
		pthread_mutex_unlock(&mutex);
       	random2 = (rand() % 20);
		if ( size > random2) {
    		pthread_mutex_lock(&mutex);
    		size -= random2;
        	pthread_cond_signal(&people_garden);
        	pthread_mutex_unlock(&mutex);
		}
  		printf("People in Garden %li...\n", size);
	}
}

void* east(void *arg) {
	printf("East entrance opened...\n");
	update_garden();
	pthread_exit(NULL);
}

void* west(void *arg) {
	printf("West entrance opened...\n");
	update_garden();
	pthread_exit(NULL);
}
 
int main(int argc, char* argv[])   {
    time_t t;
	pthread_t west_thread; 
	pthread_t east_thread; 
	srand((unsigned) time(&t));
	
	pthread_create(&west_thread, NULL, west, NULL);
    pthread_create(&east_thread, NULL, east, NULL);
    pthread_join(west_thread, NULL);
    pthread_join(east_thread, NULL);
    printf("Garden closed\n");
    if (size) {
	    pthread_mutex_lock(&mutex);
    	size -= size;
        pthread_cond_signal(&people_garden);
        pthread_mutex_unlock(&mutex);
	}
    return 0;
}


