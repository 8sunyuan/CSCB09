#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

struct item {
	int key;
	int data;
	struct item *next;
};

struct item *head = NULL;

int main()
{
    extern void filllist();
    extern void printall();
    filllist();
    printall();
    return(0);
}

void filllist()
{
    struct item *a, *b, *c, *d;
 
    a = (struct item*)malloc(sizeof(struct item));
    b = (struct item*)malloc(sizeof(struct item));
    c = (struct item*)malloc(sizeof(struct item));
    d = (struct item*)malloc(sizeof(struct item));
    a->key = 5;
    a->data = 0;
    a->next = b;
    b->key = 20;
    b->data = 2;
    b->next = c;
    c->next = d;
    c->key = 22;
    c->data = 6;
    d->key = 38;
    d->data = 3;
    d->next = NULL;
    head = a;
}

void printall() {
	struct item *curr = head;
	while (curr != NULL) {
		printf("%d: %d\n", curr->key, curr->data);
		curr = curr->next;
	}
	printf("[end]\n");
}
