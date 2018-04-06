/*
 * socket demonstrations:
 * This is the server side of an "internet domain" socket connection, for 
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
    int fd, clientfd;
    int len;
    socklen_t size;
    struct sockaddr_in r, q;
    char buf[80];

    /* this is the same as before, except it says "INET" instead of "UNIX" */
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return(1);
    }

    /* This is slightly different: specify port number. */
    memset(&r, '\0', sizeof r);
    r.sin_family = AF_INET;
    r.sin_addr.s_addr = INADDR_ANY;
    r.sin_port = htons(1234);

    /* this is the same */
    if (bind(fd, (struct sockaddr *)&r, sizeof r) < 0) {
        perror("bind");
        return(1);
    }
    if (listen(fd, 5)) {
	perror("listen");
	return(1);
    }

    /* this is the same */
    size = sizeof q;
    if ((clientfd = accept(fd, (struct sockaddr *)&q, &size)) < 0) {
        perror("accept");
        return(1);
    }

    /* And this is the same too. */
    if ((len = read(clientfd, buf, sizeof buf - 1)) < 0) {
        perror("read");
        return(1);
    }
    buf[len] = '\0';
    /*
     * Here we should be converting from the network newline convention to the
     * unix newline convention, if the string can contain newlines.
     */
    printf("The other side said: \"%s\"\n", buf);

    /* This is the same, except there's nothing to unlink. */
    close(clientfd);

    /*
     * We didn't really have to do that since we're exiting.
     * But usually you'd be looping around and accepting more connections.
     */

    return(0);
}
