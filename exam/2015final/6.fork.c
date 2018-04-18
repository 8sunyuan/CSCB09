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
    char input[50];
    sprintf(input, "<%s", argv[1]);
    char output[50];
    sprintf(output, ">%s", argv[2]);
    printf("%s %s\n", input, output);
    result = fork();
    if (result == -1) {
        perror("fork");
        return 1;
    } else if (result == 0) {   // Child process
        execl("/usr/bin/tr", "tr", "e", "f", input, output, (char *)NULL);
        perror("/usr/bin/tr");
        return 1;
    } else if (result > 0) {    // Parent process
        wait(&status);
        printf("child process exit status was %d\n", status);
    }
    return 0;
}
