#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h> /* canal de comunicacion */
#include <sys/socket.h> /* tcp/ip */
#include <netinet/in.h> /* protocolos de comunicacion basados en red, arq de red */
#include <string.h>

//códigos de peticiones
#define GET 101
#define LIST 102
#define BYE 103
//códigos de error
#define PermissionDenied 201
#define FileNotFound 202
#define InternalError 203
#define UnknownCommand 204
#define RouteIsDir 205
#define DirNotFound 206
#define RouteNotDir 207
//códigos de success
#define SendingFile 301
#define SendingDir 302

#define DEFAULT_PORT 9997
#define DEFAULT_IP "127.0.0.1"
#define SIZE 1000

#endif