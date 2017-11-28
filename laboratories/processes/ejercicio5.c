#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

/* gcc ejercicio4.c -o descendiendo */
/* ./descendiendo num */

void child_process(int n, int total){
  int i, pid, j;
  
  if(n <= total){
    if( (pid = fork()) < 0 ){
      exit(-1);
    } else if (pid == 0){
      n++;
      for(j = 0; j < n; j++){
        child_process(n, total);
      }
    } else{
      for(i = 0; i < n; i++){
        printf("\t");
      }
      printf("PPID = %i\tPID = %i\tNIVEL = %i\n", getppid(), getpid(), n);
      sleep(2);
      exit(0);
    }
  }
}

int main(int argc, char* argv[]){
  if (argc != 2){
    fprintf(stderr, "forma de uso: %s num\n", argv[0]);
    return -1;
  }
  int total;
  total = atoi(argv[1]);
  if(total <= 0) {
    fprintf(stderr, "%s: el programa recibe un numero entero posiitivo mayor a 0.", argv[0]);
    return -1;
  }
  
  child_process(0, total);
  
  return 0;
}