#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int fd, len;
    char buf[80], *p;
    struct sockaddr_in r;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return(1);
    }

    memset(&r, '\0', sizeof r);
    r.sin_family = AF_INET;
    r.sin_addr.s_addr = htonl((127 << 24) | 1);
    r.sin_port = htons(1234);

    if (connect(fd, (struct sockaddr *)&r, sizeof r) < 0) {
        perror("connect");
        return(1);
    }

    if (fgets(buf, sizeof buf, stdin) == NULL)
	return(0);
    if ((p = strchr(buf, '\n')))
	*p = '\0';

    len = strlen(buf);
    if (write(fd, buf, len) != len) {
        perror("write");
        return(1);
    }

    return(0);
}
