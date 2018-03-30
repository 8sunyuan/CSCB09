/*
 * Listen on a port, accept a connection, execute (argv+1)
 * i.e. command line looks like:
 *	listener [-p port] cmd [arg1 ...]
 * Example:
 *	listener /bin/echo Hello, world
 * or
 *	listener -p 1234 /bin/echo Hello, world
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/signal.h>

int port = 3098;

extern void exitbad();


int main(int argc, char **argv)
{
    int c, status = 0;
    socklen_t len;
    int listenfd, clientfd;
    struct sockaddr_in r;
    extern char **environ;

    while ((c = getopt(argc, argv, "p:")) != EOF) {
	if (c == 'p') {
	    if ((port = atoi(optarg)) == 0) {
		fprintf(stderr, "%s: non-numeric port \"number\"\n", argv[0]);
		exitbad();
	    }
	} else {
	    status = 1;
	}
    }

    if (status || argc == optind) {
	fprintf(stderr, "usage: %s [-p port]\n", argv[0]);
	exitbad();
    }

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("socket");
	exitbad();
    }
    c = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &c, sizeof c)) {
	perror("setsockopt");
	exitbad();
    }

    r.sin_family = AF_INET;
    r.sin_addr.s_addr = INADDR_ANY;
    r.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&r, sizeof r)) {
	perror("bind");
	exitbad();
    }

    if (listen(listenfd, 5)) {
	perror("listen");
	exitbad();
    }

    len = sizeof r;
    if ((clientfd = accept(listenfd, (struct sockaddr *)&r, &len)) < 0) {
	perror("accept");
	exitbad();
    }

    /* redirect all standard file descriptors to client */
    dup2(clientfd, 0);
    dup2(clientfd, 1);
    dup2(clientfd, 2);
    close(clientfd);
    close(listenfd);

    (void)execve(argv[optind], argv + optind, environ);
    perror(argv[optind]);
    exitbad();
    return(0);  /* not reached, but shut up gcc -Wall */
}


void exitbad()
{
    /*
     * Since this will be being invoked from an init-like program, and the
     * assignment three specification does not ask the student to implement
     * init's re-spawn delay in the event of an apparently-misbehaving
     * process, we put in a short delay before exiting in error.
     */
    fflush(stdout);
    fflush(stderr);
    sleep(4);
    exit(1);
}
