#include "..\extclib\http.h"
#include <string.h>
#include <stdio.h>

void index_page(int conn, HTTPreq *req);
void about_page(int conn, HTTPreq *req);

int main(void) {
	HTTP *serve = new_http("127.0.0.1:7545");

    //parsehtml_http(conn, "page404.html");

	handle_http(serve, "/", index_page);
	handle_http(serve, "/about/", about_page);

	listen_http(serve);
	free_http(serve);
	return 0;
}

void index_page(int conn, HTTPreq *req) {
    //parsehtml_http(conn, "page404.html");
	if(strcmp(req->path, "/") != 0) {
        printf("error\n");
		parsehtml_http(conn, "page404.html");
		return;
	}
	parsehtml_http(conn, "index.html");
}

void about_page(int conn, HTTPreq *req) {
	if(strcmp(req->path, "/about/") != 0) {
        printf("error\n");
		parsehtml_http(conn, "page404.html");
		return;
	}
	parsehtml_http(conn, "about.html");
}