#include "header.h"

/* gcc server.c -o server */
/* ./server & */
void serves_client(int nsfd, DIR* root){
    int code_sent, cmd_recieved, msg_size;
    char addr[NAME_MAX + 1];
    char* reply;
    int error;
    struct stat ss;
    DIR *newDir;
    struct dirent *dir_entry;
    int fd_in, bytes;
    reply = (char*) malloc((SIZE) * sizeof(char) );
    //Envío de saludo
    code_sent = 1;
    reply = "Hola\n";
    msg_size = strlen(reply);
    write(nsfd, &msg_size, sizeof(msg_size));
    write(nsfd, &code_sent, sizeof(code_sent));
    //write(nsfd, &reply, sizeof(reply));
    //PUEDE QUE SEA:
    write(nsfd, &reply, msg_size * sizeof(char));
    do {
        error = 0;
        //recibe tamaño
        read(nsfd, &msg_size, sizeof(msg_size));
        if(msg_size > (NAME_MAX + 1)){
            msg_size = NAME_MAX + 1;
            code_sent = InternalError;
            reply = "Error: tamaño demasiado grande\n";
            error = 1;
        }
        //recibe instrucción
        read(nsfd, &cmd_recieved, sizeof(cmd_recieved));
        //recibe info adicional
        read(nsfd, &addr, msg_size * sizeof(char));
        if (addr[0] != '/' || (strstr(addr, "..") != NULL)){
            code_sent = InternalError;
            reply = "Error: la dirección no se puede salir de directorio raiz\n";
            error = 1;
        }
        if( !error ){
            switch(cmd_recieved){
                case GET:
                    if (lstat(addr, &ss) == -1) {
                		code_sent = InternalError;
                        reply = "Error interno.\n";
                		break;
                	}
                	if(S_ISDIR(ss.st_mode)){
                	    code_sent = RouteIsDir;
                	    reply = "Error: dirección dada es de un directorio\n";
                	    break;
                	}
                	if ( (fd_in = open(addr, O_RDONLY)) < 0 ) {
                	    code_sent = PermissionDenied;
                	    reply = "No se pudo abrir el archivo.\n";
                	    break;
                	}
                	code_sent = SendingFile;
                	bytes = read(fd_in, &reply, SIZE * sizeof(char));
                	close(fd_in);
                break;
                case LIST:
                    if (lstat(addr, &ss) == -1) {
                		code_sent = InternalError;
                        reply = "Error interno.\n";
                		break;
                	}
                	if(!S_ISDIR(ss.st_mode)){
                	    code_sent = RouteNotDir;
                	    reply = "Error: dirección dada no es de un directorio\n";
                	    break;
                	}
                	if ( (newDir = opendir(addr)) == NULL ) {
                	    code_sent = PermissionDenied;
                	    reply = "No se pudo abrir el directorio.\n";
                	    break;
                	}
                	code_sent = SendingDir;
                	while( (dir_entry = readdir(newDir)) != NULL ){
                	    sprintf(reply, "%s\n", dir_entry->d_name);
                	}
                break;
                case BYE:
                    //do nothing
                break;
                default:
                    code_sent = UnknownCommand;
                    reply = "Error: commando desconocido.\n";
                break;
            }
        }
        msg_size = strlen(reply);
        write(nsfd, &msg_size, sizeof(msg_size));
        write(nsfd, &code_sent, sizeof(code_sent));
        write(nsfd, &reply, sizeof(reply));
    } while(cmd_recieved != BYE);
    close(nsfd);
}

void server(char* ip, int port, char* root, char* program){
    int sfd, nsfd, pid;
    struct sockaddr_in server_info, client_info;
    DIR *dir;
    //Crear root directory
    struct stat st = {0};
    if (stat(root, &st) == -1) {
        mkdir(root, 0700);
    }
    //Entrar a root directory
    if ( (dir = opendir(root)) == NULL ) {
		perror(program);
		exit(-1);
	}
    
    if ( (sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror(program);
        exit(-1);
    }
    server_info.sin_family = AF_INET; 
    server_info.sin_addr.s_addr = inet_addr(ip);
    server_info.sin_port = htons(port);
    
    if ( bind(sfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0 ) {
        perror(program);
        exit(-1);
    }
    listen(sfd, 1);
    while(1){
        int len = sizeof(client_info);
        if ( (nsfd = accept(sfd, (struct sockaddr *) &client_info, &len)) < 0 ){
            perror(program);
            exit(-1);
        }
        
        if ( (pid = fork()) < 0 ){
            perror(program);
        } else if (pid == 0) {
           close(sfd); //el proceso hijo no requiere escuchar, por eso cierra el canal de escucha
           serves_client(nsfd, dir);
           exit(0);
        } else {
           close(nsfd);
        }
        
    }
}

int main(int argc, char* argv[]){
    char ip[15];
    int port;
    char* root = "prueba";
    
    strcpy(ip, DEFAULT_IP);
	port = DEFAULT_PORT;
	if (argc == 2){
	    root = argv[1];
	}
	if(argc > 2){
	    printf("Usage: %s [rootDir]", argv[0]);
	    return -1;
	}
	
	
	server(ip, port, root, argv[0]);
	
	return 0;
}