#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//Sacar tipos de archivos
#include <sys/stat.h>
//navegar el directorio
#include <dirent.h>
//strcpy y demás
#include <string.h>

/* gcc excercise3.c -o estadistica */
/* ./estadistica [directorio] */
float count = 0;
float dir = 0;
float chr = 0;
float blk = 0;
float reg = 0;
float lnk = 0;
float sck = 0;
float ffo = 0;

void printStats() {
    int porcentaje;
    printf("\tTIPO\tPORCENTAJE\n");
    //printf("Regulares: %f\n", reg);
    if(dir > 0) {
        porcentaje = (int)((dir / count) * 100);
        printf("\tDIR\t%i%%\n", porcentaje);
    }
    if(chr > 0) {
        porcentaje = (int)((chr / count) * 100);
        printf("\tCHR\t%i%%\n", porcentaje);
    }
    if(blk > 0) {
        porcentaje = (int)((blk / count) * 100);
        printf("\tBLOCK\t%i%%\n", porcentaje);
    }
    if(reg > 0) {
        porcentaje = (int)((reg / count) * 100);
        printf("\tFILE\t%i%%\n", porcentaje);
    }
    if(lnk > 0) {
        porcentaje = (int)((lnk / count) * 100);
        printf("\tLINK\t%i%%\n", porcentaje);
    }
    if(sck > 0) {
        porcentaje = (int)((sck / count) * 100);
        printf("\tSOCK\t%i%%\n", porcentaje);
    }
    if(ffo > 0) {
        porcentaje = (int)((ffo / count) * 100);
        printf("\tFIFO\t%i%%\n", porcentaje);
    }
    //printf("\n");
    //resetear a 0
    count = 0;
    dir = 0;
    chr = 0;
    blk = 0;
    reg = 0;
    lnk = 0;
    sck = 0;
    ffo = 0;
}

void statisticfy(char* directory, char* name, char* program) {
    char filename[NAME_MAX + 1];
    struct stat ss;
    
    sprintf(filename, "%s/%s", directory, name);
    if(lstat(filename, &ss) == -1 ){
        perror(program);
        exit(-1);
    }
    
    /* https://www.gnu.org/software/libc/manual/html_node/Testing-File-Type.html
    
S_ISCHR (mode) is equivalent to: ((mode & S_IFMT) == S_IFCHR)
Macro: int S_IFMT
This is a bit mask used to extract the file type code from a mode value. 
    */
    switch(ss.st_mode & S_IFMT){
        case S_IFDIR: dir++;
        break;
        case S_IFCHR: chr++;
        break;
        case S_IFBLK: blk++;
        break;
        case S_IFREG: reg++;
        break;
        case S_IFLNK: lnk++;
        break;
        case S_IFSOCK: sck++;
        break;
        case S_IFIFO: ffo++;
        break;
        default:
        break;
    }
    count++;
    //printf("Count: %f\n", count);
}

void list(char *directory, char *program) {
    char filename[NAME_MAX + 1];
    DIR *dir;
    struct dirent *dir_entry;
    struct stat ss;
    
    if( (dir = opendir(directory)) == NULL ) {
        perror(program);
        exit(-1);
    }
    
    printf("Directorio %s\n", directory);
    while( (dir_entry = readdir(dir)) != NULL ) {
        if (strcmp(dir_entry->d_name, ".") == 0 || 
            strcmp(dir_entry->d_name, "..") == 0) {
                continue;
        }
        statisticfy(directory, dir_entry->d_name, program);
    }
    printStats();
    //Subfolders
    rewinddir(dir);
    while( (dir_entry = readdir(dir)) != NULL ) {
        if (strcmp(dir_entry->d_name, ".") == 0 || 
            strcmp(dir_entry->d_name, "..") == 0) {
                continue;
        }
        sprintf(filename, "%s/%s", directory, dir_entry->d_name);
        stat(filename, &ss);
        if (S_ISDIR(ss.st_mode)) {
            list(filename, program);
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]) {
    //Para no hacer malloc, hace directory referencia a dir_name
    char dir_name[NAME_MAX + 1];
    char *directory;
    
    strcpy(dir_name, ".");
    directory = dir_name;
    if(argc > 2) {
        fprintf(stderr, "usage: %s directory\n", argv[0]);
        return -1;
    }
    if(argc == 2){
        directory = argv[1];
    }
    
    list(directory, argv[0]);
    
    return 0;
}