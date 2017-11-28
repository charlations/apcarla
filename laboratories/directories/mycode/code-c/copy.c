#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int fd_in, fd_out, nbytes;
	char buffer;
	
	if (argc != 3) {
		printf("usage: %s source_file destination_file\n", argv[0]);
		return -1;
	}
	
	if ( (fd_in = open(argv[1], O_RDONLY)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	if ( (fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror(argv[0]);
		return -1;
	}
	
	while ( (nbytes = read(fd_in, &buffer, sizeof(char))) != 0 ) {
		write(fd_out, &buffer, nbytes);
	}
	
	close(fd_in);
	close(fd_out);
	return 0;
}
