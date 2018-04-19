#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include<unistd.h>


int main(int argc, char ** argv) {
    int result, status;

    if (argc != 3) {
        fprintf(stderr, "usage: %s file1 file2\n", argv[0]);
        return 1;
    }

    result = fork();
    if (result == -1) {
        perror("fork");
        return 1;
    } else if (result == 0) {   // Child process
        execl("/usr/bin/tr", "tr", "e", "f", "<", argv[1], ">", argv[2], (char *)NULL);
        perror("/usr/bin/tr");
        return 1;
    } else if (result > 0) {    // Parent process
        wait(&status);
        printf("child process exit status was %d\n", status);
    }
    return 0;
}
