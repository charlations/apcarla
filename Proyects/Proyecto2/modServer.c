#include "header.h"

void sendError(int nsfd, int errorType) {
  int msg_size;
  char reply[SIZE];
  
  switch (errorType) {
    case PermissionDenied:
      strcpy(reply, "Error: Permiso Denegado.");
    break;
    case FileNotFound:
      strcpy(reply, "Error: Archivo No Encontrado");
    break;
    case InternalError:
    default:
      strcpy(reply, "Error: Error Interno");
    break;
    case UnknownCommand:
      strcpy(reply, "Error: Ya ni sé qué onda");
    break;
    case RouteIsDir:
      strcpy(reply, "Error: Ruta lleva a un directorio");
    break;
    case DirNotFound:
      strcpy(reply, "Error: Directorio no encontrado");
    break;
    case RouteNotDir:
      strcpy(reply, "Error: Ruta no es un directorio");
    break;
  }
  msg_size = strlen(reply);
  write(nsfd, &errorType, sizeof(int));
  write(nsfd, &msg_size, sizeof(int));
  write(nsfd, &reply, msg_size * sizeof(char));
  
  //limpiando
  errorType = 0;
  strcpy(reply, "");
}

void execGet(int nsfd, char *rootDir) {
  int msg_size, code_sent;
  char addr[NAME_MAX + 1];
  char aux[NAME_MAX + 1];
  char reply[SIZE];
  struct stat ss;
  int fd_in;
  
  //lee tamaño
  read(nsfd, &msg_size, sizeof(int));
  read(nsfd, &addr, msg_size * sizeof(char));
  printf("addr original: %s\n", addr);
  if(msg_size > (NAME_MAX + 1)){
    sendError(nsfd, InternalError);
    exit(-1); //si no debiera matar mi conexión, usaría "return;"
  }
  if (addr[0] != '/' && (strstr(addr, "..") != NULL)){
    sendError(nsfd, PermissionDenied);
    exit(-1);
  }
  sprintf(aux, "%s%s", rootDir, addr);
  strcpy(addr, aux);
  printf("addr: %s\n", addr);
  if (lstat(addr, &ss) == -1) {
    sendError(nsfd, InternalError);
    exit(-1);
  }
  if(S_ISDIR(ss.st_mode)){
    sendError(nsfd, RouteIsDir);
    exit(-1);
  }
  if ( (fd_in = open(addr, O_RDONLY)) < 0 ) {
    sendError(nsfd, InternalError);
    exit(-1);
  }
  printf("%s - Paso todas las verificaciones\n", addr);
  code_sent = SendingFile;
  read(fd_in, &reply, SIZE * sizeof(char));
  close(fd_in);
  
  //Enviar mensaje
  msg_size = strlen(reply);
  write(nsfd, &code_sent, sizeof(code_sent));
  write(nsfd, &msg_size, sizeof(msg_size));
  write(nsfd, &reply, msg_size * sizeof(char));
  
  //limpiando
  code_sent = 0;
  strcpy(reply, "");
}

void execList(int nsfd, char *rootDir){
  int msg_size, code_sent;
  char addr[NAME_MAX + 1];
  char aux[NAME_MAX + 1];
  char reply[SIZE];
  struct stat ss;
  DIR *newDir;
  struct dirent *dir_entry;
  
  read(nsfd, &msg_size, sizeof(int));
  read(nsfd, &addr, msg_size * sizeof(char));
  
  printf("msg_size: %i\n", msg_size);
  printf("addr original: %s\n", addr);
  if(msg_size > (NAME_MAX + 1)){
    sendError(nsfd, InternalError);
    exit(-1); //si no debiera matar mi conexión, usaría "return;"
  }
  if (addr[0] != '/' && (strstr(addr, "..") != NULL)){
    sendError(nsfd, PermissionDenied);
    exit(-1);
  }
  sprintf(aux, "%s%s", rootDir, addr);
  strcpy(addr, aux);
  printf("addr: %s\n", addr);
  
  if (lstat(addr, &ss) == -1) {
    sendError(nsfd, InternalError);
    exit(-1); //si no debiera matar mi conexión, usaría "return;"
  }
  if(!S_ISDIR(ss.st_mode)){
    sendError(nsfd, RouteNotDir);
    exit(-1); //si no debiera matar mi conexión, usaría "return;"
  }
  if ( (newDir = opendir(addr)) == NULL ) {
    sendError(nsfd, PermissionDenied);
    exit(-1); //si no debiera matar mi conexión, usaría "return;"
  }
  printf("Sending LIST\n");
  sprintf(reply, "\t%s\n", addr);
  while( (strlen(reply) < SIZE) && (dir_entry = readdir(newDir)) != NULL ){
    sprintf(aux, "%s\n", dir_entry->d_name);
    strcat(reply, aux);
	}
	closedir(newDir);
	
  //Enviar mensaje
  msg_size = strlen(reply);
  write(nsfd, &code_sent, sizeof(code_sent));
  write(nsfd, &msg_size, sizeof(msg_size));
  write(nsfd, &reply, msg_size * sizeof(char));
  
  //limpiando
  code_sent = 0;
  strcpy(reply, "");
}


void serves_client(int nsfd, char *rootDir) {
  int code_sent, cmd_recieved, msg_size;
  //char addr[NAME_MAX + 1];
  char reply[SIZE];
  
  //Envía saludo :)
  printf("Enviando saludo\n");
  code_sent = 1;
  strcpy(reply, "Hola");
  msg_size = strlen(reply);
  write(nsfd, &code_sent, sizeof(code_sent));
  write(nsfd, &msg_size, sizeof(msg_size));
  write(nsfd, &reply, msg_size * sizeof(char));
  printf("Se envio el saludo :)\n");
  
  do {
    read(nsfd, &cmd_recieved, sizeof(int));
    printf("Received cmd %i\n", cmd_recieved);
    
    //switch de cmd
    switch(cmd_recieved) {
      case GET:
        printf("Received cmd LOG\n");
        execGet(nsfd, rootDir);
      break;
      case LIST:
        execList(nsfd, rootDir);
      break;
      case BYE:
        //do nothing
      break;
      default:
        sendError(nsfd, UnknownCommand);
      break;
    }
    cmd_recieved = UnknownCommand;
    //end do
  } while (cmd_recieved != BYE);
  
  close(nsfd);
}


void server(char* ip, int port, char* root, char* program){
  int sfd, nsfd, pid;
  struct sockaddr_in server_info, client_info;
  DIR *dir;
  //Crear root directory
  struct stat st = {0};
  
  printf("root = %s\tport = %i\tip = %s\n", root, port, ip);
  if (stat(root, &st) == -1) {
      mkdir(root, 0700);
  }
  //Entrar a root directory
  if ( (dir = opendir(root)) == NULL ) {
	perror(program);
	exit(-1);
	}
	closedir(dir);
    
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
       serves_client(nsfd, root);
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
	port = DEFAULT_PORT;
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