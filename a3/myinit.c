#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

// LINKED LIST FOR RESPAWN PROCESSES
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

void pop(int pid, struct Node *t) {
  struct Node *s = t;

  t = t->next;
  // Make sure requested process to be removed isn't head
  // and that t->pid != -1 (process list is empty and only has head)
  if (pid != -1 && t->pid != -1) {
    while (t->pid != pid) {
      t = t->next;
      s = s->next;
    }
    s = t->next;
    free(t);
  }
}

void change_pid(int result, int pid, struct Node *t) {
  struct Node *p = t->next;
  // Replace process pid with new id of result
  while (p->pid != -1) {
    if (p->pid == pid) {
      p->pid = result;
      break;
    }
    p = p->next;
  }
}

struct Node *get_process(int pid, struct Node *t) {
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

void printall(struct Node *t) {
	struct Node *p = t->next;
	int i = 1;
	while (p->pid > 0 ) {
		printf("PID%d: %d\n", i, p->pid);
		p = p->next;
	}
}

int is_space(char *s) {
  while (*s != '\0') {
    if (!isspace(*s))
      return 0;
    s++;
  }
  return 1;
}

int main(int argc, char **argv) {
	struct Node *head, *curr, *check;
	int c, stat, status, respawn, result, i, cont;
	int pid;
	stat = 0;
	respawn = 0;
	char *runlevel, *line, *first, *second, *third, *p;
	FILE *fp;

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

	if (argc == 1 || (optind + 1 != argc)) stat = 1;

	// Error checking
	if (stat) {
		fprintf(stderr, "usage: %s [-r runlevel] file\n", argv[0]);
		return stat;
	}

	if (argc == 2) runlevel = "3";
	// ./myinit -r123456 file1 is invalid
	if (runlevel[1] != '\0') {
		fprintf(stderr, "%s: runlevel must be a single character\n", argv[0]);
		return 1;
	}

	if ((fp = fopen(argv[optind], "r")) == NULL) { // Open innitab file
		perror(argv[optind]);
		return 1;
	}

	line = malloc(1000*sizeof(char));
	// initialize list
	head = initialize(-1);
	curr = head;

	while((fgets(line, 1000, fp)) != NULL) {
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
		if (is_space(line))
			continue;

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
				} else if (strcmp(second, "once") == 0) {
					respawn = 0;
				} else {
					fprintf(stderr, "%s: invalid action %s\n", argv[0], second);
					continue;
				}
			} else {
				continue;
			}
		// cont = 2, first token is empty, strtok on line to get second token
		} else {
			if ((second = strtok(line, ":")) != NULL) {
				if (strcmp(second, "respawn") == 0) {
					respawn = 1;
				} else if (strcmp(second, "once") == 0) {
					respawn = 0;
				} else {
					fprintf(stderr, "%s: invalid action %s\n", argv[0], second);
					continue;
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
			perror("fork");
			return(1);
		} else if (result == 0) { // Child process
			usleep(100000);
			execl("/bin/sh", "sh", "-c", third, (char *)NULL);
			perror("/bin/sh");
			return(1);
		} else if (result > 0) {	// Parent Process
			// Respawn process, insert at end of linked list
			if (respawn) curr = insert(result, third, curr);
		}
	}
	fclose(fp);

	// List is empty, no processes to respawn
	if (head->pid == curr->pid) return 0;

	while (1) {
		pid = wait(&status);
		if ((check = get_process(pid, head))->pid > 0) {
			result = fork();
			if (result == -1) {
				perror("fork");
				return(1);
			} else if (result == 0) {
				execl("/bin/sh", "sh", "-c", check->command, (char *)NULL);
				perror("/bin/sh");
				return(1);
			} else if (result > 0) {
      			// Change pid to new process id that returned
        		change_pid(result, pid, head);
      		}
		}
	}
	// Should not reach here
	return 1;
}
