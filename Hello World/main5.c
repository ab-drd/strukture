#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_FILENAME 256
#define MAX_CHAR 256

struct list; typedef struct list* Position; typedef struct list List;
struct list {
	int element;
	Position next;
};

int File_Number_Lines(char*);
int ReadFromFile(Position, char*, int);
void Insert_Sort(Position, Position);
void Insert_After(Position, Position);
void Insert_End(Position, Position);
void Print_List(Position, int);
Position MakeNew(int);
Position Set_Intersection(Position, Position);
Position Set_Union(Position, Position);
Position Find_Last(Position);

int main() 
{
	char filename[MAX_CHAR];
	int line_number = 0, chosen_number = 0, i = 0, operation_yesno = 0, 
		operation_number = 0, operation_choice = 0, _choice_1 = 0, _choice_2 = 0;
	Position heads;
	
	printf("========== Dobro dosli na pocetak programa! ==========\nUnesite ime datoteke iz koje citate: ");
	scanf(" %s", filename);
	line_number = File_Number_Lines(filename);
	if (line_number == -1) {
		printf("Nije pronadjena datoteka tog imena!\nKraj programa");
		return -2;
	}
	else if (line_number < 2) {
		printf("Imate manje od 2 liste u datoteci! Molimo promijenite svoju datoteku!\nKraj programa");
		return -1;
	}

	printf("Detektirano je %d lista. Koliko ih zelite ucitati? (minimalno 2)\n", line_number);
	scanf("%d", &chosen_number);
	while (1) {
		if (chosen_number < 2) {
			printf("Ne mozete ucitati manje od 2 liste! Molimo ponovno unesite koliko lista zelite ucitati:\n");
			scanf("%d", &chosen_number);
		}
		else if (chosen_number > line_number) {
			printf("Ne mozete ucitati vise od %d listi! Molimo ponovno unesite koliko lista zelite ucitati:\n", line_number);
			scanf("%d", &chosen_number);
		}
		else {
			break;
		}
	}
	heads = (Position)malloc(chosen_number * sizeof(List));
	for (i = 0; i < chosen_number; i++) {
		(heads[i]).next = NULL;
	}
	ReadFromFile(heads, filename, chosen_number);
	puts("=======================================");
	printf("Ovo je vas ispis:\n\n");
	for (i = 0; i < chosen_number; i++) {
		Print_List(heads + i, i);
	}
	puts("=======================================");
	printf("\nZelite li uci u menu operacija?\n(1) Da\n(2) Ne (->zavrsetak programa)\n");
	scanf("%d", &operation_yesno);
	if (operation_yesno == 1) {
		puts("=======================================");
		printf("\n**************	Usli ste u menu operacija.	**************\n\n");
		while (1) {
			puts("=======================================");
			printf("%d OPERACIJA || VASE LISTE:\n", operation_number);
			for (i = 0; i < chosen_number; i++) {
				Print_List(heads + i, i);
			}
			puts("=======================================");
			printf("\nMolimo odaberite operaciju:\n(0) za izlaz\n(1) Unija lista\n(2) Presjek lista\n");
			scanf("%d", &operation_choice);
			puts("=======================================");
			if (operation_choice == 0) {
				printf("Izlaz iz programa.\n");
				break;
			}
			else if (operation_choice == 1) {
				if (chosen_number == 2) {
					Print_List(Set_Union(heads, heads + 1), -1);
				}
				else {
					printf("\nUneseno je vise od 2 liste. Molimo odaberite dvije nad kojima zelite napraviti uniju (0 - %d)\n", chosen_number - 1);
					printf("Unesite index prve liste:\n");
					scanf("%d", &_choice_1);
					printf("Unesite index druge liste:\n");
					scanf("%d", &_choice_2);

					Print_List(Set_Union(heads + _choice_1, heads + _choice_2), -1);
				}
			}
			else if (operation_choice == 2) {
				if (chosen_number == 2) {
					Print_List(Set_Intersection(heads, heads + 1), -2);
				}
				else {
					printf("\nUneseno je vise od 2 liste. Molimo odaberite dvije nad kojima zelite napraviti presjek (0 - %d)\n", chosen_number - 1);
					printf("Unesite index prve liste:\n");
					scanf("%d", &_choice_1);
					printf("Unesite index druge liste:\n");
					scanf("%d", &_choice_2);

					Print_List(Set_Intersection(heads + _choice_1, heads + _choice_2), -2);
				}
			}
			else {
				printf("Netocna opcija - povratak na pocetak menu-a\n");
			}
			if (operation_choice > 0 && operation_choice < 3) {
				operation_number++;
			}
			puts(" ");
		}
	}

	return 0;
}

