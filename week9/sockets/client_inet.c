/*
 * socket demonstrations:
 * This is the client side of an "internet domain" socket connection, for
 * communicating over the network.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int fd;
    struct sockaddr_in r;

    /* this is the same as before, except it says "INET" instead of "UNIX" */
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return(1);
    }

    /* This is slightly different: specify IP address and port number. */
    memset(&r, '\0', sizeof r);
    r.sin_family = AF_INET;
    r.sin_addr.s_addr = htonl((127 << 24) | 1);
    r.sin_port = htons(1234);
    /*
     * That address is 127.0.0.1 -- take the 127 and shift it to the left 24
     * bits so as to put it in the upper octet.  More commonly we would look
     * up a hostname with gethostbyname().
     */

    /* this is the same as before */
    if (connect(fd, (struct sockaddr *)&r, sizeof r) < 0) {
        perror("connect");
        return(1);
    }
    /*
     * this is the same as before too.  All that's different is the
     * connection setup.  This is why sockets are good.
     */
    if (write(fd, "Hello", 5) != 5) {
        perror("write");
        return(1);
    }

    return(0);
}
