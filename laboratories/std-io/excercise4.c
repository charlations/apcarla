#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//navegar el directorio
#include <dirent.h>
//String
#include <string.h>

/* gcc excercise4.c -o normalize */
/* ./normalize directory newDirectory */

#define SIZE 1000
int val[SIZE];
int max;

void findMax(){
    int i = SIZE;
    while (!val[i]) {
        i--;
    }
    max = i;
}

float normalizando(float num) {
    return num / (float) max;
}

void normalize(char *directory, char *name, char *newDirectory, char *program) {
    FILE *file, *newFile;
    int t, intersections, size, i;
    float num;
    char filename[NAME_MAX + 1], newFilename[NAME_MAX + 1];
    
    sprintf(filename, "%s/%s", directory, name);
    sprintf(newFilename, "%s/%s", newDirectory, name);
    
    //printf("\tNormalize: file = %s newFile = %s\n", filename, newFilename);
    if( (file = fopen(filename, "r")) == NULL ) {
        perror("fopen read");
        exit(-1);
    }
    //printf("\tpasó fopen read\n");
    if( (newFile = fopen(newFilename, "w")) == NULL ) {
        perror("fopen write");
        exit(-1);
    }
    //printf("\tpasó fopen write\n");
    
    while( fscanf(file, "%i,%i", &t, &intersections) != EOF ) {
        fprintf(newFile, "%i,%i", t, intersections);
        size = intersections * intersections;
        for(i = 0; i < size; i++) {
            fscanf(file, ",%f", &num);
            //printf("Normalize -> %s: %f", filename, num);
            fprintf(newFile, ",%f", normalizando(num));
        }
        fprintf(newFile, "\n");
    }
    fclose(file);
    fclose(newFile);
}

void output(char *directory, char *newDirectory, char *program) {
    DIR *dir;
    struct dirent *dir_entry;
    struct stat *ss;
    
    //printf("\tinicia output\n");
    if( (dir = opendir(directory)) == NULL ) {
        perror(program);
        exit(-1);
    }
    if (stat(newDirectory, &ss) == -1) {
        mkdir(newDirectory, 0700);
        //printf("\tcreo directorio nuevo\n");
    }
    while((dir_entry = readdir(dir)) != NULL ){
        if (strcmp(dir_entry->d_name, ".") == 0 || 
            strcmp(dir_entry->d_name, "..") == 0) {
                continue;
        }
        normalize(directory, dir_entry->d_name, newDirectory, program);
    }
    closedir(dir);
}

void getVal(char *directory, char *name, char *program) {
    FILE *file;
    int t, intersections, size, i;
    int num;
    char filename[NAME_MAX + 1];
    
    sprintf(filename, "%s/%s", directory, name);
    
    if( (file = fopen(filename, "r")) == NULL ) {
        perror("fopen");
        exit(-1);
    }
    
    memset(val, 0, SIZE * sizeof(int));
    
    while ( fscanf(file, "%i,%i", &t, &intersections) != EOF ) {
        size = intersections * intersections;
        for(i = 0; i < size; i++) {
            fscanf(file, ",%i.00", &num);
            val[num] ++;
        }
    }
    fclose(file);
}

void input(char *directory, char* program) {
    DIR *dir;
    struct dirent *dir_entry;
    
    if( (dir = opendir(directory)) == NULL ) {
        perror(program);
        exit(-1);
    }
    while((dir_entry = readdir(dir)) != NULL ){
        if (strcmp(dir_entry->d_name, ".") == 0 || 
            strcmp(dir_entry->d_name, "..") == 0) {
                continue;
        }
        getVal(directory, dir_entry->d_name, program);
    }
    closedir(dir);
}


int main (int argc, char* argv[]) {
    
    if (argc != 3) {
        fprintf(stderr, "usage: %s directory newDirectory\n", argv[0]);
        return -1;
    }
    
    input(argv[1], argv[0]);
    //printf("Acabó input\n");
    findMax();
    //printf("Acabó findMax\n");
    output(argv[1], argv[2], argv[0]);
    //printf("Acabó output\n");
    
    return 0;
}