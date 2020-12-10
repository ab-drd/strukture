#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_CHAR 256

struct element;
typedef struct element Element;
typedef struct element* Position;
struct element
{
	int value;
	Position next;

};

int ReadFromFile(Position, char*);
int ReadUserInput(Position);
int CalculatePostfix(Position, char*);

void Push(Position, Position);
int Pop(Position);

Position MakeNewElement(int);

int main()
{
	int postfixInputChoice = 0;
	int shouldIStop = 0;
	int counter = 1;

	while (!shouldIStop)
	{
		char* filename[MAX_CHAR];
		Element head;
		head.next = NULL;

		printf("=========================\n\t   %d\n=========================\n", counter++);
		printf("Odaberite nacin upisa postfix izraza:\n(1)\tCitanje iz datoteke\n(2)\tUpis preko tipkovnice\n(0)\tIzlaz iz programa\n\n");

		scanf("%d", &postfixInputChoice);

		switch (postfixInputChoice)
		{
			case 1:

				puts("\nUnesite ime datoteke iz koje zelite ispisivati:");
				scanf(" %s", filename);

				if (ReadFromFile(&head, filename) != -1 && ElementCounter(&head) == 1)
				{
					printf("Rezultat postfix izraza: %d", Pop(&head));
				}

				else
				{
					printf("\nDani postfix izraz je neispravan\n");
				}

				getchar();

				printf("\n\nPress ENTER to continue...\n=========================\n");
				getchar();

				system("cls");

				break;

			case 2:

				if (ReadUserInput(&head) != -1 && ElementCounter(&head) == 1)
				{
					printf("\nRezultat postfix izraza: %d", Pop(&head));
				}

				else
				{
					printf("\nDani postfix izraz je neispravan\n");
				}

				printf("\n\nPress ENTER to continue...\n=========================\n");
				getchar();

				system("cls");

				break;

			case 0:
				system("cls");
				printf("\nBye bye :(\n");
				shouldIStop = 1;
				break;

			default:
				printf("\nNeispravan unos\n");
				break;
		}
	}

	return 0;
}



int ReadFromFile(Position head, char* filename)
{
	char* wholeString = (char*)malloc(MAX_CHAR * sizeof(char));
	char* readString[MAX_CHAR];

	int e;
	int stringLength = 0;
	int total = 0;
	
	FILE* filePointer;
	filePointer = fopen(filename, "r");

	if (filePointer == NULL)
	{
		printf("ERROR OPENING FILE\n");
		return -1;
	}

	fgets(wholeString, MAX_CHAR, filePointer);

	stringLength = strlen(wholeString);

	printf("\nPOSTFIX IZRAZ: %s", wholeString);

	do
	{
		sscanf(wholeString, " %s %n", readString, &e);

		if (CalculatePostfix(head, readString) == -1) 
		{
			return -1;
		}

		wholeString += e;
		total += e;

	} while (total < stringLength);

	fclose(filePointer);
	return 0;
}



int ReadUserInput(Position head)
{
	char* wholeString = (char*) malloc (MAX_CHAR * sizeof(char));
	char* readString[MAX_CHAR];

	int e;
	int stringLength = 0;
	int total = 0;

	printf("\nUnesite vas postfix izraz - odvojite sve elemente razmakom:\n");

	getchar();
	
	gets(wholeString, MAX_CHAR);

	stringLength = strlen(wholeString);

	do
	{
		sscanf(wholeString, " %s %n", readString, &e);

		if (CalculatePostfix(head, readString) == -1) 
		{
			return -1;
		}

		wholeString += e;
		total += e;

	} while (total < stringLength);

	return 0;
}



int CalculatePostfix(Position head, char* currentString)
{
	int returnValue = 0;

	int value1 = 0;
	int value2 = 0;

	int isCharacterDigit = 1;

	int i = 0;

	for (i; currentString[i] != '\0'; i++)
	{
		if (!isdigit(currentString[i]))
		{
			isCharacterDigit = 0;
			break;

		}
	}

	if (isCharacterDigit)
	{
		Push(head, MakeNewElement(atoi(currentString)));
	}
	else
	{
		if (ElementCounter(head) < 2)
		{
			returnValue = -1;
		}

		else
		{
			value1 = Pop(head);
			value2 = Pop(head);

			if (currentString[0] == '+')
			{
				Push(head, MakeNewElement(value1 + value2));
			}
			else if (currentString[0] == '-')
			{
				Push(head, MakeNewElement(value2 - value1));
			}
			else if (currentString[0] == '*')
			{
				Push(head, MakeNewElement(value1 * value2));
			}
			else if (currentString[0] == '/')
			{
				Push(head, MakeNewElement(value2 / value1));
			}
		}
	}

	return returnValue;
}



void Push(Position head, Position newElement)
{
	newElement->next = head->next;
	head->next = newElement;
}



int Pop(Position Head)
{
	int returnedValue;

	Position popThis = Head->next;
	Head->next = popThis->next;
	returnedValue = popThis->value;

	free(popThis);

	return returnedValue;
}



Position MakeNewElement(int someValue)
{
	Position newElement = (Position)malloc(sizeof(Element));

	newElement->value = someValue;
	newElement->next = NULL;

	return newElement;
}



int ElementCounter(Position head)
{
	int counter = 0;
	Position P = head->next;

	while (P != NULL) {
		P = P->next;
		counter++;
	}

	return counter;

}