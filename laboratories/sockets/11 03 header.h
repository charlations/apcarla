#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h> /* canal de comunicacion */
#include <sys/socket.h> /* tcp/ip */
#include <netinet/in.h> /* protocolos de comunicacion basados en red, arq de red */

#define MISSED -1
#define HIT 1

#define CONTINUE 2
#define END -2
#define ANSWER 3

#endif