int File_Number_Lines(char* filename)
{
	int count = 0;
	char c;
	FILE* file;
	file = fopen(filename, "r");

	if (file == NULL) {
		printf("ERROR OPENING FILE\n");
		return -1;
	}

	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}

	fclose(file);
	return count;
}

int ReadFromFile(Position Head, char* ffilename, int number_of_lines)
{
	int felement, line_num = 0, e = 0, line_length = 0, total = 0;
	char line[MAX_CHAR]; char* temporary;
	FILE* file;
	file = fopen(ffilename, "r");
	if (file == NULL) {
		printf("ERROR OPENING FILE\n");
		return -1;
	}

	for (line_num; line_num < number_of_lines; line_num++) {
		total = 0;

		fgets(line, sizeof line, file);
		line_length = strlen(line);

		temporary = (char*)malloc(sizeof(line));
		strcpy(temporary, line);

		sscanf(temporary, "%d %n", &felement, &e);

		while (total < line_length) {
			Insert_Sort(Head + line_num, MakeNew(felement));
			temporary += e;
			total += e;

			sscanf(temporary, "%d %n", &felement, &e);
		}
	}

	fclose(file);
	return 0;
}

void Insert_Sort(Position Head, Position P)
{
	Position Q = Head->next, Previous = Head;
	while (Q != NULL) {
		if (P->element < Q->element) {
			P->next = Q;
			Previous->next = P;
			break;
		}
		
		Previous = Previous->next;
		Q = Q->next;
	}
	if (Q == NULL) {
		Insert_After(Previous, P);
	}

}

void Insert_After(Position after_P, Position after_Q)
{
	after_Q->next = after_P->next;
	after_P->next = after_Q;
}

void Insert_End(Position Head, Position end_P)
{
	Position end_Q = Find_Last(Head);
	end_Q->next = end_P;
	end_P->next = NULL;
}

void Print_List(Position Head, int index)
{
	Position P = Head->next;
	if (index == -1) {
		puts("ISPIS REZULTATA UNIJE:");
	}
	else if (index == -2) {
		puts("ISPIS REZULTATA PRESJEKA:");
	}
	else {
		printf("ISPIS %d. LISTE:\n", index);
	}

	while (P != NULL) {
		printf("%d (%d)	  ", P->element, P);
		P = P->next;
	}
	puts("\n");
}

Position MakeNew(int make_element)
{
	Position Made;
	Made = (Position)malloc(sizeof(List));

	Made->element = make_element;
	Made->next = NULL;

	return Made;
}

Position Set_Intersection(Position P1, Position P2)
{
	Position Result;
	Result = (Position)malloc(sizeof(List));
	Result->next = NULL;
	P1 = P1->next; P2 = P2->next;

	while (P1 != NULL && P2 != NULL) {
		if (P1->element == P2->element) {
			Insert_End(Result, MakeNew(P1->element));
			P1 = P1->next; P2 = P2->next;
		}
		else if (P1->element < P2->element) {
			P1 = P1->next;
		}
		else {
			P2 = P2->next;
		}
	}

	return Result;
}

Position Set_Union(Position P1, Position P2)
{
	Position Result;
	Result = (Position)malloc(sizeof(List));
	Result->next = NULL;
	P1 = P1->next; P2 = P2->next;

	while (P1 != NULL && P2 != NULL) {
		if (P1->element < P2->element) {
			Insert_End(Result, MakeNew(P1->element));
			P1 = P1->next;
		}
		else if (P1->element == P2->element) {
			Insert_End(Result, MakeNew(P1->element));
			P1 = P1->next; P2 = P2->next;
		}
		else{
			Insert_End(Result, MakeNew(P2->element));
			P2 = P2->next;
		}
	}
	if (P2 != NULL && P1 == NULL) {
		while (P2 != NULL) {
			Insert_End(Result, MakeNew(P2->element));
			P2 = P2->next;
		}
	}
	else if (P1 != NULL && P2 == NULL) {
		while (P1 != NULL) {
			Insert_End(Result, MakeNew(P1->element));
			P1 = P1->next;
		}
	}

	return Result;
}

Position Find_Last(Position Head)
{
	Position Found_Last = Head;
	while (Found_Last->next != NULL) {
		Found_Last = Found_Last->next;
	}
	return Found_Last;
}
