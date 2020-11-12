#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_FILENAME 256
#define MAX_CHAR 256

struct polynomial;
typedef struct polynomial* Position;
typedef struct polynomial Poly;

struct polynomial {
	int coeff;
	int exp;
	Position next;
};

int ReadFromList(Position, char*, int);
int File_Number_Lines(char*);
void Insert_Sort(Position, Position);
void Insert_After(Position, Position);
void Insert_Before(Position, Position, Position);
void Insert_End(Position, Position);
void Print_List(Position, int);
Position MakeNew(int, int);
Position SumOfTwo(Position, Position);
Position ProductOfTwo(Position, Position);
Position Find_Before(Position, Position);
Position Find_Last(Position);
Position Find_Exponent(Position, int);

int main()
{
	char filename[MAX_FILENAME];
	int lines = 0, i = 0, choice_line = 0, bad_choice = 0, operation_yesno = 0, operation_number = 0, 
		operation_choice = 0, _choice_1 = 0, _choice_2 = 0;

	Poly* heads;
	Poly result; result.next = NULL;

	printf("Dobrodosli na pocetak programa. Za pocetak, molimo unesite datoteku iz koje zelite citati polinome:\n");
	scanf(" %s", filename);
	lines = File_Number_Lines(filename);
	printf("Detektirano je %d polinoma u danoj datoteci. Koliko ih zelite ucitati u liste?:\n", lines);
	scanf("%d", &choice_line);
	while (1) {
		if (bad_choice > 2) {
			printf("Ucitali ste nesto nemoguce vec %d puta! Citajte malo!\n", bad_choice);
		}
		if (choice_line > lines || choice_line < 1) {
			if (choice_line > lines) {
				printf("Ne mozete ucitati vise polinoma nego sto ih je u datoteci! ");
			}
			else if (choice_line == 0) {
				printf("Ne mozete odabrati 0 polinoma! ");
			}
			else if (choice_line < 0) {
				printf("Ne mozete odabrati negativan broj polinoma! ");
			}
			printf("Koliko polinoma zelite ucitati?\n");
			scanf("%d", &choice_line);
			bad_choice++;
		}
		else {
			printf("Odlucili ste ucitati %d od %d polinoma. Slijedi ucitavanje...\n\n\r", choice_line, lines);
			break;
		}
	}

	heads = (Poly*)malloc(choice_line*sizeof(Poly));
	for (i = 0; i < choice_line; i++) {
		(heads[i]).next = NULL;
	}
	ReadFromList(heads, filename, choice_line);

	puts("=======================================");
	printf("Ovo je vas ispis:\n");
	for (i = 0; i < choice_line; i++) {
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
			printf("%d OPERACIJA || VASI POLINOMI:\n", operation_number);
			for (i = 0; i < choice_line; i++) {
				Print_List(heads + i, i);
			}
			puts("=======================================");
			printf("\nMolimo odaberite operaciju:\n(0) za izlaz\n(1) Zbrajanje polinoma\n(2) Mnozenje polinoma\n");
			scanf("%d", &operation_choice);
			if (operation_choice == 0) {
				printf("Izlaz iz programa.\n");
				break;
			}
			else if (operation_choice == 1) {
				if (choice_line == 1) {
					printf("Slijedi zbrajanje polinoma sa samim sobom...\n");
					Print_List(SumOfTwo(heads, heads), -1);
				}
				else if (choice_line == 2) {
					printf("Slijedi zbrajanje izabranih polinoma...\n");
					Print_List(SumOfTwo(heads, heads+1), -1);
				}
				else {
					printf("\nUneseno je vise od 2 polinoma. Molimo odaberite 2 koja zelite zbrojiti (0 - %d)\n", choice_line-1);
					printf("Unesite prvi polinom:\n");
					scanf("%d", &_choice_1);
					printf("Unesite drugi polinom:\n");
					scanf("%d", &_choice_2);
					Print_List(SumOfTwo(heads + _choice_1, heads + _choice_2), -1);
				}
			}
			else if (operation_choice == 2) {
				if (choice_line == 1) {
					printf("Slijedi mnozenje polinoma sa samim sobom...\n");
					Print_List(ProductOfTwo(heads, heads), -1);
				}
				else if (choice_line == 2) {
					printf("Slijedi mnozenje izabranih polinoma...\n");
					Print_List(ProductOfTwo(heads, heads+1), -1);
				}
				else {
					printf("Uneseno je vise od 2 polinoma. Molimo odaberite 2 koja zelite pomnoziti (0 - %d)\n", choice_line - 1);
					printf("Unesite prvi polinom:\n");
					scanf("%d", &_choice_1);
					printf("Unesite drugi polinom:\n");
					scanf("%d", &_choice_2);
					Print_List(ProductOfTwo(heads + _choice_1, heads + _choice_2), -1);
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

int ReadFromList(Position Head, char* ffilename, int number_of_lines)
{
	int fcoeff = 0, fexp = 0, line_num = 0, e = 0, line_length = 0, total = 0;
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
		
		sscanf(temporary, "%dx%d %n", &fcoeff, &fexp, &e);

		while (total < line_length) {
			if (fcoeff != 0) {
				Insert_Sort(Head + line_num, MakeNew(fcoeff, fexp));
			}
			temporary += e;
			total += e;

			sscanf(temporary, "%dx%d %n", &fcoeff, &fexp, &e); 
		}
	}

	fclose(file);
	return 0;
}

int File_Number_Lines(char* filename) 
{
	int count = 0;
	char c;
	FILE* file;
	file = fopen(filename, "r");

	if (file == NULL) {
		printf("ERROR OPENING FILE");
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

void Insert_Sort(Position Head, Position P)
{
	Position Q = Head->next, Previous = Head;
	int check = 0;
	while (Q != NULL) {
		if (P->exp < Q->exp) {
			P->next = Q;
			Previous->next = P;
			break;
		}
		else if (P->exp == Q->exp) {
			if (P->coeff + Q->coeff != 0) {
				Q->coeff += P->coeff;
				break;
			}
			else {
				Previous->next = Q->next;
				free(Q);
				check++;
				break;
			}
		}
		Previous = Previous->next;
		Q = Q->next;
	}
	if (check == 0 && Q == NULL) {
		Insert_After(Previous, P);
	}
	
}

void Insert_After(Position after_P, Position after_Q)
{
	after_Q->next = after_P->next;
	after_P->next = after_Q;
}

void Insert_Before(Position Head, Position before_P, Position before_Q)
{
	Position R = Find_Before(Head, before_P);
	if (R == NULL) {
		return;
	}
	before_Q->next = before_P;
	R->next = before_Q;
}

void Insert_End(Position Head, Position end_P) 
{
	Position end_Q = Find_Last(Head);
	end_Q->next = end_P;
	end_P->next = NULL;
}

void Print_List(Position Head, int number) {
	Position Current = Head->next;
	puts(" ");
	if (number == -1) {
		puts("ISPIS REZULTATA:");
	}
	else {
		printf("ISPIS %d:\n", number);
	}
	for (Current; Current != NULL; Current = Current->next) {
		printf("%d*x^%d (%d)	", Current->coeff, Current->exp, Current);
	}
	puts(" ");
}

Position MakeNew(int make_coeff, int make_exp)
{
	Position Made;
	Made = (Position)malloc(sizeof(Poly));

	Made->coeff = make_coeff;
	Made->exp = make_exp;
	Made->next = NULL;

	return Made;
}

Position SumOfTwo(Position P1, Position P2) 
{
	Position Result;
	Result = (Position)malloc(sizeof(Poly));
	Result->next = NULL;

	P1 = P1->next; P2 = P2->next;

	while (P1 != NULL && P2 != NULL) {
		if (P1->exp == P2->exp) {
			if (P1->coeff + P2->coeff != 0) {
				Insert_End(Result, MakeNew(P1->coeff + P2->coeff, P1->exp));
			}
			P1 = P1->next; P2 = P2->next;
		}
		else if (P1->exp < P2->exp) {
			Insert_End(Result, MakeNew(P1->coeff, P1->exp));
			P1 = P1->next;
		}
		else {
			Insert_End(Result, MakeNew(P2->coeff, P2->exp));
			P2 = P2->next;
		}
	}
	if (P1 == NULL) {
		while (P2 != NULL) {
			Insert_End(Result, MakeNew(P2->coeff, P2->exp));
			P2 = P2->next;
		}
	}
	else if (P2 == NULL) {
		while (P1 != NULL) {
			Insert_End(Result, MakeNew(P1->coeff, P1->exp));
			P1 = P1->next;
		}
	}

	return Result;
}

Position ProductOfTwo(Position P1, Position P2) 
{
	Position Result, Poly1 = P1->next, Poly2 = P2->next, Exponent = NULL, Before = NULL;
	Result = (Position)malloc(sizeof(Poly));
	Result->next = NULL;

	while (Poly1 != NULL) {
		while (Poly2 != NULL) {
			if (Result->next == NULL) {
				Insert_After(Result, MakeNew(Poly1->coeff * Poly2->coeff, Poly1->exp + Poly2->exp));
			}
			else {
				Exponent = Find_Exponent(Result, Poly1->exp + Poly2->exp);
				if (Exponent == NULL) {
					Insert_End(Result, MakeNew(Poly1->coeff * Poly2->coeff, Poly1->exp + Poly2->exp));
				}
				else {
					if (Exponent->coeff + Poly1->coeff * Poly2->coeff == 0) {
						Before = Find_Before(Result, Exponent);
						Before->next = Exponent->next;
						free(Exponent);
					}
					else {
						Exponent->coeff += Poly1->coeff * Poly2->coeff;
					}
				}
			}
			Poly2 = Poly2->next;
		}
		Poly2 = P2->next;
		Poly1 = Poly1->next;
	}

	return Result;

}

Position Find_Before(Position Head, Position before_this)
{
	Position Found_Before = Head->next;
	while (Found_Before != NULL) {
		if (Found_Before->next == before_this) {
			break;
		}
		Found_Before = Found_Before->next;
	}
	if (Found_Before == NULL) {
		printf("Nije pronadjen argument before_this");
	}

	return Found_Before;
}

Position Find_Last(Position Head) 
{
	Position Found_Last = Head;
	while (Found_Last->next != NULL) {
		Found_Last = Found_Last->next;
	}
	return Found_Last;
}

Position Find_Exponent(Position Head, int find) 
{
	Position Exponent = Head->next;
	while (Exponent != NULL && Exponent->exp != find) {
		Exponent = Exponent->next;
	}

	return Exponent;
}