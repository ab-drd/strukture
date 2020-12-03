#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILENAME 256
#define MAX_CHAR 256

struct list;
typedef struct list* Position;
typedef struct list List;

struct list {
	int element;
	Position next;
};


int ValidIntInputAndCheck(int, int);
int CountElements(Position);

int PushStack(Position);
Position PushQueue(Position);
int Pop(Position);


void PrintElements(Position, int);
Position CreateElement();



int main() 
{
	int typeChoice = -1;
	int stackActionChoice = -1;
	int queueActionChoice = -1;
	int stopCondition = 1;
	int counter = 1;

	puts("\n>>>>>>>>>> Dobrodosli na pocetak programa <<<<<<<<<<	");

	puts("\nOdaberite s kojim tipom podatka zelite raditi:\n1 - stog\n2 - red\n0 - izlaz iz programa");

	typeChoice = ValidIntInputAndCheck(0, 2);

	switch (typeChoice) 
	{
		case 1:
			system("cls");
			puts("\n>>>>>>>>>> Odabrali ste rad sa stogom <<<<<<<<<<");

			List stackHead;
			stackHead.next = NULL;
			
			while (stopCondition) 
			{
				PrintElements(&stackHead, 1);

				printf("\n========= %d. akcija || %d elemenata =========\n\n", counter++, CountElements(&stackHead));

				puts("Odaberite akciju:\n1 - stavljanje elementa na stog\n2 - skidanje elementa sa stoga\n0 - izlaz iz programa");

				stackActionChoice = ValidIntInputAndCheck(0, 2);
				system("cls");

				switch(stackActionChoice) 
				{
					case 1:

						PushStack(&stackHead);
						break;

					case 2:

						if (CountElements(&stackHead) != 0) {
							Pop(&stackHead);
						}

						break;

					case 0:
						stopCondition = 0;
						break;

				}

			}
			break;

		case 2:
			system("cls");
			puts("\n>>>>>>>>>> Odabrali ste rad sa redom <<<<<<<<<<");

			List queueHead;
			queueHead.next = NULL;

			Position lastQueue = &queueHead;

			while (stopCondition)
			{
				PrintElements(&queueHead, 2);

				printf("\n========= %d. akcija || %d elemenata =========\n\n", counter++, CountElements(&queueHead));

				puts("Odaberite akciju:\n1 - stavljanje elementa na red\n2 - skidanje elementa sa reda\n0 - izlaz iz programa");

				queueActionChoice = ValidIntInputAndCheck(0, 2);
				system("cls");
				switch (queueActionChoice)
				{
				case 1:

					lastQueue = PushQueue(lastQueue);
					break;

				case 2:

					if (CountElements(&queueHead) != 0) {
						Pop(&queueHead);
						if (CountElements(&queueHead) == 0) {
							lastQueue = &queueHead;
						}
					}

					break;

				case 0:
					stopCondition = 0;
					break;

				}
			}
			break;

		case 0:
			break;

	}

	return 0;
}

int ValidIntInputAndCheck(int range1, int range2) 
{
	int someInteger = 0;

	printf("\nUnesite broj izmedju %d i %d:\n", range1, range2);
	scanf("%d", &someInteger);

	while (someInteger < range1 || someInteger > range2) {
		printf("\nUneseni broj je izvan ranga %d - %d\nPonovite unos:\n", range1, range2);
		scanf("%d", &someInteger);
	}

	return someInteger;
}

int CountElements(Position stackHead)
{
	int countedElements = 0;
	Position P = stackHead->next;

	while (P != NULL)
	{
		countedElements++;
		P = P->next;

	}
	
	return countedElements;
}

int RandomIntegerInGivenRange(int lower, int higher)
{
	int randomInteger;

	srand(time(0));

	randomInteger = (rand() % (higher - lower + 1)) + lower;

	return randomInteger;
}

int PushStack(Position stackHead)
{
	Position newStackElement = CreateElement();

	newStackElement->next = stackHead->next;
	stackHead->next = newStackElement;

	printf("\n>>>>> Element %d dodan na stog\n", newStackElement->element);

	return 1;
}

Position PushQueue(Position queueOldLast) 
{
	Position newQueueElement = CreateElement();

	queueOldLast->next = newQueueElement;

	printf("\n>>>>> Element %d dodan na red\n", newQueueElement->element);

	return newQueueElement;

}

int Pop(Position Head)
{
	Position popThis = Head->next;
	Head->next = popThis->next;

	printf("\n>>>>> Element %d skinut sa stoga/reda\n", popThis->element);

	free(popThis);

	return 1;
}

void PrintElements(Position head, int choice)
{
	Position P = head->next;

	if (CountElements(head) == 0)
	{
		
		puts("===========================");

		if (choice == 1) {
			printf("\nTrenutno nema elemenata na stogu");
		}
		else {
			printf("\nTrenutno nema elemenata u redu");
		}
	}

	else 
	{
		if (choice == 1) {
			puts("\nISPIS STOGA:");
		}
		else {
			puts("\nISPIS REDA:");
		}

		for (P; P != NULL; P = P->next)
		{
			printf("%d ", P->element);
		}

	}
	puts("");
}

Position CreateElement()
{
	Position newElement = (Position)malloc(sizeof(List));

	newElement->element = RandomIntegerInGivenRange(10, 100);
	newElement->next = NULL;

	return newElement;
}
