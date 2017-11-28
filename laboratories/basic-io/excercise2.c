#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//se necesutan ambas para el O_RDONLY y así
#include <fcntl.h>
#include <sys/types.h>
/* compile: gcc excercise2.c -o cifrado       */
/* execute: ./cifrado numero origen destino   */

#define SIZE 128 

//verifica que sea un entero positivo.
int isPosInt(char num[]){
  int i;
  for(i = 0; num[i] != 0; i++){
    if(!isdigit(num[i])){
      return 0;
    }
  }
  return 1;
}

int main(int argc, char* argv[]){
  int fd_in, fd_out, bytes, i, n;
  char buffer[sizeof(char)*SIZE];
  char aux [SIZE]; //creo que ambos sirven igual.
  /* verificar que sí esté dando el número de bytes correcto... :P
  char a = 'a';
  printf("sizeof 128*a = %i\n", (int)sizeof(a)*SIZE);
  printf("sizeof buffer = %i\n", (int)sizeof buffer);
  printf("sifeof int = %i\n", (int)sizeof(int));*/
  if (argc != 4){
    fprintf(stderr, "Forma de uso: %s numero origen destino", argv[0]);
    return -1;
  }
  /*Asegurar que funcione isPosInt
  char a[3] = {'1', '0', '3'};
  if(isPosInt(a)){
    printf("Si es un digito\n");
  } else{
    printf("No es un digito\n");
  }*/
  if(!isPosInt(argv[1])){
    printf("Error: el primer parámetro debe ser un numero entero positivo.\n");
    return -1;
  }
  n = atoi(argv[1]);
  if((fd_in = open(argv[2], O_RDONLY)) < 0 ){
    fprintf(stderr, "Error: El archivo %s no existe.\n", argv[2]);
    return -1;
  }
  if( (fd_out = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0 ){
    perror(argv[0]);
    return -1;
  }
  while( (bytes = read(fd_in, buffer, SIZE * sizeof(char))) != 0){
    if (bytes == SIZE) {
      for(i = 0; i < (bytes-1); i++){
        aux[i] = buffer[(i + n) % 128];
      }
      write(fd_out, aux, bytes);
    } else {
      write(fd_out, buffer, bytes);
    }
  }
  
  close(fd_in);
  close(fd_out);
  
  return 0;
}