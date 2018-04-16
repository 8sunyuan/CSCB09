#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

void cat(char **str1, char *str2) {
    char *temp;
    int len1 = strlen(*str1);
    int len2 = strlen(str2);
    temp = malloc(len1 + 1);
    memcpy(temp, *str1, len1);
    *str1 = malloc(len1 + len2 + 1);
    sprintf(*str1, "%s%s", temp, str2);
    free(temp);
}

int main() {
    char *name;
    name = malloc(100);
    strcpy(name, "initial");

    char buf[100];
    read(STDIN_FILENO, buf, 99);
    cat(&name, buf);
    
    printf("NAME: %s\n", name);
    return 0;
}
