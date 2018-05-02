#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

struct Node {
	int pid;         // unique process id
	char *command;   // string containing third field in innitab
	struct Node *next;
};

struct Node* initialize(int id) {
	struct Node *t;
	t = (struct Node *) malloc(sizeof(struct Node));
	t->pid = -1;
	t->next = t;
	// Create list with head node having pid -1
	return t;
}

struct Node* insert(int pid, char *com, struct Node *curr) {
  // Create Node with given pid and command
  // Insert to tail of list and return it
  struct Node *x;
	x = (struct Node *) malloc(sizeof(struct Node));
	x->pid = pid;
	x->command = malloc(1001*sizeof(char));
 	strcpy(x->command, com);
	x->next = curr->next;
	curr->next = x;
	return x;
}

void change_pid(int result, int pid, struct Node *t) {
  t = t->next;
  // Replace process pid with new id of result
  while (t->pid != -1) {
    if (t->pid == pid) {
      t->pid = result;
      break;
    }
  }
}

struct Node *get_proc(int pid, struct Node *t) {
	struct Node *x = t->next;
	while (x->pid > 0) {
		if (x->pid == pid)
      // Return node that has process id of pid
			return x;
		x = x->next;
	}
	// Returns head of list if pid is not in list
	return x;
}

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

	char *line, *first, *second, *third, *p;
	line = malloc(1001*sizeof(char));

	head = initialize(-1);
	curr = head;

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
				for (j = 0; j < strlen(runlevel); j++)
					if (*p == runlevel[j]) {
						putchar(runlevel[j]);
						run = 1; }
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
		result = fork();
		if (result == -1) {
			perror("fork:");
			continue;
		} else if (result == 0) { // Child process
			usleep(100000);
			execl("/bin/sh", "sh", "-c", third, (char *)NULL);
			perror("/bin/sh");
			continue;
		} else if (result > 0) {	// Parent Process
			if (respawn) {
				//printf("linked list no longer empty\n");
				curr = insert(result, third, curr);
				printf("Inserting PID: %d    COMM: %s\n", curr->pid, curr->command);
			}
		}
	}

	//printf("PID: %d    COMM: %s\n", head->pid, head->command);
	//printf("PID: %d    COMM: %s\n", check->pid, check->command);
	//printf("PID: %d    COMM: %s\n", curr->pid, curr->command);

	if (head->pid == curr->pid) return 0; // List is empty so no processes to respawn

	while (1) {
		pid = wait(&status);
		if ((check = get_proc(pid, head))->pid > 0) {
			printf("Process in list PID: %d    COMM: %s\n", check->pid, check->command);
			result = fork();
			printf("Current process id -- %d", result);
			if (result == -1) {
				perror("fork:");
				return(1);
			} else if (result == 0) {
				printf("respawn command: %s", check->command);
				usleep(10000);
				execl("/bin/sh", "sh", "-c", check->command, (char *)NULL);
				perror("/bin/sh");
			} else if (result > 0) {
        // Change pid to new process id that returned
        change_pid(result, check->pid, head);
      }
		}
	}
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
