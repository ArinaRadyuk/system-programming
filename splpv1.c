/*
 * SPLPv1.c
 * The file is part of practical task for System programming course.
 * This file contains validation of SPLPv1 protocol.
 */


 /*
   Радюк Арина Юрьевна
   14 группа
 */



 /*
 ---------------------------------------------------------------------------------------------------------------------------
 # |      STATE      |         DESCRIPTION       |           ALLOWED MESSAGES            | NEW STATE | EXAMPLE
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 1 | INIT            | initial state             | A->B     CONNECT                      |     2     |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 2 | CONNECTING      | client is waiting for con-| A<-B     CONNECT_OK                   |     3     |
   |                 | nection approval from srv |                                       |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 3 | CONNECTED       | Connection is established | A->B     GET_VER                      |     4     |
   |                 |                           |        -------------------------------+-----------+----------------------
   |                 |                           |          One of the following:        |     5     |
   |                 |                           |          - GET_DATA                   |           |
   |                 |                           |          - GET_FILE                   |           |
   |                 |                           |          - GET_COMMAND                |           |
   |                 |                           |        -------------------------------+-----------+----------------------
   |                 |                           |          GET_B64                      |     6     |
   |                 |                           |        ------------------------------------------------------------------
   |                 |                           |          DISCONNECT                   |     7     |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 4 | WAITING_VER     | Client is waiting for     | A<-B     VERSION ver                  |     3     | VERSION 2
   |                 | server to provide version |          Where ver is an integer (>0) |           |
   |                 | information               |          value. Only a single space   |           |
   |                 |                           |          is allowed in the message    |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 5 | WAITING_DATA    | Client is waiting for a   | A<-B     CMD data CMD                 |     3     | GET_DATA a GET_DATA
   |                 | response from server      |                                       |           |
   |                 |                           |          CMD - command sent by the    |           |
   |                 |                           |           client in previous message  |           |
   |                 |                           |          data - string which contains |           |
   |                 |                           |           the following allowed cha-  |           |
   |                 |                           |           racters: small latin letter,|           |
   |                 |                           |           digits and '.'              |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 6 | WAITING_B64_DATA| Client is waiting for a   | A<-B     B64: data                    |     3     | B64: SGVsbG8=
   |                 | response from server.     |          where data is a base64 string|           |
   |                 |                           |          only 1 space is allowed      |           |
 --+-----------------+---------------------------+---------------------------------------+-----------+----------------------
 7 | DISCONNECTING   | Client is waiting for     | A<-B     DISCONNECT_OK                |     1     |
   |                 | server to close the       |                                       |           |
   |                 | connection                |                                       |           |
 ---------------------------------------------------------------------------------------------------------------------------

 IN CASE OF INVALID MESSAGE THE STATE SHOULD BE RESET TO 1 (INIT)

 */


#include "splpv1.h"
#include <stdio.h>
#include<string.h>



 /* FUNCTION:  validate_message
  *
  * PURPOSE:
  *    This function is called for each SPLPv1 message between client
  *    and server
  *
  * PARAMETERS:
  *    msg - pointer to a structure which stores information about
  *    message
  *
  * RETURN VALUE:
  *    MESSAGE_VALID if the message is correct
  *    MESSAGE_INVALID if the message is incorrect or out of protocol
  *    state
  */


int bitmap[128] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
					0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
					0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };

int dataMap[128] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
				  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };


int natural(char* str) {
	if (str[8] == 0 || str[8] == '\0')
		return 0;
	int i = 8;
	while (str[i] != '\0') {
		if (str[i] >= '0' && str[i] <= '9')
			i++;
		else
			return 0;
	}
	return 1;
}

int b64(char* str) {
	int count = 0;
	int i = 5;
	while (bitmap[str[i]])
	{
		i++;
		count++;
	}
	int equalsCount = 0;
	while (str[i] == '=' && equalsCount < 2)
	{
		i++;
		count++;
		equalsCount++;
	}
	if (str[i] == '\0' && count % 4 == 0)
	{
		return 1;
	}
	return 0;
}
int data_data(char* str) {
	int i = 0;
	while (dataMap[str[i]]) {
		i++;
	}
	return i;
}

