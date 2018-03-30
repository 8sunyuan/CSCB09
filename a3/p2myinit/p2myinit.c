#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv) {
	int c, stat, status, respawn, result, i, cont;
	int result2;
	stat = 0;
	respawn = 0;
	char *runlevel, *line, *first, *second, *third, *p;
	FILE *fp;
	extern int is_empty(const char *s);


	while ((c = getopt(argc, argv, "r:")) != EOF) {
		switch (c) {
			case 'r':
				runlevel = optarg;
				break;
			default:
				stat = 1;
				break;
		}
	}

	if (optind + 1 != argc)	stat = 1;

	// Error checking
	if (stat) {
		fprintf(stderr, "usage: %s [-r runlevel] file\n", argv[0]);
		return stat;
	}

	if (argc == 2) runlevel = "3"; // No -r flags, default to 3
	// ./myinit -r123456 file1 is invalid
	if (runlevel[1] != '\0') {
		fprintf(stderr, "%s: runlevel must be a single character\n", argv[0]);
		return 1;
	}

	if ((fp = fopen(argv[optind], "r")) == NULL) { // Open innitab file
		perror(argv[optind]);
		return 1;
	}

	line = malloc(1001*sizeof(char));

	while((fgets(line, 1001, fp)) != NULL) {
		cont = 0;
		respawn = 0;
		// Remove comments from line
		if ((p = strchr(line, '#')))
			*p = '\0';
		// Make new line to terminating 0
		if ((p = strchr(line, '\n')))
			*p = '\0';
		p = line;
		// Skip empty lines
		if (is_empty(line))
			continue;
		//printf("No seg fault here\n");
		// Field 1: Check run levels, empty string means runs on all levels
		if (line[0] != ':') {
			if ((first = strtok(line, ":")) != NULL) {
				for (i = 0; first[i] != '\0'; i++) {
					if (first[i] == runlevel[0]) cont = 1;
				}
			}
		//First token is empty, run on all levels
		} else if (line[0] == ':') {
			cont = 2;
		}

		if(!cont)
			continue;

		// Field 2: Check respawn condition
		// cont = 1, first token had characters, need to check because of strtok
		if (cont == 1) {
			if ((second = strtok(NULL, ":")) != NULL) {
				if (strcmp(second, "respawn") == 0) {
					respawn = 1;
				}
			} else {
				continue;
			}
		// cont = 2, first token is empty, strtok on line to get second token
		} else {
			if ((second = strtok(line, ":")) != NULL) {
				if (strcmp(second, "respawn") == 0) {
					respawn = 1;
				}
			} else {
				continue;
			}
		}

		// Field 3: Get command
		if ((third = strtok(NULL, ":")) ==  NULL) continue;

		// Performing execl and fork now
		result = fork();
		if (result == -1) {
			perror("fork:");
			continue;
		} else if (result == 0) {
			// usleep(100000);
			// //printf("Process executing: %d\n", getpid());
			// execl("/bin/sh", "sh", "-c", third, (char *)NULL);
			// perror("/bin/sh");
			// continue;
			do {
				result2 = fork();
				if (result2 == -1) {
					perror("fork:");
					exit(1);
				} else if (result2 == 0) {
					execl("/bin/sh", "sh", "-c", third, (char *)NULL);
					perror("/bin/sh");
				} else if (result2 > 0) {
					int x;
					while((x == wait(&status)) == result2) {
						wait(&status);
						printf("Finished wait\n");
					}
				}
			} while (respawn);
			exit(0);
		} else if (result > 0) { // Parent process, simply go to next line for process
			continue;
		}
	}
	// Should not reach here
	return 1;
}

int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}