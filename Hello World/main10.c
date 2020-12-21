#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE (256)

struct treeNode;
typedef struct treeNode TreeNode;
typedef struct treeNode* Tree;
struct treeNode
{
	int number;
	Tree left;
	Tree right;
};

struct list;
typedef struct list List;
typedef struct list* Position;
struct list
{
	Tree node;
	Position next;
};

int ReadFromFile(Position, char*);
int CreateTree(Position, char*);
void Push(Position, Position);
Tree Pop(Position);
Position CreateTreeNumber(int);
Position CreateOperandNode(char, Tree, Tree);
int ElementCounter(Position);
void CreatePrintString(Tree, char*);
void WriteInto(char*, char*);

int main()
{
	List stackHead;
	stackHead.node = NULL;
	stackHead.next = NULL;

	char readFilename[MAX_STRING_SIZE];

	printf("Input name of file:\n");
	scanf(" %s", readFilename);

	if(ReadFromFile(&stackHead, readFilename) != -1 && ElementCounter(&stackHead) == 1)
	{
		char writeFilename[MAX_STRING_SIZE];

		printf("Postfix successfully read from file.\nInput name of write file:\n");
		scanf(" %s", writeFilename);

		char printString[MAX_STRING_SIZE] = "";
		CreatePrintString(Pop(&stackHead), printString);

		WriteInto(printString, writeFilename);
		
	}
	else
	{
		printf("ERROR: INCORRECT POSTFIX EXPRESSION\n");
	}

	return 0;
}

int ReadFromFile(Position stackHead, char* filename)
{
	FILE* filePointer = fopen(filename, "r");

	if(filePointer == NULL)
	{
		printf("ERROR OPENING FILE\n");
		return -1;
	}

	char* postfix = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
	//fgets(postfix, MAX_STRING_SIZE, filePointer);
	fscanf(filePointer, "%[^\n]%*c", postfix);

	int stringLength = strlen(postfix);

	printf("POSTFIX: %s", postfix);

	char readString[MAX_STRING_SIZE];
	int charactersRead = 0;
	int currentTotal = 0;

	do
	{
		sscanf(postfix, " %s %n", readString, &charactersRead);
		
		if(CreateTree(stackHead, readString) == -1)
		{
			return -1;
		}

		postfix += charactersRead;
		currentTotal += charactersRead;
	
	} while (currentTotal < stringLength);

	fclose(filePointer);
	return 0;
}

int CreateTree(Position stackHead, char* givenString)
{
	int isCharacterDigit = 1;

	int i = 0;
	for(i; givenString[i] != '\0'; i++)
	{
		if(!isdigit(givenString[i]))
		{
			isCharacterDigit = 0;
			break;
		}
	}

	if(isCharacterDigit)
	{
		Push(stackHead, CreateTreeNumber(atoi(givenString)));
	}
	else
	{
		if(ElementCounter(stackHead) < 2) 
		{
			return -1;
		}

		Tree valueRight = Pop(stackHead);
		Tree valueLeft = Pop(stackHead);

		if (givenString[0] == '+' || givenString[0] == '-' || givenString[0] == '*' || givenString[0] == '/')
		{
			Push(stackHead, CreateOperandNode(givenString[0], valueLeft, valueRight));
		}
		else
		{
			printf("ERROR: UNRECOGNIZED CHARACTER\n");
			return -1;
		}
	}

	return 0;
}

void Push(Position stackHead, Position newElement)
{
	newElement->next = stackHead->next;
	stackHead->next = newElement;
}

Tree Pop(Position stackHead)
{
	Tree returnedValue;

	Position popThis = stackHead->next;
	stackHead->next = popThis->next;

	returnedValue = popThis->node;

	free(popThis);

	return returnedValue;
}

Position CreateTreeNumber(int number)
{
	Tree newNode = NULL;
	newNode = (Tree)malloc(sizeof(TreeNode));

	newNode->number = number;
	newNode->left = NULL;
	newNode->right = NULL;

	Position newElement = NULL;
	newElement = (Position)malloc(sizeof(struct list));
	newElement->node = newNode;
	newElement->next = NULL;

	return newElement;
}

Position CreateOperandNode(char operand, Tree left, Tree right)
{
	Tree newNode = NULL;
	newNode = (Tree)malloc(sizeof(TreeNode));

	newNode->number = operand;
	newNode->left = left;
	newNode->right = right;

	Position newElement = NULL;
	newElement = (Position)malloc(sizeof(struct list));
	newElement->node = newNode;
	newElement->next = NULL;

	return newElement;
}

int ElementCounter(Position stackHead)
{
	int counter = 0;
	Position P = stackHead->next;

	while (P != NULL)
	{
		P = P->next;
		counter++;
	}

	return counter;
}

void CreatePrintString(Tree currentNode, char* targetString)
{
	if (currentNode == NULL)
	{
		return;
	}
	
	char temporaryString[MAX_STRING_SIZE];
	
	if (currentNode->left == NULL && currentNode->right == NULL)
	{
		sprintf(temporaryString, "%d ", currentNode->number);
		strcat(targetString, temporaryString);
	}
	else
	{
		strcat(targetString, "( ");
		CreatePrintString(currentNode->left, targetString);
		sprintf(temporaryString, "%c ", currentNode->number);
		strcat(targetString, temporaryString);
		CreatePrintString(currentNode->right, targetString);
		strcat(targetString, ") ");
	}
}

void WriteInto(char* writeString, char* writeFilename)
{
	FILE* filePointer = fopen(writeFilename, "w");

	fprintf(filePointer, "%s", writeString);

	fclose(filePointer);
}