int data_command(char* str) {
	if (str[11] != ' ')
		return 0;
	int count = 0;
	int i = 12;
	while (str[i] != ' ') {
		if (dataMap[str[i]])
			i++;
		else
			return 0;
	}
	char* temp = str + i + 1;
	if (!strncmp(temp, "GET_COMMAND", 11))
		return 1;
	else
		return 0;

}

int data_file(char* str) {
	if (str[8] != ' ')
		return 0;
	int count = 0;
	int i = 9;
	while (str[i] != ' ') {
		if (dataMap[str[i]])
			i++;
		else
			return 0;
	}
	char* temp = str + i + 1;
	if (!strncmp(temp, "GET_FILE", 8))
		return 1;
	else
		return 0;

}

int state = 1;
char* comand;
int cmdLength = 0;

enum test_status validate_message(struct Message* msg)
{
	char* text = msg->text_message;
	enum Direction direction = msg->direction;
	switch (state) {
	case 1:
		if (direction == A_TO_B && !strcmp(text, "CONNECT")) {
			state = 2;
			return MESSAGE_VALID;
		}
		else {
			state = 1;
			return MESSAGE_INVALID;
		}
	case 2:
		if (direction == B_TO_A && !strcmp(text, "CONNECT_OK")) {
			state = 3;
			return MESSAGE_VALID;
		}
		else {
			state = 1;
			return MESSAGE_INVALID;
		}
	case 3:
		if (direction == A_TO_B) {
			if (!strcmp(text, "GET_VER")) {
				state = 4;
				return MESSAGE_VALID;
			}
			else if (!strcmp(text, "GET_DATA")) {
				state = 5;
				comand = "GET_DATA";
				cmdLength = 8;
				return MESSAGE_VALID;
			}
			else if (!strcmp(text, "GET_COMMAND")) {
				state = 5;
				comand = "GET_COMMAND";
				cmdLength = 11;
				return MESSAGE_VALID;
			}
			else if (!strcmp(text, "GET_FILE")) {
				state = 5;
				comand = "GET_FILE";
				cmdLength = 8;
				return MESSAGE_VALID;
			}
			else if (!strcmp(text, "GET_B64")) {
				state = 6;
				return MESSAGE_VALID;
			}
			else if (!strcmp(text, "DISCONNECT"))
			{
				state = 7;
				return MESSAGE_VALID;
			}
			else
				return MESSAGE_INVALID;
		}
		else {
			state = 1;
			return MESSAGE_INVALID;
		}
	case 4:
		if (direction == B_TO_A && !strncmp(text, "VERSION ", 8) && natural(text) == 1) {
			state = 3;
			return MESSAGE_VALID;
		}
		else
		{
			state = 1;
			return MESSAGE_INVALID;
		}
	case 5:
		if (direction == B_TO_A) {
			if (!strncmp(text, comand, cmdLength) && text[cmdLength] == ' ')
			{
				char* txt = text + cmdLength + 1;
				int i = data_data(txt);

				if (txt[i] == ' ' && !strncmp(txt + i + 1, comand, cmdLength))
				{
					state = 3;
					return MESSAGE_VALID;
				}
			}
			state = 1;
			return MESSAGE_INVALID;
		}
		else {
			state = 1;
			return MESSAGE_INVALID;
		}
	case 6:
		if (direction == B_TO_A && !strncmp(text, "B64: ", 5) && b64(text) == 1) {
			state = 3;
			return MESSAGE_VALID;
		}
		else {
			state = 1;
			return MESSAGE_INVALID;
		}
	case 7:
		state = 1;
		if (direction == B_TO_A && !strcmp(text, "DISCONNECT_OK")) {
			return MESSAGE_VALID;
		}
		else {
			return MESSAGE_INVALID;
		}
	default:
		state = 1;
		return MESSAGE_INVALID;
	}
}