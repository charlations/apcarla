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



void recorre(char* directory, char* nom, int n){
    char filename[NAME_MAX + 1];
    char newfilename[NAME_MAX + 1];
    DIR *dir;
    struct dirent *dir_entry;
    char* ext;
    char* aux;
    int res;
    
    if( (dir = opendir(directory)) == NULL ){
        exit(-1);
    }
    while( (dir_entry = readdir(dir)) != NULL ){
        sprintf(filename, "%s", dir_entry->d_name);
        if(strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0){
            continue;
        }
        if( (strstr(filename, nom)) != NULL ){
            //es uno de los archivos que estamos buscado
            ext = strrchr(filename, '.');
            if (!ext) {
                //no extension
                exit(-1);
            }
            if(strcmp(ext, ".log")){
                sprintf(newfilename, "%s.1", nom);
                res = rename(filename, newfilename);
            } else{
                int i;
                for (i = 1; i < n; i++){
                    sprintf(newfilename, ".%i", i);
                }
            }
        }
    }
    
    
}

int main(int argc, char* argv[]){
    /**TODO: 
     * asegurarse que hayan 4 parámetros directorio nomArchivo numCopias tiempo
     * asegurarse que se pueda exista el directorio
     * asegurarse que exista nomArchivo.log
     * asegurarse que numCopias sea > 1
     * //verifica que sea un entero positivo.
        int isPosInt(char num[]){
          int i;
          for(i = 0; num[i] != 0; i++){
            if(!isdigit(num[i])){
              return 0;
            }
          }
          return 1;
        }
     * asegurarse de tener permisos de reading y writing en directorio
     * while(1){
         recorre();
         wait(tiempo);
     }
    **/
    return 0;
}