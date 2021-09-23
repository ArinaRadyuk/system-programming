#ifndef _LAB_1_DNS_HT_
#define _LAB_1_DNS_HT_

#include "dns.h"

typedef struct Hashtable Hashtable;
Hashtable* CreateNewHashtable(unsigned int size);
void DeleteOurHashTable(Hashtable* table);
void InsertNodeInHashtable(Hashtable* table, const char* key, IPADDRESS value);
IPADDRESS SearchInHashTable(Hashtable* table, const char* key);
#endif