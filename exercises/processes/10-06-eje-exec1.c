#include <stdio.h>
#include <unistd.h>

/* gcc 10-06-eje-exec1.c */
/* ./a.out ls -a | sort -r | more  (o cualquier otro comando que quiera correr)*/

int main(int argc, char* argv[]){
    int pid;
    
    if (argc < 2){
        fprintf(stderr, "usage: %s cmd1 cmd2 ... cmdn\n", argv[0]);
        return -1;
    }
    
    printf("PARENT PID = %i\n", getpid());
    if( (pid = fork()) < 0 ) {
        perror(argv[0]);
        return -1;
    } else if(pid == 0){
        printf("CHILD PID = %i\n", getpid());
        execvp(argv[1], &argv[1]);
        /* es argv[1] porque argv[0] es ./a.out, haciendo que infinítamente se
        corra a sí mismo sin otra instrucción */
        //execlp("ls", "ls", "-aR", (char*) 0);
    } else{
        //do nothing
        /* Aquí no se necesita un "wait(NULL);", porque en cuanto se corre el 
        exec, se destruye el programa y se considera como acabado el padre */
    }
    
    printf("PARENT ID = %i ENDED\n", getpid());
    return 0;
}