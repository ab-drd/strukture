#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DIRECTORY_LENGTH (128)
#define MAX_USER_INPUT (128)
#define MAX_STRING_LENGTH (64)



struct dir;
typedef struct dir Dir;
typedef struct dir* Position;

struct dir
{

	char name[MAX_DIRECTORY_LENGTH];
	Position sibling;
	Position child;

};



struct stack;
typedef struct stack Stack;
typedef struct stack* StackPosition;

struct stack
{
	Position data;
	StackPosition next;
};



void Remove(Position);
void CreateChild(Position);
void InsertAfter(Position, Position);
void PrintDirectory(Position);
void PrintCurrentPath(StackPosition, Position);

Position MoveCurrent(Position);
Position CurrentToParent(StackPosition);
Position MakePosition(char*);

StackPosition CreateStackElement(Position);
int PushStack(StackPosition, Position);
Position PopStack(StackPosition);


int main()
{
	int shouldIStop = 0;
	char userInput[MAX_USER_INPUT];

	Position root = (Position)malloc(sizeof(Dir));
	strcpy(root->name, "C:");
	root->child = NULL;
	root->sibling = NULL;

	Position current = root;

	Stack parentStack;
	parentStack.data = NULL;
	parentStack.next = NULL;

	while (!shouldIStop)
	{
		PrintCurrentPath(current, &parentStack);

		printf("\nOpcije:\nmd - stvaranje poddirektorija\ncd - ulazak u odredjeni direktorij");
		printf("\ncd.. - povratak na roditeljski direktorij\ndir - ispis poddirektorija u trenutnom direktoriju\nexit - izlazak iz programa\n\n");
		scanf(" %s", userInput);
		system("cls");

		if (!strcmp(userInput, "md"))
		{
			CreateChild(current);
		}
		else if (!strcmp(userInput, "cd"))
		{
			current = MoveCurrent(current, &parentStack);
		}
		else if (!strcmp(userInput, "cd.."))
		{
			Position value = CurrentToParent(&parentStack);
			if(value != NULL) 
			{
				current = value;
			}
			else 
			{
				printf("Opcija se ne moze izvrsiti\n");
			}
		}
		else if (!strcmp(userInput, "dir"))
		{
			PrintDirectory(current);
		}
		else if (!strcmp(userInput, "exit"))
		{
			while (parentStack.next != NULL)
			{
				PopStack(&parentStack);
			}

			if (root->child != NULL) 
			{
				Remove(root->child);
			}

			printf("o/");

			shouldIStop = 1;
		}
		else 
		{
			printf("Neispravna opcija!\n");
		}

		puts("");
	}

	return 0;
}

void Remove(Position current)
{
	if (current->child == NULL) {
		return;
	}

	Remove(current->sibling);
	Remove(current->child);

	free(current);
}

void CreateChild(Position current)
{
	int satisfied = 1;
	char* newDirectoryName[MAX_USER_INPUT];

	printf("Unesite ime novog poddirektorija: ");
	scanf(" %s", newDirectoryName);

	if (current->child == NULL)
	{
		current->child = MakePosition(newDirectoryName);
	}
	else
	{
		Position newChild = MakePosition(newDirectoryName);

		if(strcmp(newDirectoryName, current->child->name) < 0)
		{
			newChild->sibling = current->child;
			current->child = newChild;
		}
		else if(!strcmp(newDirectoryName, current->child->name)) 
		{
			printf("Poddirektorij vec postoji\n");
			satisfied = 0;
		}
		else
		{
			current = current->child;

			while (current->sibling != NULL)
			{
				if (strcmp(newDirectoryName, current->sibling->name) < 0) 
				{
					InsertAfter(current, newChild);
					return;
				}

				current = current->sibling;
			}

			InsertAfter(current, newChild);
		}
	}

	if (satisfied) 
	{
		printf("Direktorij imena '%s' uspjesno stvoren\n", newDirectoryName);
	}
}

void InsertAfter(Position currentPosition, Position newPosition)
{
	newPosition->sibling = currentPosition->sibling;
	currentPosition->sibling = newPosition;
}

void PrintDirectory(Position current) 
{
	if (current->child == NULL) 
	{
		printf("Direktorij je prazan!\n");
	}
	else 
	{
		current = current->child;

		printf("Direktorij sadrzava:\n\n");
		while (current != NULL) 
		{
			printf("		%s\n", current->name);
			current = current->sibling;
		}
	}
}

void PrintCurrentPath(Position current, StackPosition stackHead)
{
	printf("<");
	if(stackHead->next != NULL) 
	{
		StackPosition stackElement = stackHead->next;

		Stack printHelper;
		printHelper.data = NULL;
		printHelper.next = NULL;

		while (stackElement != NULL)
		{
			stackElement->next = printHelper.next;
			printHelper.next = stackElement;

			stackElement = stackElement->next;
		}

		StackPosition printElement = printHelper.next;

		while (printElement != NULL)
		{
			printf("%s/", printElement->data->name);
			printElement = printElement->next;
		}
	}
	
	printf("%s>", current->name);
}

Position MoveCurrent(Position current, StackPosition stackHead)
{
	char* directoryName[MAX_USER_INPUT];

	if (current->child == NULL)
	{
		printf("Direktorij je prazan!\n");
		return current;
	}
	else
	{
		printf("Unesite ime direktorija na koji zelite ici: ");
		scanf(" %s", directoryName);

		Position rememberParent = current;

		Position searchPosition = current->child;

		while (searchPosition != NULL) 
		{
			if (!strcmp(searchPosition->name, directoryName)) 
			{

				printf("Uspjesno prebacivanje na direktorij '%s'\n", searchPosition->name);
				PushStack(rememberParent, stackHead);
				return searchPosition;
			}
			searchPosition = searchPosition->sibling;
		}

		printf("Direktorij nije pronadjen\n");
		return rememberParent;
	}
}

Position MakePosition(char* newDirectoryName)
{
	Position newDirectory = (Position)malloc(sizeof(Dir));

	strcpy(newDirectory->name, newDirectoryName);
	newDirectory->child = NULL;
	newDirectory->sibling = NULL;

	return newDirectory;
}

Position CurrentToParent(StackPosition stackHead)
{
	Position returnValue = PopStack(stackHead);
	if (returnValue != NULL) 
	{
		return returnValue;
	}
	else 
	{
		return NULL;
	}
}

StackPosition CreateStackElement(Position dirElement)
{
	StackPosition newStackElement = (StackPosition)malloc(sizeof(Stack));

	newStackElement->data = dirElement;
	newStackElement->next = NULL;

	return newStackElement;
}

int PushStack(Position parent, StackPosition stackHead)
{
	StackPosition newStackElement = CreateStackElement(parent);

	newStackElement->next = stackHead->next;
	stackHead->next = newStackElement;

	printf("Dodano na parent stack\n");

	return 1;
}

Position PopStack(StackPosition Head)
{
	StackPosition popThis = Head->next;

	if (popThis == NULL) 
	{
		return NULL;
	}
	else 
	{
		Position returnValue = popThis->data;

		Head->next = popThis->next;

		free(popThis);

		return returnValue;
	}
}