#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv) {
	int c, status, result;
	status = 0;
	int i;
	int run;
	int respawn;
	char *runlevel;
	runlevel = "\0";
	FILE *fp;
	extern int is_empty(const char *s);


	while ((c = getopt(argc, argv, "r:")) != EOF) {
		switch (c) {
			case 'r':
				strcat(runlevel, optarg);
				break;
			default:
				status = 1;
				break;
		}
	}
	printf("runlevels: %s", runlevel);
	if (status) {
		fprintf(stderr, "usage: %s [-r runlevel] file\n", argv[0]);
		return status;
	}

	if (argc == 2) runlevel = "3"; // No -r flags, default to 3
	if ((fp = fopen(argv[optind], "r")) == NULL) { // Open innitab file
		perror(argv[optind]);
		return 1;
	}

	// if (optind + 1 != argc) {
	// 	fprintf(stderr, "usage: %s [-r runlevel] file\n", argv[0]);
	// 	return status;
	// }
	//
	// if ((fp = fopen(argv[2], "r")) == NULL) {
	// 	perror(argv[2]);
	// 	return 1;
	// }

	char *line, *first, *second, *third, *p;
	line = malloc(1001*sizeof(char));
	first = malloc(1001*sizeof(char));
	second = malloc(1001*sizeof(char));
	third = malloc(1001*sizeof(char));
	memset(first,'\0',1001);
	memset(second,'\0',1001);
	memset(third,'\0',1001);
	while((fgets(line, 1001, fp)) != NULL) {
		i = 0;
		respawn = 0;
		run = 0;
		//printf("LINE: %s\n", line);
		// Remove comments from line
		if ((p = strchr(line, '#')))
			*p = '\0';
		// Remove newline characters from line
		if ((p = strchr(line, '\n')))
			*p = '\0';
		// Skip empty lines
		if (is_empty(line))
			continue;
		p = line;
		/* 1. Get first field for run levels
					Perform strchr but add characters to string first
					as we increment */
		if (*p == ':') {
			run = 1;
			i++;
			first[0] = '\0';
		} else {
			while (*p && *p != ':') {
				if (*p == *runlevel) run = 1;
				first[i++] = *p++;
			}
			first[i] = '\0';
		}
		if (*p == '\0' && !run) // Line ends early, continue to next line
			continue;
		p++;
		//printf("first: %s  pchar: %c\n", first, *p);
		// 2. Get second field for respawn behavior
		i = 0;
		if (*p == ':') {
			i++;
			second[0] = '\0';
		} else {
			while (*p && *p != ':') {
				//putchar(*p);
				second[i++] = *p++;
			}
			second[i] = '\0';
		}
		if (*p == '\0')
			continue;
		else if (strcmp(second, "respawn") == 0){
			//printf("second: %s", second);
			//printf("shouldn't be here respawn: %d", respawn);
			respawn = 1;}
		else if (strcmp(second, "once") == 0)
			respawn = 0;
		else
			continue;
		p++;
		//printf("second: %s  pchar: %c\n", second, *p);
		// 3. Get field for command
		i = 0;
		while (*p && *p != ':')
			third[i++] = *p++;
		third[i] = '\0';
		// Perform fork processes
		do {
			result = fork();
			if (result == -1) {
				perror("fork:");
				return(1);
			} else if (result == 0) { // Child process
				execl("/bin/sh", "sh", "-c", third, (char *)NULL);
				perror("/bin/sh");
				return (127);
			} else if (result > 0) {	// Parent Process
				if (waitpid(result, &status, 0) < 0)
					perror("wait");
			}
		} while (respawn);
	}
	return status;
}


int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}
