#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>



int main() {
    int server_socket, clientfd, len;
    socklen_t size;
    struct sockaddr_in r, q;
    char buf[100];

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&r, '\0', sizeof r);
    r.sin_family = AF_INET;
    r.sin_addr.s_addr = INADDR_ANY;
    r.sin_port = htons(2000);

    if (bind(server_socket, (struct sockaddr *)&r, sizeof r) < 0) {
        perror("bind");
        return(1);
    }
    if (listen(server_socket, 5)) {
    	perror("listen");
    	return(1);
    }


    while (1) {
        size = sizeof(q);
        if ((clientfd = accept(server_socket, (struct sockaddr *)&q, &size)) < 0) {
            perror("connect");
            return(1);
        }

        while ((len = read(clientfd, buf, sizeof(buf) - 1)) > 0) {
            buf[len] = '\0';
            printf("%s\n", buf);
        }
        close(clientfd);

    }

    return 1;
}
