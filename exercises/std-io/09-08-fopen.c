#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> //Aquí están los memset

#define SIZE 1000
#define WIDTH 73

/* compile: gcc 09-08-fopen.c -o histo       */
/* execute: ./histo file   */

int values[SIZE];

void read_values(char* filename, char* program) {
    FILE *file;
    int time, intersections, size, i;
    float num;
    
    file = fopen(filename, "r");
    if(file == NULL){
        perror(program);
        exit(-1);
    }
    //inicializa todo el arreglo en 0 (el segundo parámetro)
    memset(values, 0, SIZE * sizeof(int));
    
    //Opción 1: si no sé cuánto voy a estar leyendo
    /*
    while ( fscanf(file, ",%f", &num) ){
    }
    //QUE ES LO MISMO QUE
    while ( (fscanf(file, ",%f", &num)) != 0 ){
    }
    */
    //Opción 2: conociendo el formato (sé que me va a dar el tiempo, y luego el número de intesecciones)
    /*
    fscanf(file, "%i,%i", &time, &intersections);
    */
    //Opción 3: combinada (igual se sabe el formato)
    //Lee de línea por línea hasta llegar a EOF
    while( fscanf(file,"%i,%i", &time, &intersections) != EOF ){
        size = intersections * intersections;
        for(i = 0; i < size; i++){
            fscanf(file, ",%f", &num);
            values[(int)num]++;
        }
    }
    fclose(file);
}

int max (int a, int b){
    return ( (a > b) ? a : b );
}
/**escalar las instancias de cada número, NO EL NÚMERO!!!
 * O sea es hacer que no haya algún valor con 1000 repeticiones o algo así, para
 *  que no salgan 1000 estrellitas en nuestro output.
**/
int convert (int val, int width, int max){
    return (int) ((val * width) / max);
}
void display_histogram(){
    int i, m, j;
    m = 0;
    for (i = 1; i < SIZE; i++) {
        m = max(m, values[i]);
    }
    for (i = 1; i < SIZE; i++){
        values[i] = convert(values[i], WIDTH, m);
    }
    for(i = 1; i < SIZE; i++){
        if(values[i] != 0){
            fprintf(stdout, "[%2i] : ", i);
            for (j = 0; j < values[i]; j++){
                fprintf(stdout, "*");
            }
            fprintf(stdout, "\n");
        }
    }
}
/**El problema con esto es que cuando hay un valor con muy pocas instancias
 * comparado con el que más tiene, su contador convertido se vuelve 0, 
 * eliminando el que aparescan en el histograma... Se puede evitar esto
 * poniendo los valores con cero asteriscos así:
 * 
 * void display_histogram(){
        int i, m, j, val;
        m = 0;
        for (i = 1; i < SIZE; i++) {
            m = max(m, values[i]);
        }
        for(i = 1; i < SIZE; i++){
            if(values[1] != 0){
                val = convert(values[i], WIDHT, m);
                fprintf(stdout, "[%2i] : ", i);
                for (j = 0; j < val; j++){
                    fprintf(stdout, "*");
                }
                fprintf(stdout, "\n");
            }
        }
    }
**/
int main (int argc, char* argv[]){
    if(argc != 2){
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return -1;
    }
    
    read_values(argv[1], argv[0]);
    display_histogram();
    
    return 0;
}