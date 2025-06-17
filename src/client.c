#include <stdio.h>
#include <stdint.h>

#include "net.h"

#define BUFF_SIZE 256

#define INDEX(ptr, init) (ptr-init)

extern void inputs_io(char *buffer, size_t size);

int main(void){
    int conn = connect_net("127.0.0.1:8070");
    if(conn < 0){
        fprintf(stderr, "%d\n", conn);
        return 1;
    }

    char buffer[BUFF_SIZE];
    inputs_io(buffer, BUFF_SIZE);

    send_net(conn, buffer, BUFF_SIZE);
    recv_net(conn, buffer, BUFF_SIZE);

    printf("%s\n", buffer);
    close_net(conn);

    return 0;
}

extern void inputs_io(char *buffer, size_t size){
    if(size == 0){
        return;
    }
    char *ptr = buffer;
    while((*ptr = getchar()) != '\n' && INDEX(ptr, buffer) < (size - 1)){
        ++ptr;
    }
    *ptr = '\0';
}