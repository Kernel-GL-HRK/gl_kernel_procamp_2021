// SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note

#define __NO_VERSION__

#include <linux/module.h>
#include <linux/slab.h>
#include "linked_list.h"

linkedList *createList(char *initData)
{
	linkedList *headNode = (linkedList *)kmalloc(sizeof(linkedList), GFP_KERNEL);

	headNode->data = initData;
	headNode->next = NULL;
	return headNode;
}

linkedList *addToStart(char *data, linkedList *headNode)
{
	linkedList *newNode = (linkedList *)kmalloc(sizeof(linkedList), GFP_KERNEL);

	newNode->data = data;
	newNode->next = headNode;
	return newNode;
}

linkedList *addToEnd(char *data, linkedList *headNode)
{
	if (headNode == NULL) {
		headNode = createList(data);
		return headNode;
	}
	linkedList *newNode = (linkedList *)kmalloc(sizeof(linkedList), GFP_KERNEL);

	newNode->data = data;
	newNode->next = NULL;
	linkedList *headPtr = headNode;

	while (headPtr->next != NULL)
		headPtr = headPtr->next;
	headPtr->next = newNode;
}

linkedList *add(char *data, int id, linkedList *headNode)
{
	if (headNode == NULL) {
		headNode = createList(data);
		return headNode;
	}
	if (!id) {
		headNode = addToStart(data, headNode);
		return headNode;
	}
	linkedList *headPtr = headNode;

	headPtr = getNode(id, headPtr);

	linkedList *newNode = (linkedList *)kmalloc(sizeof(linkedList), GFP_KERNEL);

	newNode->data = data;
	newNode->next = headPtr->next;
	headPtr->next = newNode;
	return headNode;
}

linkedList *getNode(int id, linkedList *headNode)
{
	int count = 0;

	while (count < id && headNode->next != NULL) {
		headNode = headNode->next;
		count++;
	}
	return headNode;
}

void editNode(int id, char *newData, linkedList *headNode)
{
	headNode = getNode(id, headNode);
	headNode->data = newData;
}

void deleteList(linkedList *headNode)
{
	linkedList *headPtr;

	while (headNode != NULL) {
		headPtr = headNode;
		headNode = headNode->next;
		kfree(headPtr);
	}
}

linkedList *deleteNode(int id, linkedList *headNode)
{
	linkedList *headPtr;

	if (!id) {
		headPtr = headNode->next;
		kfree(headNode);
		return headPtr;
	}
	headNode = getNode(id, headNode);
	headPtr = headNode->next;
	headNode->next = headPtr->next;
	kfree(headPtr);
	return headNode;
}

MODULE_AUTHOR("Artem Shyshko");
MODULE_LICENSE("GPL");
