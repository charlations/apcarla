/**¿De cuál problema de sincronización se trata?
 *** Un problema de buffers. Como el ejemplo 5.1 del libro
 *** "The Little Book of Semaphores"
 * Define la estrategia y mecanismos que emplearás la solución
    del problema. Fundamenta, de manera clara y concisa, cada
    una de tus decisiones.
 *** Definir un buffer de pizza, un consumidor (estudiantes) y un productor (pizzeria)
 *** El estudiante es quien define cuantas rebanadas de pizza hay, para que solo hayan
 *** 3 semáforos, en lugar de 5.
 *** cuado se vacía, se llama EMPTY para que la pizzería les "venda" otra pizza
 * Implementación del problema. Es importante que el
    comportamiento de su solución sea idéntico al problema del
    examen y que reciba como parámetro la cantidad de
    rebanadas que tiene la pizza.
 *** pizza.c defne los semáforos. consumers consume una variable local que define
 *** la cantidad de rebanadas
 *** el producer define que ya se rellenó la pizza.
 *** como compilar y correr está en cada archivo, para probarlo.
 * Deberás colocar un archivo de texto explicando cómo
    debe ser probado tu programa. Si no existe el archivo
    se aplicará una penalización de 20 puntos aun cuando
    el problema sea correcto
**/

/* gcc pizza.c */

#include "header.h"

int main(int argc, char* argv[]) {
    int semid;
    key_t key;
    
    if(argc != 1){
        fprintf(stderr, "usage: %s\n", argv[0]);
        return -1;
    }
    
    if( (key = ftok("/dev/null", 125)) == (key_t) -1 ) {
        perror(argv[0]);
        return -1;
    }
    
    if( (semid = semget(key, 3, 0666 | IPC_CREAT)) < 0 ){
        perror(argv[0]);
        return -1;
    }
    
    semctl(semid, MUTEX, SETVAL, 1);
    semctl(semid, FULL, SETVAL, 1);
    semctl(semid, EMPTY, SETVAL, 0);
}