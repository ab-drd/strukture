#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_CHAR 256

struct osoba;
typedef struct osoba OSOBA;
typedef struct osoba* Pozicija;
struct osoba {

	char ime[MAX_CHAR];
	char prezime[MAX_CHAR];
	int godina;

	Pozicija next;
};

Pozicija MakeNew(char*, char*, int);
Pozicija FindLast(Pozicija);
Pozicija FindSurname(Pozicija, char*);
Pozicija FindSurname_One(Pozicija, char*);
Pozicija FindSurname_Name(Pozicija, char*);
void InsertSort(Pozicija);
void InsertStart(Pozicija, Pozicija);
void InsertEnd(Pozicija, Pozicija);
void Dynamic_InsertBefore_Surname(Pozicija, Pozicija, char*);
void Dynamic_InsertAfter_Surname(Pozicija, Pozicija, char*);
void Dynamic_InsertBefore_Number(Pozicija, Pozicija, int);
void Dynamic_InsertAfter_Number(Pozicija, Pozicija, int);
void Print(Pozicija);
void RemoveElement(Pozicija, Pozicija);
int ReadFromFile(Pozicija, char*);
int WriteIntoFile(Pozicija, char*);
int HowManySurname(Pozicija, char*);
int HowManyElements(Pozicija);

int main()
{
	char rime[MAX_CHAR], rprezime[MAX_CHAR], filename[MAX_CHAR];
	int rgodina = 0, counter = 1, 
		choose = 0, choose_dynamic = 0, 
		choose_before_after = 0, n_element = 0, elements = 0;

	OSOBA head;
	head.next = NULL;

	Pozicija p;
	
	while (1) {
		printf("%d. iteracija | %d elemenata u listi\n	Izaberite opciju:\nIzlaz iz programa (0)\nSortirani unos (1)\nUnos iz datoteke (2)\nDodavanje nove osobe na pocetak liste (3)", counter, elements);
		puts("\nDodavanje nove osobe na kraj liste (4)\nMenu dinamicke alokacije(5)\nBrisanje iz liste po prezimenu(6)\nIspis u datoteku (7)");
		scanf("%d", &choose);

		if (choose == 0) {
			break;
		}
		else if (choose == 1) {
			if (elements != 0) {
				puts("Sortirani unos je moguc samo kada imate 0 elemenata u listi!");
			}
			else {
				InsertSort(&head);
			}
		}
		else if (choose == 2) {
			puts("Unesite ime datoteke iz koje zelite ispisivati:");
			scanf(" %s", filename);
			ReadFromFile(&head, filename);
		}
		else if (choose == 3 || choose == 4) {
			puts("Molimo unesite osobu u formatu (ime) (prezime) (godina rodjenja): ");
			scanf(" %s %s %d", rime, rprezime, &rgodina);
			p = MakeNew(rime, rprezime, rgodina);
			if (choose == 1) {
				InsertStart(&head, p);
			}
			else {
				InsertEnd(&head, p);
			}
		}
		else if (choose == 5) {
			printf("Vas je ispis trenutno: ");
			Print(&head);
			printf("Molimo odaberite nacin biranja elementa\n1 za biranje po prezimenu\n2 za biranje po rednom broju elementa po trenutnom izgledu liste (od 0)\n");
			scanf("%d", &choose_dynamic);
			printf("Molimo odaberite gdje zelite dodati element\nIspred odredenog elementa(1)\nIza odredenog elementa(2)\n");
			scanf("%d", &choose_before_after);
			puts("Molimo unesite osobu u formatu (ime) (prezime) (godina rodjenja): ");
			scanf(" %s %s %d", rime, rprezime, &rgodina);
			p = MakeNew(rime, rprezime, rgodina);

			if (choose_dynamic == 1) {
				printf("Molimo unesite prezime osobe nakon/prije koje zelite unijeti element: ");
				scanf(" %s", rprezime);
				if (choose_before_after == 1) {
					Dynamic_InsertBefore_Surname(&head, p, rprezime);
				}
				else if (choose_before_after == 2) {
					Dynamic_InsertAfter_Surname(&head, p, rprezime);
				}
				else {
					puts("Neispravan unos!");
				}
			}
			else if (choose_dynamic == 2) {
				printf("Molimo unesite redni broj elementa po trenutnom izgledu liste (od 0): ");
				scanf("%d", &n_element);
				if (HowManyElements(&head) < n_element) {
					puts("Odabran prevelik redni broj elementa!");
				}
				else if (choose_before_after == 1) {
					Dynamic_InsertBefore_Number(&head, p, n_element);
					
				}
				else if (choose_before_after == 2) {
					Dynamic_InsertAfter_Number(&head, p, n_element);
				}
				else {
					puts("Neispravan unos!");
				}
			}
		}
		else if (choose == 6) {
			if (elements != 0) {
				puts("Molimo unesite prezime osobe koju zelite izbrisati: ");
				scanf(" %s", rprezime);

				p = FindSurname(&head, rprezime);
				RemoveElement(&head, p);
			}
			else {
				puts("Nemoguce izbrisati element ako je broj elemenata 0!");
			}
		}
		else if (choose == 7) {
			puts("Unesite ime datoteke u koju zelite upisivati:");
			scanf(" %s", filename);
			WriteIntoFile(&head, filename);
		}
		else {
			puts("Neispravna opcija!");
		}
		Print(&head);
		puts("=======================================");
		choose = -1;
		counter++;
		elements = HowManyElements(&head);
	}

	return 0;
}

