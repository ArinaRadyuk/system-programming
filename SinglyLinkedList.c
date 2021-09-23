#define _CRT_SECURE_NO_WARNINGS
#include "SinglyLinkedList.h"
#include <string.h>

SinglyLinkedList* createListNode(const char* akey, IPADDRESS avalue) {
	SinglyLinkedList* ans = (SinglyLinkedList*)malloc(sizeof(SinglyLinkedList));
	(*ans).key = (const char*)malloc((strlen(akey) + 1) * sizeof(char));
	strcpy((char*)(*ans).key, akey);
	(*ans).value = avalue;
	(*ans).next = NULL;
	return ans;
}

void deleteListNode(SinglyLinkedList* node) {
	free((*node).key);
	free(node);
}

void deleteList(SinglyLinkedList* start)
{
	SinglyLinkedList* curr = start, * temp;
	while (curr != NULL)
	{
		temp = curr;
		curr = (*curr).next;
		deleteListNode(temp);
	}
}
