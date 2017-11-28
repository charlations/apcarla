#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

void write_input(char* prefix_input, int it){
    int fd, i, r;
    char filename[NAME_MAX + 1];
    
    sprintf(filename, "%s0%i.dat", prefix_input, it);
    if( (fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ){
        printf("write_input: %s\n", filename);
        perror("analysis");
        exit(-1);
    }
    srand(12345);
    for(i = 0; i < 1000; i++){
        r = rand()%100;
        lseek(fd, i * sizeof(int), SEEK_SET);
        write(fd, &r, sizeof(int));
    }
    close(fd);
}
