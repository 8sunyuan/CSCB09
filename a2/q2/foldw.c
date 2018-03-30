#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>


#define MAX 3000

int main(int argc, char **argv) {
	int width = 80;
	int opt, status = 0, punc = 0;
	FILE *fp;
	extern void process(FILE *fp, int width, int punc);
	while ((opt = getopt(argc, argv, "w:p")) != EOF) {
		switch (opt) {
			case 'w':
				width = atoi(optarg);
				break;
			case 'p':
				punc = 1;
				break;
			default:
				status = 1;
				break;
		}
	}

	if (status) {
		fprintf(stderr, "usage: %s [-w count] [-p] [file ...]\n", argv[0]);
		return status;
	}

	if (optind == argc) { //argument count only has flags and filename
		process(stdin, width, punc);
	} else { 			  //argument count has files and/or "-"
		for (; optind < argc; optind++) {
			if (strcmp(argv[optind], "-") == 0) {
				process(stdin, width, punc);
			}
			else if ((fp = fopen(argv[optind], "r")) == NULL) {
				perror(argv[optind]);
				status = 1;
			} else {
				process(fp, width, punc);
			}
		}
	}
	return status;
}

void process(FILE *fp, int width, int punc) {
	int linebreak = -1;
	int i = 0;
	char c;
	char schar[2] = {'1', '\0'} ;
	char *line, *leftover;
	line = malloc((width+1) * sizeof(char));
	leftover = malloc((width+1) * sizeof(char));

	while((c = getc(fp)) != EOF) {
		schar[0] = c;
		if (i < width) { // Within length of line
			strcat(line, schar);
			if (c == ' ') linebreak = i;
			else if (punc && ispunct(c)) linebreak = i;
		} else { // Reached max length of line
			if (linebreak >= 0){ // There is a linebreak we can stop the line at
				// Save characters from linebreak to end of line and save that leftover
				// for the next line
				strncpy(leftover, line + linebreak + 1, width);
				strcat(leftover, schar);
				// If linebreak is the comma, don't want to index past it
				if (ispunct(line[linebreak])) line[linebreak + 1] = '\0';
				else line[linebreak] = '\0';
				printf("%s\n", line);
				// Start new line with leftover chars
				strcpy(line, "");
				strcpy(line, leftover);
				strcpy(leftover, "");
				i = strlen(line) - 1;
				linebreak = -1;
			} else { // Nowhere to break, print entire line
				// strcat(line, schar)
				printf("%s\n", line);
				line[0] = c;
				if (c == ' ') {
					line[0] = '\0';
					linebreak = -1;
					i = -1;
				} else {
					line[0] = c;
					line[1] = '\0';
					if (punc && ispunct(c)) linebreak = 0;
					i = 0;
				}
			}
		}
		i++;
	}
	printf("%s", line);
}
