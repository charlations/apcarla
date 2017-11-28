#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//open
#include <fcntl.h>
#include <sys/types.h>


/* gcc decripta.c -o decrypt */
/* ./decrypt origin destination */
/* eje: ./decrypt cripto.txt prueba.txt */

int main(int argc, char* argv[]) {
    FILE *file;
    int fd_in, fd_out;
    int i, size;
    int bytes;
    unsigned char buffer;
    //unsigned char *buffer;
    unsigned char aux;
    unsigned char left, right;
    
    if(argc != 3) {
        printf("usage: %s origin destination\n", argv[0]);
        return -1;
    }
    if( (fd_in = open(argv[1], O_RDONLY)) < 0) {
        printf("%s: No such file or directory\n", argv[0]);
    }
    if( (fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ){
        printf("%s: Could not create file\n", argv[0]);
        return -1;
    }
    
    //MALDITA SEA... >:C
    while( (bytes = read(fd_in, &buffer, sizeof(char))) != 0 ) {
        aux = (buffer << 4) | (buffer >> 4);
        write(fd_out, &aux, bytes);
    }
    /*
    file = fopen(argv[1], "r");
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fclose(file);
    
    buffer = (unsigned char*)malloc(size * sizeof(unsigned char));
    read(fd_in, buffer, size);
    
    for(i = 0; i < size; i++) {
        aux = buffer[i];
        left = aux << 4;
        right = aux >> 4;
        buffer[i] = left | right;
    }
    write(fd_out, buffer, size);
    */
    close(fd_in);
    close(fd_out);
    printf("done\n");
    return 0;
}