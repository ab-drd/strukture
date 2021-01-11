#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STRING_LENGTH (128)

struct listElement;
typedef struct listElement List;
typedef struct listElement* Position;

struct listElement 
{
	char firstName[MAX_STRING_LENGTH];
	char lastName[MAX_STRING_LENGTH];
	char mbg[MAX_STRING_LENGTH];

	Position next;
};

struct hashTable;
typedef struct hashTable HashTable;
typedef struct hashTable* Hash;

struct hashTable {
	int tableSize;
	Position* hashList;
};

int ValidIntInputAndCheck(int, int);

Hash InitializeHashTable(int);
int NextPrime(int);
bool PrimeChecker(int);

int Key(char*, int);
int ReadFromFile(char*, Hash);
Position CreatePosition(char*, char*, char*);
int AddToTable(Position, Hash);
void FindMBG(char*, char*, Hash);
bool IsMBGInHash(char*, Hash);

void PrintHashTable(Hash);

int main()
{
	Hash hashTable = InitializeHashTable(11);
	char filename[MAX_STRING_LENGTH];

	printf("Input read filename:\n");
	scanf(" %s", filename);

	if(ReadFromFile(filename, hashTable) == -1)
	{
		return -1;
	}

	bool shouldIStop = false;
	while(!shouldIStop)
	{
		printf("\nHASH TABLE:\n\n");
		PrintHashTable(hashTable);

		printf("\nDo you want to get a student's MBG?\n<1> Yes\n<0> Exit program\n");
		int userChoice = ValidIntInputAndCheck(0, 1);

		if (userChoice)
		{
			printf("Input last and first name of student you want to find the MBG of in format '(LastName) (FirstName)':\n");

			char someFirstName[MAX_STRING_LENGTH];
			char someLastName[MAX_STRING_LENGTH];

			scanf(" %s %s", someLastName, someFirstName);

			FindMBG(someFirstName, someLastName, hashTable);

			printf("Press any key to continue...\n");
			getch();
			system("cls");
		}
		else
		{
			printf("o/\n");
			shouldIStop = true;
		}
	}

	return 0;
}

int ValidIntInputAndCheck(int range1, int range2)
{
	int someInteger = 0;
	scanf("%d", &someInteger);

	while (someInteger < range1 || someInteger > range2) {
		printf("\nNumber is out of range %d - %d\nRepeat input:\n", range1, range2);
		scanf("%d", &someInteger);
	}

	return someInteger;
}

Hash InitializeHashTable(int tableSize)
{
	Hash table = NULL;
	int i;

	table = (Hash)malloc(sizeof(HashTable));

	if(table == NULL)
	{
		printf("ERROR while initalizing memory");
		return;
	}

	table->tableSize = tableSize;
	table->hashList = (Position*)malloc(NextPrime(tableSize) * sizeof(Position));

	if (table->hashList == NULL)
	{
		printf("ERROR while initalizing memory");
		return;
	}

	for(i = 0; i < table->tableSize; i++)
	{
		table->hashList[i] = NULL;
	}

	return table;
}

int NextPrime(int number)
{
	bool haveIFound = false;
	int potentialPrime = number;

	while(!haveIFound)
	{
		if (PrimeChecker(potentialPrime))
		{
			haveIFound = true;
		}

		potentialPrime++;
	}

	return potentialPrime;
}

bool PrimeChecker(int number)
{
	if (number <= 1) return false;
	if (number <= 3) return false;

	if (number % 2 == 0 || number % 3 == 0) return false;

	int i;
	for(i = 5; i*i <= number; i = i + 6)
	{
		if(number % i == 0 || number % (i + 2) == 0)
		{
			return false;
		}
	}

	return true;
}

int Key(char* string, int tableSize)
{
	int key = -1;
	int sum = 0;
	int i = 0;

	int stringLength = strlen(string);

	if(stringLength < 5)
	{
		for(i = 0; i < stringLength; i++)
		{
			sum += string[i];
		}
	}
	else
	{
		for (i = 0; i < 5; i++)
		{
			sum += string[i];
		}
	}

	return sum % tableSize;
}

