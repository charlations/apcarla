#include "header.h"

/* gcc server.c -o server */
/* ./server & */

void serves_client(int nsfd, char* rootDir, DIR* root){
    int code_sent, cmd_recieved, msg_size;
    char addr[NAME_MAX + 1];
    char reply[SIZE];
    int error;
    struct stat ss;
    DIR *newDir;
    struct dirent *dir_entry;
    int fd_in, bytes;
    //reply = (char*) malloc((SIZE) * sizeof(char) );
    //Envío de saludo
    code_sent = 1;
    strcpy(reply, "Hola");
    msg_size = strlen(reply);
    write(nsfd, &msg_size, sizeof(msg_size));
    write(nsfd, &code_sent, sizeof(code_sent));
    write(nsfd, &reply, msg_size * sizeof(char));
    do {
        error = 0;
        //recibe tamaño
        read(nsfd, &msg_size, sizeof(msg_size));
        printf("Servidor recibió el msg_size\n");
        printf("Servidor - msg_size: %i\n", msg_size);
        if(msg_size > (NAME_MAX + 1)){
            msg_size = NAME_MAX + 1;
            code_sent = InternalError;
            strcpy(reply, "Error: tamaño demasiado grande");
            error = 1;
        }
        //recibe instrucción
        read(nsfd, &cmd_recieved, sizeof(cmd_recieved));
        printf("Servidor recibió el cmd_recieved\n");
        printf("Servidor - cmd_recieved: %i\n", cmd_recieved);
        //recibe info adicional
        read(nsfd, &addr, msg_size * sizeof(char));
        printf("Servidor recibió el addr\n");
        printf("Servidor - addr: %s\n", addr);
        if (addr[0] != '/' && (strstr(addr, "..") != NULL)){
            code_sent = InternalError;
            strcpy(reply, "Error: la dirección no se puede salir de directorio raiz");
            error = 1;
        }
        printf("Server - error: %i\n", error);
        if( error == 0 ){
            sprintf(addr, "%s%s", rootDir, addr);
            printf("Server - NEW addr: %s\n", addr);
            switch(cmd_recieved){
                case GET:
                    if (lstat(addr, &ss) == -1) {
                		code_sent = InternalError;
                        strcpy(reply, "Error interno.");
                		break;
                	}
                	if(S_ISDIR(ss.st_mode)){
                	    code_sent = RouteIsDir;
                	    strcpy(reply, "Error: dirección dada es de un directorio");
                	    break;
                	}
                	if ( (fd_in = open(addr, O_RDONLY)) < 0 ) {
                	    code_sent = PermissionDenied;
                	    strcpy(reply, "No se pudo abrir el archivo.");
                	    break;
                	}
                	code_sent = SendingFile;
                	bytes = read(fd_in, &reply, SIZE * sizeof(char));
                	close(fd_in);
                break;
                case LIST:
                    if (lstat(addr, &ss) == -1) {
                		code_sent = InternalError;
                        strcpy(reply, "Error interno.");
                		break;
                	}
                	if(!S_ISDIR(ss.st_mode)){
                	    code_sent = RouteNotDir;
                	    strcpy(reply, "Error: dirección dada no es de un directorio");
                	    break;
                	}
                	if ( (newDir = opendir(addr)) == NULL ) {
                	    code_sent = PermissionDenied;
                	    strcpy(reply, "No se pudo abrir el directorio.");
                	    break;
                	}
                	code_sent = SendingDir;
                	sprintf(reply, "\t%s\n", addr);
                	while( (strlen(reply) < SIZE) && (dir_entry = readdir(newDir)) != NULL ){
                	    //usar addr como auxiliar
                	    sprintf(addr, "%s\n", dir_entry->d_name);
                	    strcat(reply, addr);
                	}
                	closedir(newDir);
                break;
                case BYE:
                    //do nothing
                break;
                default:
                    code_sent = UnknownCommand;
                    strcpy(reply, "Error: commando desconocido.");
                break;
            }
        }
        msg_size = strlen(reply);
        write(nsfd, &msg_size, sizeof(msg_size));
        write(nsfd, &code_sent, sizeof(code_sent));
        write(nsfd, &reply, msg_size * sizeof(char));
    } while(cmd_recieved != BYE);
    close(nsfd);
}

void server(char* ip, int port, char* root, char* program){
    int sfd, nsfd, pid;
    struct sockaddr_in server_info, client_info;
    DIR *dir;
    //Crear root directory
    struct stat st = {0};
    
    printf("root = %s\n", root);
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
           serves_client(nsfd, root, dir);
           exit(0);
        } else {
           close(nsfd);
        }
        
    }
}

int main(int argc, char* argv[]){
    char ip[15];
    int port;
    char root[NAME_MAX];
    
    strcpy(ip, DEFAULT_IP);
    strcpy(root,  "./prueba");
	port = 11000;
	if (argc == 2){
	    strcpy(root, argv[1]);
	}
	if(argc > 2){
	    printf("Usage: %s [rootDir]", argv[0]);
	    return -1;
	}
	
	
	server(ip, port, root, argv[0]);
	
	return 0;
}