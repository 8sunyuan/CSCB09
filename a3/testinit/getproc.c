#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

struct Node {
	int pid;
	char *command;
	struct Node *next;
};

struct Node* initialize(int id) {
	struct Node *t;
	t = (struct Node *) malloc(sizeof(struct Node));
	t->pid = -1;
	t->next = t;
	// Create list with head node have pid -1
	return t;
}

struct Node* insert(int pid, char *com, struct Node *curr) {
	struct Node *x;
	x = (struct Node *) malloc(sizeof(struct Node));
	x->pid = pid;
	x->command = malloc(1001*sizeof(char));
 	strcpy(x->command, com);
	x->next = curr->next;
	curr->next = x;
	// Returns tail node of list
	return x;
}

struct Node *get_proc(int pid, struct Node *t) {
	struct Node *x = t->next;
	while (x->pid > 0) {
		if (x->pid == pid)
			return x;
		x = x->next;
	}
	// Returns head of list if pid is not in list
	return x;
}


int main() {
  struct Node *head, *curr, *check;

  head = initialize(-1);
  printf("initialized PID: %d,  COMM: %s\n", head->pid, head->command);
  curr = head;
  printf("Curr Node   PID: %d,  COMM: %s\n\n", curr->pid, curr->command);
  curr = insert(199784, "FUCKTHIS XD\n", curr);
  printf("Curr Node   PID: %d,  COMM: %s\n", curr->pid, curr->command);
  check = get_proc(199784, head);
  printf("Check Node   PID: %d,  COMM: %s", check->pid, check->command);
  printf("initialized PID: %d,  COMM: %s\n", head->pid, head->command);

  return 0;
}
