#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

int main(int argc, char **argv) {
    int r, opt;
    int status = 0;
    FILE *fp;
    extern void process(FILE *fp, int r);

    while ((opt = getopt(argc, argv, "r:")) != EOF) {
    	switch (opt) {
    		case 'r':
    			r = atoi(optarg);
    			break;
    		default:
    			status = 1;
    			break;
    	}
    }

    if (status) {
    	fprintf(stderr, "usage: %s [-r count] [file ...]\n", argv[0]);
    	return status;
    }

    if (optind == argc) {
    	process(stdin, r);
    } else {
    	for (; optind < argc; optind++) {
    		if (strcmp(argv[optind], "-") == 0) {
				process(stdin, r);
			} else if ((fp = fopen(argv[optind], "r")) == NULL) {
				perror(argv[optind]);
				status = 1;
			} else {
				process(fp, r);
			}
    	}


    }
    return status;
}

void process(FILE *fp, int r) {
	int c;

	while ((c = getc(fp)) != EOF) {
	if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
	    putchar(c + r);
	else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
	    putchar(c - r);
	else
	    putchar(c);
    }
}