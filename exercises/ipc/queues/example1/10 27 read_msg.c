#include "10 27 header.h"

int main(int argc, char* argv[]) {
    struct message msg;
    int msqid, length = sizeof(struct message) - sizeof(long);
    long msg_type;
    key_t key;
    
    if (argc != 2) {
        printf("usage: %s msg_type\n", argv[0]);
        return -1;
    }
 
    msg_type = atol(argv[1]);
    if (msg_type < 0 || msg_type > 4) {
        printf("%s: the msg_type must be a between 0 and 4.\n", argv[0]);
        return -1;
    }
    
    if ( (key = ftok("/dev/null", 24)) == (key_t) -1 ) {
        perror(argv[0]);
        return -1;
    }
    
    if ( (msqid = msgget(key, 0666)) == -1 ) {
        perror(argv[0]);
        return -1;
    }
    
    while (msgrcv(msqid, &msg, length, msg_type, IPC_NOWAIT) != -1) {
        /* MeSsaGe ReCieVe: llamada bloqueante - si se pide leer las llamadas de un tipo de
            mensaje qu no existe, se bloquea hasta que las reciba. Poner la bandera
            IPC_NOWAIT dice que mejor mande un mensaje de error y no se quede esperando. */
		printf("MESSAGE READ: msg_type = %li process = %li value = %i\n", msg.type, msg.process, msg.value);
	}
	
	return 0;
}
