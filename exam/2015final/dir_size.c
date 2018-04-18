#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <getopt.h>

int main(int argc, char **argv) {
    DIR *dir;
    struct dirent *p;
    int size, num = 0;
    if (argc != 2) {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return 1;
    }

    if ((chdir(argv[1], "r")) != 0) {
        // Couldn't change directory
        perror(argv[1]);
        return 1;
    }

    // Read file
    while ((p = readdir(dir)))
        // Add size of file
        num+= p->d_reclen;

    close(dir);
    return 0;
}
