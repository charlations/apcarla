#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig){
    switch(sig) {
        case SIGUSR1 : printf("PID = %i, SIGNAL USR1\n", getpid());
        break;
        case SIGUSR2 : printf("PID = %i, SIGNAL USR2\n", getpid());
        break;
    }
}

int main(int argc, char * argv[]) {
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    
    while(1){
        printf("PID %i waiting for a signal\n", getpid());
        pause();
    }
    return 0;
}