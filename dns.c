#define _CRT_SECURE_NO_WARNINGS
/*************************************************************************
   LAB 1

	Edit this file ONLY!

*************************************************************************/


#pragma once
#include "dns.h"
#include"Hashtable.h"
#include <stdio.h>
#define BUF_SIZE 1024
#define _CRT_SECURE_NO_WARNINGS 
#define HASHTABLE_SIZE 130000U

DNSHandle InitDNS()
{
	return (DNSHandle)CreateNewHashtable(HASHTABLE_SIZE);
}

void LoadHostsFile(DNSHandle hDNS, const char* hostsFilePath)
{
	Hashtable* table = (Hashtable*)hDNS;
	FILE* hosts = fopen(hostsFilePath, "r");  // открыть для чтения
	char IP[16];
	char domainName[BUF_SIZE + 1];
	char buf[BUF_SIZE + 1];
	while (!feof(hosts))
	{
		fgets(buf, BUF_SIZE + 1, hosts);                  // считываем строку
		sscanf(buf, "%s%s", IP, domainName);                 // Разделяем имя и ip из буфера
		InsertNodeInHashtable(table, domainName, conversionIP(IP));
	}
	fclose(hosts);
}

void ShutdownDNS(DNSHandle hDNS)
{
	Hashtable* table = (Hashtable*)hDNS;
	DeleteOurHashTable(table);
}

IPADDRESS DnsLookUp(DNSHandle hDNS, const char* hostName)
{
	Hashtable* table = (Hashtable*)hDNS;
	return SearchInHashTable(table, hostName);
}

IPADDRESS conversionIP(const char* IPAdress) {
	IPADDRESS ans = 0;
	unsigned int groupOfNumber = 0;
	int size = strlen(IPAdress);
	for (int i = 0; i <= size; i++) {
		if (IPAdress[i] >= '0' && IPAdress[i] <= '9')
			groupOfNumber = groupOfNumber * 10 + (IPAdress[i] - '0');
		else {
			ans = ans * 256 + groupOfNumber;
			groupOfNumber = 0;
		}
	}
	return ans;
}