Pozicija MakeNew(char* nime, char* nprezime, int ngodina)
{
	Pozicija pq;
	pq = (Pozicija)malloc(sizeof(struct osoba));
	strcpy(pq->ime, nime);
	strcpy(pq->prezime, nprezime);
	pq->godina = ngodina;
	pq->next = NULL;

	return pq;
}

Pozicija FindLast(Pozicija pHead)
{
	Pozicija last = pHead;
	while (last->next != NULL) {
		last = last->next;
	}
	return last;
}

Pozicija FindSurname(Pozicija pHead, char* pprez)
{
	Pozicija find = NULL, P = pHead->next;
	int n = HowManySurname(pHead, pprez), fchoose = 0;

	if (n == 1) {
		find = FindSurname_One(pHead, pprez);
	}
	else if (n > 1) {
		printf("Pronadjeno je vise od jedne osobe s tim prezimenom. Odaberite opciju:\n1 za uzimanje prvog prezimena u listi\n2 za uzimanje osobe s prvim imenom po abecednom redoslijedu:\n");
		scanf("%d", &fchoose);
		if (fchoose == 1) {
			find = FindSurname_One(pHead, pprez);
		}
		else if (fchoose == 2) {
			find = FindSurname_Name(pHead, pprez);
		}
		else {
			puts("Neispravan unos!");
		}
	}
	
	if (find == NULL) {
		puts("SURNAME NOT FOUND");
	}

	return find;

}

Pozicija FindSurname_One(Pozicija pHead, char* pprez_o) 
{
	Pozicija ffind = NULL, P = pHead->next;

	while (1) {
		if (!strcmp(pprez_o, P->prezime)) {

			ffind = P;
			break;
		}
		P = P->next;
	}
	return ffind;
}

Pozicija FindSurname_Name(Pozicija pHead, char* pprez) 
{
	Pozicija P = pHead->next;
	Pozicija pReturn = FindSurname_One(pHead, pprez);

	while (P != NULL) {
		if (!strcmp(pprez, P->prezime) && strcmp(pReturn->ime, P->ime) > 0) {
			pReturn = P;
		}

		P = P->next;
	}

	return pReturn;
}

void InsertSort(Pozicija pHead) 
{
	int insert_number = 1, insert_godina = 0, insert_choose = 0;
	char insert_ime[MAX_CHAR], insert_prezime[MAX_CHAR];
	Pozicija Q = NULL, P = NULL, prev = NULL;

	while (1) {
		printf("Insert %d || Zelite li nastaviti unos?\n0 za break\n1 za nastavak\n", insert_number);
		scanf("%d", &insert_choose);
		if (insert_choose == 0) {
			break;
		}
		else if (insert_choose == 1) {
			printf("Molimo unesite %d. osobu u formatu (ime) (prezime) (godina rodjenja):\n", insert_number);
			scanf(" %s %s %d", insert_ime, insert_prezime, &insert_godina);
			Q = MakeNew(insert_ime, insert_prezime, insert_godina);
			if (insert_number == 1) {
				InsertStart(pHead, Q);
			}
			else {
				P = pHead->next;
				prev = pHead;
				while (P != NULL) {
					if (strcmp(Q->prezime, P->prezime) < 0) {
						Q->next = P;
						prev->next = Q;
						break;
					}
					else if (!strcmp(Q->prezime, P->prezime)) {
						if (strcmp(Q->ime, P->ime) < 0) {
							Q->next = P;
							prev->next = Q;
							break;
						}
						else if (!strcmp(Q->ime, P->ime)) {
							if (Q->godina <= P->godina) {
								Q->next = P;
								prev->next = Q;
								break;
							}
							else {
								Q->next = P->next;
								P->next = Q;
								break;
							}
						}
					}
					P = P->next;
					prev = prev->next;
				}
				if (P == NULL) {
					InsertEnd(pHead, Q);
				}
			}
		}
		insert_number++;
	}
	
	
}

