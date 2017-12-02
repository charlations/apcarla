#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> /* strrchr */
#include <fcntl.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

/* compile: gcc rotacion.c -o rotacion       */
/* execute: ./rotacion directorio nomArchivo numCopias tiempo   */



void recorre(char* filename, char* nom, int n){
    FILE *newFile;
    char newfilename[NAME_MAX + 1];
    char* ext;
    char* aux;
    int res, i;
    
    printf("'recorriendo' %s\n", filename);
    
    ext = strrchr(filename, '.');
    if (ext == NULL) {
        //no extension
        exit(0);
    }
    if(strcmp(ext, ".log") == 0){
        sprintf(newfilename, "%s.1", nom);
        res = rename(filename, newfilename);
        printf("\tSe cambió el nombre de %s a %s\n", filename, newfilename);
        if( (newFile = fopen(filename, "w+")) == NULL ){
            perror("fopen write");
            exit(-1);
        }
        printf("\tSe creo el nuevo archivo %s\n", filename);
        fclose(newFile);
    } else{
        res = 1;
        i = 0;
        while (i < n && res) {
            sprintf(aux, ".%i", i);
            if(strcmp(ext, aux) == 0){
                res = 0;
            }
            i++;
        }
        if(i < n) {
            sprintf(newfilename, "%s.%i", nom, i);
            res = rename(filename, newfilename);
            printf("\tSe cambió el nombre de %s a %s\n", filename, newfilename);
            if( (newFile = fopen(filename, "w+")) == NULL ){
                perror("fopen write");
                exit(-1);
            }
            printf("\tSe creo el nuevo archivo %s\n", filename);
            fclose(newFile);
        } else {
            res = remove(filename);
            printf("\tSe borró %s\n", filename);
        }
    }

}

void circula(char* directory, char* nom, int numCopias, int t, char* program) {
    DIR *dir;
    struct dirent *dir_entry;
    struct stat ss;
    char filename[NAME_MAX + 1];
    char aux[NAME_MAX + 1];
    int existe = 0;
    
    if( (dir = opendir(directory)) == NULL ) {
        perror("Error al abrir el directorio.\n");
        exit(-1);
    }
    printf("Se abrió el directorio %s\n", directory);
    sprintf(aux, "%s.log", nom);
    while( (dir_entry = readdir(dir)) != NULL ){
        sprintf(filename, "%s", dir_entry->d_name);
        if(strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0){
            continue;
        }
        if( (strcmp(aux, filename)) == 0 ){
            //es uno de los archivos que buscamos
            existe = 1;
            printf("Se encontró %s\n", aux);
        }
    }
    if(!existe) {
        perror("No se encontro el archivo .log\n");
        exit(-1);
    }
    rewinddir(dir);
    
    while(existe){
        printf("Loop de existe\n");
        while( (dir_entry = readdir(dir)) != NULL ){
            sprintf(filename, "%s", dir_entry->d_name);
            if(strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0){
                continue;
            }
            if( (strstr(filename, nom)) != NULL ){
                //es uno de los archivos que buscamos
                recorre(filename, nom, numCopias);
            }
        }
        sleep(t);
        rewinddir(dir);
    }
}

int main(int argc, char* argv[]){
    int numCopias, tiempo;
    
    if(argc != 5) {
        fprintf(stderr, "usage: %s directorio nomArchivo numCopias tiempo", argv[0]);
        return -1;
    }
    numCopias = atoi(argv[3]);
    if(numCopias <= 0) {
        fprintf(stderr, "%s: numCopias debe ser un entero mayor a 0\n", argv[0]);
        return -1;
    }
    tiempo = atoi(argv[4]);
    if(tiempo <= 0) {
        fprintf(stderr, "%s: tiempo debe ser un entero mayor a 0\n", argv[0]);
        return -1;
    }
    
    printf("Abriendo directorio %s\n", argv[1]);
    circula(argv[1], argv[2], numCopias, tiempo, argv[0]);
    
    return 0;
}