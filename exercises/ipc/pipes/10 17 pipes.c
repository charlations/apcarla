#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* Todas las pipas están aquí, WIN32 no tiene la librería de PIPES :C */ 
#include <sys/types.h>

void execute(char* order1, char* order2, char* order3, char* program){
    /* creación de las pipes de stdin y stdout de P1 a P2 y de P2 a P3 */
    int p1p2[2], p2p3[2];
    int pid;
        /* comando de creación de una tubería sin nombre */
    if( pipe(p1p2) < 0 ) {
        perror(program);
        exit(-1);
    }
    
    if( (pid=fork()) < 0 ) {
        perror(program);
        exit(-1);
    } else if(pid == 0) {
        // es PA
        if ( pipe(p2p3) < 0 ) {
            perror(program);
            exit(-1);
        }
        if ( (pid = fork()) < 0 ) {
            perror(program);
            exit(-1);
        } else if (pid == 0){
            //es CHILD
            /* redireccionar salida del CHILD a la tubería p2p3 */
            close(1); /* stdout default: pantalla AHORA ESTÁ CERRADA */
            dup(p2p3[1]);
            close(p2p3[0]); /* nunca se usó */
            close(p2p3[1]); /* ya está siendo apuntado por sdtout */
            execlp(order1, order1, (char*) 0);
        } else {
            // CÓDIGO DEL PROCESSO PADRE
            /*reescribir el stdin (predet: teclado) */
            close(0);
            dup(p2p3[0]); /*recuerda que dup va al fd más pequeño que encuentre,
                            por eso dup(p2p3[0]) se va al fd0 */
            close(p2p3[0]);
            close(p2p3[1]);
            
            /*reescribir el stdout (predet: pantalla) */
            close(1);
            dup(p1p2[1]);
            close(p1p2[0]);
            close(p1p2[1]);
            
            execlp(order2, order2, (char*) 0);
        }
        
    } else {
        //es GRANDPA
        close(0); /* estás cerrando el fileDescriptor 0 (input) */
        dup(p1p2[0]); /* le estás asignando el fd0(default: entrada de teclado)
                        [previamente apartado, pero ahora cerrado] por el 
                        close(0); para asignarle un nuevo input, que va a ser
                        p1p2[0] */
                      /* lée más de dup y dup2 en una de las primeras lecturas */
        /**cerrar la tubería p1p2[0] porque ya está siendo apuntada por el stdin
         * y cerrar la tubería p1p2[1] porque, COMO MEDIDA DE SEGURIDAD, SIEMPRE
         * SE DEBEN CERRAR LAS fd QUE NO SE ESTÉN USANDO!!!!!
        **/
        close(p1p2[0]);
        close(p1p2[1]);
        execlp(order3, order3, (char*) 0);
    }
    
}


int main(int argc, char* argv[]) {
    if(argc != 4){
        fprintf(stderr, "usage: %s order1 order2 order3\n", argv[0]);
        return -1;
    }
    
            /*se recibe en órden, pero internamente se executa al revés*/
    execute(argv[1], argv[2], argv[3], argv[0]);
    
    return 0;
}