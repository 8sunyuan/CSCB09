#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char **argv)
{
    int fd, i, port;
    struct hostent *hp;
    struct sockaddr_in r;
    static char name[] = "abcdefghijklmnopqrstuvwxyz";

    if (argc != 3 || (port = atoi(argv[2])) <= 0) {
	fprintf(stderr, "usage: trickyclient hostname portnumber\n");
	return(1);
    }

    if ((hp = gethostbyname(argv[1])) == NULL) {
	fprintf(stderr, "%s: no such host\n", argv[1]);
	exit(1);
    }
    if (hp->h_addr_list[0] == NULL || hp->h_addrtype != AF_INET) {
	fprintf(stderr, "%s: not an internet protocol host name\n", argv[1]);
	exit(1);
    }

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("socket");
	return(1);
    }
    memset(&r, '\0', sizeof r);
    r.sin_family = AF_INET;
    memcpy(&r.sin_addr, hp->h_addr_list[0], hp->h_length);
    r.sin_port = htons(port);
    if (connect(fd, (struct sockaddr *)&r, sizeof r) < 0) {
	perror("connect");
	return(1);
    }

    for (i = 0; i < 5; i++) {
	sleep(1);
	write(fd, name + i * 5, 5);
	printf("sent %.5s (but no newline, so the server shouldn't react yet)\n",
		name + i * 5);
	fflush(stdout);
    }

    printf("[slowing down now]\n");
    fflush(stdout);
    sleep(3);

    write(fd, "z\r\n", 3);
    printf("Now sent 'z' and the newline, so it should finally acknowledge the name.\n");
    printf("The name is all 26 letters, abcdefghijklmnopqrstuvwxyz.\n");
    fflush(stdout);

    sleep(4);
    printf("Now exiting without reading the data from the server.  If your server gets\n");
    printf("upset at this, that might still be ok for CSC B09 assignment purposes.\n");
    fflush(stdout);
    return(0);
}
