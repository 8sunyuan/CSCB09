/*
 * socket demonstrations:
 * This is the server side of an "internet domain" socket connection, for 
 * communicating over the network.
 *
 * In this case we are willing to wait either for chatter from the client
 * _or_ for a new connection.  (In a more realistic example we would be
 * processing requests in a loop.)
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

int main()
{
    int fd, clientfd, maxfd;
    socklen_t len;
    struct sockaddr_in r, q;
    struct timeval tv;
    fd_set fds;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return(1);
    }
    memset(&r, '\0', sizeof r);
    r.sin_family = AF_INET;
    r.sin_addr.s_addr = INADDR_ANY;
    r.sin_port = htons(1234);

    if (bind(fd, (struct sockaddr *)&r, sizeof r) < 0) {
        perror("bind");
        return(1);
    }
    if (listen(fd, 5)) {
	perror("listen");
	return(1);
    }
    len = sizeof q;
    if ((clientfd = accept(fd, (struct sockaddr *)&q, &len)) < 0) {
        perror("accept");
        return(1);
    }

    /*
     * ok, now we have a connection.
     * Do we do a read?  Well, not if someone else might connect before
     * our new client says anything!  To do this right, you need to use
     * select().
     */

    tv.tv_sec = 5;  /* timeout in seconds (you'll use a NULL timeval pointer
                     * for your assignment, because you want to wait until
                     * there's activity, however long that is) */
    tv.tv_usec = 0;  /* and microseconds */
    /* "fds" shows which file descriptors we're interested in */
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    maxfd = fd;
    FD_SET(clientfd, &fds);
    if (clientfd > maxfd)
	maxfd = clientfd;
    switch (select(maxfd + 1, &fds, NULL, NULL, &tv)) {
    case 0:
        printf("timeout happened\n");
        break;
    case -1:
        perror("select");
        break;
    default:
        if (FD_ISSET(fd, &fds))
            printf("a new client is connecting\n");  /* so we should accept(), etc */
        else if (FD_ISSET(clientfd, &fds))
            printf("the old client is chattering\n");  /* so we should read(), etc */
        else
            printf("won't happen\n");
    }

    return(0);
}
