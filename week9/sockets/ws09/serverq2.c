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
    char buf[80], *p;

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

    size = sizeof q;
    if ((clientfd = accept(fd, (struct sockaddr *)&q, &size)) < 0) {
        perror("accept");
        return(1);
    }

    if ((len = read(clientfd, buf, sizeof buf - 1)) < 0) {
        perror("read");
        return(1);
    }
    buf[len] = '\0';

    printf("The other side said: \"%s\"\n", buf);

    if (fgets(buf, sizeof buf, stdin) == NULL)
	return(0);
    if ((p = strchr(buf, '\n')))
	*p = '\0';

    len = strlen(buf);
    if (write(clientfd, buf, len) != len) {
        perror("write");
        return(1);
    }

    /* This is the same, except there's nothing to unlink. */
    close(clientfd);

    /*
     * We didn't really have to do that since we're exiting.
     * But usually you'd be looping around and accepting more connections.
     */

    return(0);
}