void InsertStart(Pozicija pHead, Pozicija P)
{
	P->next = pHead->next;
	pHead->next = P;
}

void InsertEnd(Pozicija pHead, Pozicija P)
{
	Pozicija q = FindLast(pHead);
	q->next = P;
	P->next = NULL;
}

void Dynamic_InsertBefore_Surname(Pozicija pHead, Pozicija Q, char* pprez) 
{
	Pozicija P = pHead->next, 
		     surname = FindSurname(pHead, pprez);
	if (surname != NULL) {
		while (P->next != surname) {
			P = P->next;
		}
		Q->next = surname;
		P->next = Q;
	}
}

void Dynamic_InsertAfter_Surname(Pozicija pHead, Pozicija Q, char* pprez) 
{
	Pozicija P = pHead->next, surname = FindSurname(pHead, pprez);
	if (surname != NULL) {
		Q->next = surname->next;
		surname->next = Q;
	}
}

void Dynamic_InsertBefore_Number(Pozicija pHead, Pozicija Q, int fnumber)
{
	Pozicija P = pHead->next;
	int goal = fnumber - 1, fcounter = 0;
	if (fnumber == 0) {
		InsertStart(pHead, Q);
	}
	else {
		while (fcounter != goal) {
			P = P->next;
			fcounter++;
		}
		Q->next = P->next;
		P->next = Q;
	}
}
void Dynamic_InsertAfter_Number(Pozicija pHead, Pozicija Q, int fnumber)
{
	Pozicija P = pHead->next;
	int fcounter = 0;
	while (fcounter != fnumber) {
		P = P->next;
		fcounter++;
	}
	Q->next = P->next;
	P->next = Q;

}

void Print(Pozicija pHead)
{
	Pozicija p = pHead->next;
	puts("ISPIS: ");
	for (p; p != NULL; p = p->next) {
		printf("%s %s %d (%d)   ", p->ime, p->prezime, p->godina, p);
	}
	puts(" ");
}

void RemoveElement(Pozicija pHead, Pozicija pRemove)
{
	Pozicija q = pHead;
	if (pRemove != NULL) {
		while (q != NULL && q->next != pRemove) {
			q = q->next;
		}
		q->next = pRemove->next;
		free(pRemove);
	}
}

int ReadFromFile(Pozicija pHead, char* filename) 
{
	Pozicija Q = NULL;
	char read_ime[MAX_CHAR], read_prezime[MAX_CHAR];
	int read_godina;

	FILE* pfile;
	pfile = fopen(filename, "r");
	if (pfile == NULL) {
		printf("ERROR OPENING FILE\n");
		return -1;
	}

	while (fscanf(pfile, " %s %s %d", read_ime, read_prezime, &read_godina) != EOF) {
		Q = MakeNew(read_ime, read_prezime, read_godina);
		InsertEnd(pHead, Q);
	}

	fclose(pfile);
	return 0;
}

int WriteIntoFile(Pozicija pHead, char* filename) 
{
	Pozicija P = pHead->next;

	FILE* pfile;
	pfile = fopen(filename, "w");
	if (pfile == NULL) {
		printf("ERROR OPENING FILE\n");
		return -1;
	}

	while (P != NULL) {
		fprintf(pfile, "%s %s %d\n", P->ime, P->prezime, P->godina);
		P = P->next;
	}

	fclose(pfile);
	return 0;
}

int HowManySurname(Pozicija pHead, char* pprez)
{
	int number = 0;
	Pozicija p = pHead->next;
	for (p; p != NULL; p = p->next) {
		if (!strcmp(pprez, p->prezime)) {
			number++;
		}
	}

	return number;
}

int HowManyElements(Pozicija pHead) 
{
	int elements = 0;
	Pozicija P = pHead->next;

	while (P != NULL) {
		elements++;
		P = P->next;
	}

	return elements;
}