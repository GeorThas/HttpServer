#include "extclib/http.h"
#include <string.h>

void index_page(int conn, HTTPreq *request);
void about_page(int conn, HTTPreq *request);

void main(void){
    HTTP *serve = new_http("127.0.0.1:7545");

    handle_http(serve, "/", index_page);
    handle_http(serve, "/about", about_page);

    listen_http(serve);
    free_http(serve);
    return 0;
}

void index_page(int conn, HTTPreq *request){
    if(strcmp(request->path, "/") != 0){
        parsehtml_http(conn, "page404.html");
        return;
    }
    parsehtml_http(conn, "index.html");
}

void about_page(int conn, HTTPreq *request){
    if(strcmp(request->path, "/about") != 0){
        parsehtml_http(conn, "page404.html");
        return;
    }
    parsehtml_http(conn, "about.html");
}