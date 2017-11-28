#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* compile: gcc 09-12-proc0.c       */
/* execute: ./a.out    */

int main(int argc, char* argv[]) {
    int pid, result, retid;
    if (argc != 1){
        fprintf(stderr, "usage: %s\n", argv[0]);
        return -1;
    }
    
    if( (pid = fork()) < 0 ){
        perror(argv[0]);
        return -1;
    } else if (pid == 0){ //child
        sleep(2);
        printf("CHILD PID = %i ppid = %i\n", getpid(), getppid());
        exit(42);
    } else if (pid > 0) { //parent
        printf("PARENT PID = %i PPID = %i\n", getpid(), getppid());
        wait(&result);
        printf("EXIT STATUS = %i\n", WEXITSTATUS(result));
    }
    
    
    return 0;
}