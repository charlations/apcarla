#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

void write_output(char* file_in, char* prefix_output, int it){
    int fd, i;
    char filename[NAME_MAX + 1];
    
    sprintf(filename, "%s0%i.dat", prefix_output, it);
    if( (fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ){
        printf("write_input: %s\n", filename);
        perror("analysis");
        exit(-1);
    }
    float avg = find_avg(file_in);
    float var = find_var(file_in, avg);
    char* out;
    sprintf(out, "AVG = %f\nVAR = %f\n");
    write(fd, &out, 25 * sizeof(char));
    /*srand(12345);
    for(i = 0; i < 1000; i++){
        lseek(fd, i * sizeof(int), SEEK_SET);
        write(fd, rand()%100, sizeof(int));
    }*/
    close(fd);
}

float find_avg(char* file_in){
    int fd, sum, i, *data;
    if ( (fd = open(file_in, O_RDONLY)) < 0 ) {
		printf("find_avg: %s\n", file_in);
		perror("analysis");
		return -1;
	}
	
	data = (int *) malloc(1000 * sizeof(int));
	if (data == 0) {
		printf("find_avg: Not enough memory.\n");
		return -1;
	}
	
	read(fd, data, 1000 * sizeof(int));
	close(fd);
	
	sum = 0;
	for(i = 0; i < 1000; i++){
	    sum += data[i];
	}
	free(data);
	data = 0;
	return sum/1000;
}

float find_var(char* file_in, float avg){
    int fd, var, i, *data;
    if ( (fd = open(file_in, O_RDONLY)) < 0 ) {
		printf("find_var: %s\n", file_in);
		perror("analysis");
		return -1;
	}
	
	data = (int *) malloc(1000 * sizeof(int));
	if (data == 0) {
		printf("find_var: Not enough memory.\n");
		return -1;
	}
	
	read(fd, data, 1000 * sizeof(int));
	close(fd);
	
	var = 0;
	for(i = 0; i < 1000; i++){
	    var += ((float)data[i] - avg);
	}
	free(data);
	data = 0;
	return var/1000;
}

