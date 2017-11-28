#include <stdio.h>
#include <unistd.h>

extern char **environ;

int main(int argc, char* argv[]){
    char** var;
    var = environ;
    while (var != NULL){
        
    }
}