int ReadFromFile(char* filename, Hash hashTable)
{
	char readFirstName[MAX_STRING_LENGTH];
	char readLastName[MAX_STRING_LENGTH];
	char readMBG[MAX_STRING_LENGTH];

	FILE* filePointer = fopen(filename, "r");

	if(filePointer == NULL)
	{
		printf("ERROR: FILE WAS NULL\n");
		return -1;
	}

	printf("\nSuccessfully opened file, reading...\n");

	while(fscanf(filePointer, " %s %s %s", readFirstName, readLastName, readMBG) != EOF)
	{
		if(AddToTable(CreatePosition(readFirstName, readLastName, readMBG), hashTable) == -1)
		{
			fclose(filePointer);
			return -1;
		}
	}

	printf("\nHash loaded...\n");

	fclose(filePointer);
	return 0;
}

Position CreatePosition(char* firstName, char* lastName, char* mbg)
{
	Position newPosition = (Position)malloc(sizeof(List));

	strcpy(newPosition->firstName, firstName);
	strcpy(newPosition->lastName, lastName);
	strcpy(newPosition->mbg, mbg);

	newPosition->next = NULL;

	return newPosition;
}

int AddToTable(Position newHashElement, Hash hashTable)
{
	if(IsMBGInHash(newHashElement->mbg, hashTable))
	{
		printf("\nTried to input duplicate MBG (%s %s - %s)\n >>> Element will not be inputted\n", newHashElement->lastName,
			newHashElement->firstName, newHashElement->mbg);
	}
	else
	{
		int hashKey = Key(newHashElement->lastName, hashTable->tableSize);

		char newElement[MAX_STRING_LENGTH * 3] = "";
		strcat(newElement, newHashElement->lastName);
		strcat(newElement, newHashElement->firstName);
		strcat(newElement, newHashElement->mbg);

		if (hashTable->hashList[hashKey] == NULL)
		{
			hashTable->hashList[hashKey] = newHashElement;
		}
		else
		{
			Position currentElement = hashTable->hashList[hashKey];

			char current[MAX_STRING_LENGTH * 3] = "";
			strcat(current, currentElement->lastName);
			strcat(current, currentElement->firstName);
			strcat(current, currentElement->mbg);

			if (strcmp(newElement, current) < 0)
			{
				hashTable->hashList[hashKey] = newHashElement;
				newHashElement->next = currentElement;
			}
			else
			{
				while (currentElement->next != NULL)
				{
					char otherCurrent[MAX_STRING_LENGTH * 3] = "";
					strcat(otherCurrent, currentElement->next->lastName);
					strcat(otherCurrent, currentElement->next->firstName);
					strcat(otherCurrent, currentElement->next->mbg);

					if (strcmp(newElement, otherCurrent) < 0)
					{
						newHashElement->next = currentElement->next->next;
						currentElement->next = newHashElement->next;
						break;
					}
				}

				if (currentElement->next == NULL)
				{
					newHashElement->next = currentElement->next;
					currentElement->next = newHashElement;
				}
			}
		}
	}
	
	return 0;
}

void FindMBG(char* someFirstName, char* someLastName, Hash hashTable)
{
	int hashKey = Key(someLastName, hashTable->tableSize);

	Position currentElement = hashTable->hashList[hashKey];

	int counter = 0;

	while(currentElement != NULL)
	{
		if(counter == 1)
		{
			printf("Found more than two students with the same name - outputting all\n");
		}

		if(!strcmp(someFirstName, currentElement->firstName) && !strcmp(someLastName, currentElement->lastName))
		{
			if(counter == 0)
			{
				printf("Found matching student:\n");
			}
			printf("	>>> %s %s - %s\n", someLastName, someFirstName, currentElement->mbg);
			counter++;
		}
		currentElement = currentElement->next;
	}

	if(counter == 0)
	{
		printf("Student was not found in Hash\n");
	}
}

bool IsMBGInHash(char* someMBG, Hash hashTable)
{
	int i;

	bool foundMBG = false;

	for(i = 0; i < hashTable->tableSize; i++)
	{
		Position currentElement = hashTable->hashList[i];
		while(currentElement != NULL)
		{
			if(!strcmp(currentElement->mbg, someMBG))
			{
				foundMBG = true;
				break;
			}

			currentElement = currentElement->next;
		}

		if(foundMBG)
		{
			break;
		}
	}

	return foundMBG;
}

void PrintHashTable(Hash hashTable)
{
	int i;

	for(i = 0; i < hashTable->tableSize; i++)
	{
		printf("	>>> KEY %d <<<\n", i);
		Position currentList = hashTable->hashList[i];
		int counter = 1;
		while(currentList != NULL)
		{
			printf("	       >%d< %s %s\n", counter++, currentList->lastName, currentList->firstName);

			currentList = currentList->next;
		}
	}
}