#ifndef _LAB_1_DNS_SLL_
#define _LAB_1_DNS_SLL_
#include "dns.h"

typedef struct SinglyLinkedList {
	const char* key;
	IPADDRESS value;
	struct SinglyLinkedList* next;
}
SinglyLinkedList;

SinglyLinkedList* createListNode(const char* akey, IPADDRESS avalue);
void deleteListNode(SinglyLinkedList* node);
void deleteList(SinglyLinkedList* start);

#endif
