#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//threads
#include <pthread.h>

/* gcc jardin.c -lpthread -o jardin */


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int cant = 0;

void* entrada(void *arg) {
    int i, r, ent;
    printf("Se abrieron las puertas.\n");
    
    for(i = 0; i < 50; i++) {
        pthread_mutex_lock(&mutex);
        r = (rand() % 10) + 1;
        ent = (rand() % 1);
        if(r > cant) { ent = 1; }
        if(ent) {
            cant += r;
        } else {
            cant -= r;
        }
        pthread_mutex_unlock(&mutex);
        printf("Personas en el jardín: %i\n", cant);
    }
    
    
}

int main(int argc, char* argv[]) {
    pthread_t entradas_thread[2];
    
    pthread_create(&entradas_thread[0], NULL, entrada, NULL);
    pthread_create(&entradas_thread[1], NULL, entrada, NULL);
    
    pthread_join(entradas_thread[0], NULL);
    pthread_join(entradas_thread[1], NULL);
    
    printf("Acabó el día, sacando a %i personas.\n", cant);
    
    return 0;
}
/**
 * El jardín botánico de la Ciudad de México se encuentra abierto a todo el 
 * público. Cualquier persona puede entrar por uno de los dos accesos que tiene 
 * el jardin: hacia el este y oeste del mismo. La administración desea determinar
 * cuántas personas hay en el jardín en un momento dado. Diseña una solución en 
 * pthreads hacer esto. El programa representará cada entrada con un pthread y 
 * actualiza un contador compartido.
**/