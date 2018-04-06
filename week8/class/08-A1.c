#include <stdio.h>
#include <unistd.h>

int main()
{
    execl("/bin/ls", "ls", "file", (char *)NULL);
    perror("/bin/ls");
    return(1);
}
