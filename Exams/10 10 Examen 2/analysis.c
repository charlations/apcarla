#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include "one.h"
#include "two.h"

/* gcc one.c two.c analysis.c -o analysis */
/* analysis timeout prefix_input prefix_output */

int main(int argc, char* argv []){
    int timeout;
    
    if(argc != 4){
        printf("usage: %s timeout prefix_input prefix_output.\n", argv[0]);
		return -1;
    }
    timeout = atoi(argv[1]);
    if(timeout <= 0){
        printf("%s: the number of processes must be greater than 1.\n", argv[0]);
    	return -1;
    }
    
    
    return 0;
}