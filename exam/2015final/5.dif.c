#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s file1 file2\n", argv[0]);
        return 1;
    }

    FILE *f1, *f2;
    // Error check opening files
    if ((f1 = fopen(argv[1], "r")) == NULL) {
        perror(argv[1]);
        return 1;
    } else if ((f2 = fopen(argv[2], "r")) == NULL) {
        perror(argv[2]);
        return 1;
    }

    char line1[1001];
    char line2[1001];
    char dif[2050];

    if (fgets(line1, 1001, f1) == NULL) {
        perror("fgets");
        return 1;
    }
    if (fgets(line2, 1001, f2) == NULL) {
        perror("fgets");
        return 1;
    }

    if (strcmp(line1, line2) == 0) {
        return 0;
    } else {
        if (line1[0] == '\0')
            sprintf(dif, "0a1\n> %s\n", line2);
        else if (line2[0] == '\0')
            sprintf(dif, "1d0\n< %s\n", line1);
        else
            sprintf(dif, "1c1\n< %s\n---\n> %s\n", line1, line2);
    }
    printf("%s", dif);
    return 0;
}
