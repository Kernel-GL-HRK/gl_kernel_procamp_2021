/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define NULL ((void *)0)

typedef struct LinkedList {
	char *data;
	struct LinkedList *next;
} linkedList;

linkedList *createList(char *initData);
linkedList *addToStart(char *data, linkedList *headNode);
linkedList *addToEnd(char *data, linkedList *headNode);
linkedList *add(char *data, int id, linkedList *headNode);
linkedList *getNode(int id, linkedList *headNode);
void editNode(int id, char *newData, linkedList *headNode);
void deleteList(linkedList *headNode);
linkedList *deleteNode(int id, linkedList *headNode);

#endif
