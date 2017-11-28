/* aunque se puede hacer todo en un solo archivo, vamos a separar en 2 archivos:
uno que inicialize las variables y otro que use los productores y consumidores */

#include "10 20 header.h"

/* gcc 10\ 20\ buffer.c */

int main(int argc, char* argv[]){
    int i, semid, size;
    unsigned short final_values[4];
    key_t key; /* sys/ipc */
    
    if(argc != 2){
        fprintf(stderr, "usage: %s size\n", argv[0]);
        return -1;
    }
    
    size = atoi(argv[1]);
    if (size < 1){
        fprintf(stderr, "%s: size must be a positive number greater than zero\n", argv[1]);
        return -1;
    }
    
            /* genera mi llave que me sirven para generar un código hash 
            no necesita realmente información. El primer parámetro es su nombre, 
            que puede ser el que quisiera. En este caso se está poniendo "null" 
            como nombre. El segundo parámetro es un caractér (de 0 a 255). Si 
            quiero luego recuperar la información de la llave, debo usar la misma
            combinación.*/
    if( (key = ftok("/dev/null", 125)) == (key_t) -1 ) {
        perror(argv[0]);
        return -1;
    }
    /* bajo esta llave crea 4 semáforos con estos permisos y como los estoy creando, ocupo el IPC_create 
       el semget es para crear un semáforo. Igual hay otros tipos que luego vamos a ver (de arreglos, 
       pilas, y demás */
    if( (semid = semget(key, 4, 0666 | IPC_CREAT)) < 0 ){
        perror(argv[0]);
        return -1;
    }
    
    /* dar valores a los semáforos creados */
    /*    semid es el id del recurso dentro del sistema, que tiene un arreglo de semáforos.
          el orden de los parámetros es: idRecurso, semáforo al que (o a partir del que) se va a
          afectar, la orden a hacer y el valor de la orden a hacer */
    semctl(semid, PRODUCERS, SETVAL, 1);
    semctl(semid, CONSUMERS, SETVAL, 1);
    semctl(semid, ITEMS,     SETVAL, 0);
    semctl(semid, SPACES,    SETVAL, size);
    
    semctl(semid, PRODUCERS, GETALL, final_values);
    printf("value: ");
    for (i = 0; i < 4; i++){
        printf("%3i", final_values[i]);
    }
    printf("\n");
    return 0;
}