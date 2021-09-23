#define _CRT_SECURE_NO_WARNINGS
#include "Hashtable.h"
#include "SinglyLinkedList.h"
#include <stdlib.h>
#define HASH_BASE 47UL//179,47,181

typedef struct Hashtable {
	unsigned int size;
	SinglyLinkedList** arrayInHashTable;
}
Hashtable;

Hashtable* CreateNewHashtable(unsigned int size) {
	Hashtable* ans = (Hashtable*)malloc(sizeof(Hashtable));  // выделяемем память
	(*ans).arrayInHashTable = (SinglyLinkedList**)calloc(size, sizeof(SinglyLinkedList*));  // выделяем блок памяти для массива и забиваем нулями 
	(*ans).size = size;
	return ans;
}

void DeleteOurHashTable(Hashtable* table) {
	for (int i = 0; i < (*table).size; i++)
	{
		deleteList((*table).arrayInHashTable[i]);
	}
	free((*table).arrayInHashTable);
	free(table);
}

void InsertNodeInHashtable(Hashtable* table, const char* key, IPADDRESS value) {
	SinglyLinkedList* node = createListNode(key, value);
	int position = hashFunction(key) % (*table).size;
	SinglyLinkedList* pointer = (*table).arrayInHashTable[position], * prer = NULL;
	while (pointer != NULL)
	{
		prer = pointer;
		pointer = (*pointer).next;
	}
	if (prer == NULL)
	{
		(*table).arrayInHashTable[position] = node;
	}
	else {
		(*prer).next = node;
	}
}

IPADDRESS SearchInHashTable(Hashtable* table, const char* key) {
	int position = hashFunction(key) % (*table).size;
	SinglyLinkedList* pointer = (*table).arrayInHashTable[position];
	while (pointer != NULL)
	{
		if (strcmp(key, (*pointer).key) == 0)
			return (*pointer).value;
		pointer = (*pointer).next;
	}
	return INVALID_IP_ADDRESS;
}


unsigned long hashFunction(const char* str)
{
	unsigned long ans = 0;
	int size = strlen(str);
	for (int i = 0; i < size; i++)
	{
		ans = ans * HASH_BASE + str[i];
	}
	return ans;
}