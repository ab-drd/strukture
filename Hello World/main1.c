#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_FILE_NAME 100

typedef struct {

	char ime[15];
	char prezime[20];
	int bodovi;

} studenti;

int Redak(char* filename) 
{

	int count = 0;
	char c;
	FILE* dat;

	dat = fopen(filename, "r");
	if (dat == NULL) {
		printf("ERROR OPENING FILE");
		return -1;
	}

	while ((c = fgetc(dat)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}

	fclose(dat);
	return count;
}

int Ucitavanje(char* filename, studenti* baza)
{

	int i = 0;

	FILE* dat;
	dat = fopen(filename, "r");
	if (dat == NULL) {
		printf("ERROR OPENING FILE");
		return -1;
	}

	while (fscanf(dat, " %s %s %d", baza[i].ime, baza[i].prezime, &(baza[i].bodovi)) != EOF) {
		i++;
	}

	fclose(dat);
	return 0;
}

int Ispis(char* filename, studenti* baza, int br) 
{
	int i, aps, max_bod = 50;

	FILE* dat;
	dat = fopen(filename, "r");
	if (dat == NULL) {
		printf("ERROR OPENING FILE");
		return -1;
	}

	for (i = 0; i < br; i++) {
		printf("Ime: %s\nPrezime: %s\nBroj bodova: %d\nRelativni broj bodova: %.2f%% \n", baza[i].ime, baza[i].prezime, baza[i].bodovi, (float) (baza[i].bodovi) / max_bod * 100);
		puts("=================");
	}

	fclose(dat);
	return 0;
}

int main() 
{
	
	studenti* baza;
	char filename[MAX_FILE_NAME];
	int brRed;

	scanf("%s", filename);

	brRed = Redak(filename);

	baza = (studenti*)malloc(brRed * sizeof(studenti));

	Ucitavanje(filename, baza);
	Ispis(filename, baza, brRed);
	

	return 0;
}