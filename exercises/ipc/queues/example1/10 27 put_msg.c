#include "10 27 header.h"

int main(int argc, char* argv[]) {
    struct message msg;
    int i, msqid, num_msg, length = sizeof(struct message) - sizeof(long);
                            /* del tamaño de la estructura se le resta el tamaño de un
                                long, porque como se debe poner primero un long para
                                determinar el tipo de mensaje que es, no se debe considerar
                                como parte de la longitud. 
                            */
    long msg_type;
    key_t key;
    
    if (argc != 1) {
        printf("usage: %s\n", argv[0]);
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
    
    srand(getpid());
	num_msg = rand() % 30 + 1;
	for (i = 0; i < num_msg; i++) {
		msg.type = (rand() % 4) + 1;
		msg.process = getpid();
		msg.value = (rand() % 100) + 1;
		msgsnd(msqid, &msg, length, 0);
		/* fila de mensajes a la cual mandar el mensaje, el apuntador al mensaje que
		    se va a mandar, la longitud y banderas adicionales */
		printf("MESSAGE POSTED: msg_type = %li process = %li value = %i\n", msg.type, msg.process, msg.value);
		//sleep(rand() % 5);
	}
	
	return 0;
}
