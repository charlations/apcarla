#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

#define SIZE 10000

/* compile: gcc examen1.c -o find       */
/* execute: ./find word [directory]   */

char dir_name[NAME_MAX + 1];
char *imprimeDirectorio = ".";
//verificar que sea un directorio que se puede abrir
int verDid(char *addr){
    DIR *dr = opendir(addr);
    return (dr == NULL);
}
//Recorre todos los archivos 
void recorre(char *directory, const char *word){
    //printf("Entro a recorre\n");
    
    char filename[NAME_MAX + 1];
    DIR *dir;
    struct dirent *dir_entry;
    struct stat ss;
    
    if ( (dir = opendir(directory)) == NULL ) {
		exit(-1);
	}
    //printf("Pudo abrir el directorio\n");
    while( (dir_entry = readdir(dir)) != NULL ){
        sprintf(filename, "%s/%s", directory, dir_entry->d_name);
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..")  == 0){
            //printf("Entró a . o ..\n");
            continue;
        }
        //puede acceder a los lstats?
        if(lstat(filename, &ss) == -1){
            printf("No pudo acceder a los lstats\n");
            exit(-1);
        }
        //Primero solo revisamos archivos, para hacer bien el formato de folders y así
        //Es archivo
        if(S_ISREG(ss.st_mode)){
            if(busca(filename, word)){
                printf("    >   %s\n", filename);
            }
        }
    }
    printf("\n");
    //Ahora entra a cada directorio
    rewinddir(dir);
    while( (dir_entry = readdir(dir)) != NULL ){
        sprintf(filename, "%s/%s", directory, dir_entry->d_name);
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..")  == 0){
            //printf("Entró a . o ..\n");
            continue;
        }
        //Es directorio
        stat(filename, &ss);
        if(S_ISDIR(ss.st_mode)){
            printf("%s:\n", filename);
            recorre(filename, word);
        }
    }
}
//Busca word dentro del archivo, si lo encuentra, regresa true
int busca(char *file, char *word){
    //printf("Entro a busca %s\n", file);
    int fd_in, found;
    char buffer[SIZE];
    
    if ( (fd_in = open(file, O_RDONLY)) < 0 ) {
		return 0;
	}

    memset(buffer, 0, SIZE * sizeof(char));
    found = 0;
    /*Lo que se puede hacer para que el buffer no divida la
    palabra que estamos buscando en 2 es:
    Tomas un buffer de 10,000 inicial y LUEGO entras a un while
    con el buffer iniciando menos el tamaño de la palabra buscada 
    */
    while( read(fd_in, buffer, SIZE * sizeof(char)) != 0 ){
        if( (strstr(buffer, word)) != NULL ) {
            found = 1;
            break;
        }
    }
    
    close(fd_in);
    return found;
}


int main(int argc, char* argv[]){

    if(argc < 2 || argc > 3){
        fprintf(stderr, "Usage: ./find word [directory]\n");
        return -1;
    }
    if(argc == 3){
        sprintf(dir_name, "./%s", argv[2]);
        imprimeDirectorio = dir_name;
    } else {
        sprintf(dir_name, ".");
    }
    //printf("Busca dentro de \"%s\"\n", dir_name);
    if(verDid(dir_name)){
        fprintf(stderr, "Error: no se puede abrir el directorio \"%s\"\n", dir_name);
        return -1;
    }
    //printf("prueba1\n");
    char *word = argv[1];
    //printf("prueba2\n");
    printf(".:\n");
    recorre(dir_name, word);
    //printf("Acabo el recorre\n");
    return 0;
}