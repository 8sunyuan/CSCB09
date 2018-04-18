#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    DIR *cwd;
    struct dirent *p;
    int num = 0;
    if (argc != 2) {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return 1;
    }

    if (chdir(argv[1]) != 0) {
        // Couldn't change directory
        perror(argv[1]);
        return 1;
    }

    cwd = opendir(".");

    // Read file
    while ((p = readdir(cwd)))
        // Add size of file
        if (strcmp(p->d_name, ".") != 0 && strcmp(p->d_name, "..") != 0)
            num += p->d_reclen;
    printf("%d\n", num);
    return 0;
}
