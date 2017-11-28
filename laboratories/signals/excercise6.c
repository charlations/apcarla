#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* gcc excercise5.c -o ejecutar */
/* ./ejecutar orden1 orden2 orden3 */

int main(int argc, char* argv[]){
    int pid, i;
    
    if(argc > 1 && argc != 4){
        fprintf(stderr, "usage: %s order1 order2 order3 OR %s\n", argv[0], argv[0]);
        return -1;
    }
    if(argc == 1){
        for(i = 0; i < 3; i++){
            if( (pid = fork()) < 0 ) {
                perror(argv[0]);
                return -1;
            } else if(pid == 0){
                switch(i){
                    case 0: execlp("ls", "ls", (char*) 0); break;
                    case 1: execlp("ps", "ps", (char*) 0); break;
                    case 2: execlp("pwd", "pwd", (char*) 0); break;
                }
            } else{
                wait(0);
            }
        }
        return 0;
    }
    for(i = 1; i < argc; i++){
        if( (pid = fork()) < 0 ) {
            perror(argv[0]);
            return -1;
        } else if(pid == 0){
            execlp(argv[i], argv[i], (char*) 0);
            /* es argv[1] porque argv[0] es ./a.out, haciendo que infinítamente se
            corra a sí mismo sin otra instrucción */
            //execlp("ls", "ls", "-aR", (char*) 0);
        } else{
            wait(0);
        }
    }
    
    return 0;
}