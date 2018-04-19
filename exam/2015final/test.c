#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include<unistd.h>

char command[1000] = "";

extern void parseargs(int argc, char **argv);

int main(int argc, char ** argv) {

    int result, status;
    parseargs(argc, argv);

    result = fork();
    if (result == -1) {
        perror("fork");
        return 1;
    } else if (result == 0) {   // Child process
        printf("%s\n", command);
        execl("/bin/ls", "echo", command, (char *)NULL);
        perror("/usr/bin/ls");
        return 1;
    } else if (result > 0) {    // Parent process
        wait(&status);
        printf("child process exit status was %d\n", status);
    }

    return 0;

}

void parseargs(int argc, char **argv)
{
    int c, i = 1;
    while ((c = getopt(argc, argv, "p:")) != EOF) {
            printf("%s\n", argv[i]);
    	    strcat(command, " ");
            strcat(command, argv[i]);
    }
}
