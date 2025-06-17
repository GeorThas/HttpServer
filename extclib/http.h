#ifndef HEADERS_HTTP_H_
#define HEADERS_HTTP_H_

#include <stdint.h>

typedef struct HTTP HTTP;
typedef struct HTTPreq{
    char method[16]; //GET
    char path[2048]; //books
    char proto[16];  //HTTP/1.1
    uint8_t state;
    size_t index;
} HTTPreq;



extern HTTP *new_http(char *address);
extern void free_http(HTTP *http);

extern void handle_http(HTTP *http, char, void(*)(int, HTTPreq*));
extern int8_t listen_http(HTTP *http);

extern void parsehtml_http(int conn, char *filename);

#endif