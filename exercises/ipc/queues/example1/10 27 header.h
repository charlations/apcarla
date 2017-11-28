#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

struct message {
    long type;
    long process;
    int value;
};

#endif
/* una estructura de tipo mensaje puede ser del tipo que se uno quiera.
    la única limitante que hay es que el primer atributo de un mensaje debe
    ser de tipo long para representar el tipo de mensaje que es.
    Ese tipo de mensaje no es que si es tipo char, int, etc, es una etiqueta
    de uso del mensaje (para que se puedan filtrar y demás)!!!
*/