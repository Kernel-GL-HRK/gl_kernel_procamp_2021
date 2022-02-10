#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define NULL ((void *)0)

typedef struct LinkedList
{
    char *data;
    struct LinkedList *next;
} linkedList;

linkedList *createList(char *);
linkedList *addToStart(char *, linkedList *);
linkedList *addToEnd(char *, linkedList *);
linkedList *add(char *, int, linkedList *);
linkedList *getNode(int, linkedList *);
void editNode(int, char *, linkedList *);
void deleteList(linkedList *);
linkedList *deleteNode(int, linkedList *);

#endif
