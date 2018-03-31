#include <stdio.h>
#include <stdlib.h>

struct item {
    int key;
    int data;
    struct item *next;
};

struct item *head = NULL;

int main()
{
    extern void insert(int key, int data), delete(int key), printall();
    extern int search(int key);

    insert(38, 3);
    insert(20, 2);
    insert(5, 0);
    insert(22, 6);
    insert(46, 18);

    printf("With all five items:\n");
    printall();

    printf("After delete(22):\n");
    delete(22);
    printall();
    printf("After delete(5):\n");
    delete(5);
    printall();
    printf("delete(30) shouldn't do anything:\n");
    delete(30);
    printall();

    return(0);
}


void insert(int key, int data)
{
    struct item *new, **pp;

    /* create the new item */
    if ((new = malloc(sizeof(struct item))) == NULL) {
        fprintf(stderr, "out of memory!\n");  /* unlikely */
        exit(1);
    }
    new->key = key;
    new->data = data;

    /* find the (struct item *) to place it at */
    for (pp = &head; *pp && (*pp)->key < key; pp = &(*pp)->next)
        ;

    /* link it in */
    new->next = *pp;
    *pp = new;

}


void delete(int key)
{
    struct item **pp;

    /* find the (struct item *) which points to the item to be deleted */
    for (pp = &head; *pp && (*pp)->key < key; pp = &(*pp)->next)
        ;

    if (*pp && (*pp)->key == key) {
        struct item *old = *pp;
        *pp = (*pp)->next;
        free(old);
    }
}


int search(int key) /* returns -1 if not found */
{
    struct item *p;

    for (p = head; p && p->key < key; p = p->next)
        ;
    if (p && p->key == key)
        return(p->data);
    else
        return(-1);
}


void printall()
{
    struct item *p;
    for (p = head; p; p = p->next)
        printf("%d: %d\n", p->key, p->data);
    printf("[end]\n");
}
