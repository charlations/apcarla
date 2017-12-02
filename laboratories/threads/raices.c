#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//threads
#include <pthread.h>
//mate
#include <math.h>

/* gcc raices.c -lpthread -o raiz -lm */

float raiz[10][10];
//char mensaje[] = "Computando, espere por favor.\n";
int acabo = 0;

void* task(void* param) {
    int i, j, tarea;
    
    tarea = *( (int *) param ); //what...
    
    if(!tarea) {
        for(i = 0; i < 10; i++) {
            for(j = 0; j < 10; j++) {
                raiz[i][j] = sqrt(tarea);
                tarea++;
            }
        }
        acabo = 1;
    } else {
        printf("Computando, espere por favor.\n");
        i = 0;
        while(!acabo && i < 10) {
            sleep(1);
            i++;
        }
        for(i = 0; i < 10; i++) {
            for(j = 0; j < 10; j++) {
                printf("%f |", raiz[i][j]);
            }
            printf("\n");
        }
    }
    sleep( 1 );
    return ( (void*) 0);
}

int main(int argc, char* argv[]) {
    pthread_t pthread_id[2];
    int ntarea1 = 1;
    int ntarea0 = 0;
    void* ret;
    
    pthread_create(&pthread_id[0], NULL, task, (void *) &ntarea1);
    pthread_create(&pthread_id[1], NULL, task, (void *) &ntarea0);
    
    pthread_join(pthread_id[0], &ret);
    pthread_join(pthread_id[1], &ret);
    
    return 0;
}
/**
 * Desarrolla un programa que calcule las raíces cuadradas de los números 
 * enteros de 0 a 99 en un hilo separado y devuelve una matriz de dobles que 
 * contienen los resultados. Mientras tanto, el hilo principal debe mostrar un 
 * mensaje corto para el usuario y, a continuación mostrar los resultados de la 
 * computación cuando están listos.
**/ 