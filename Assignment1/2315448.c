// Efe Kır 2315448
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <math.h>

struct Champion
{
	char name[20];
	float expectedWinRate;
	float numberOfBattles;
	float numberOfWins;
	float actualWinRate;
	float expectationSkew;
};

void swap(struct Champion* e, struct Champion* f);
void heapify(struct Champion  arr[], int n, int i, char *criteria);
void heapSort(struct Champion arr[], int size, char *choice);
struct Champion* initializeChampions(char filename[20], int* size);
void getBattleData(struct Champion arr[], char filename[20],int size);
void printLeaderboard(struct Champion arr[], int size);
void computeWinRate(struct Champion arr[], int size);

int main(int argc, char* argv[])
{
	// skipping one argument because the first var is my path
	if (argc < 4)
	{
		printf("Too few arguments");
		exit(1);
	}
	if (argc > 4)
	{
		printf("Too many arguments");
		exit(1);
	}
	

	int size;
	size = 0;

	printf("%s %s %s\n", argv[1], argv[2], argv[3]);

	printf("Cng 315 Assignment 1\n\n");
	struct Champion* champArr;

	champArr = initializeChampions(argv[2],&size);
	getBattleData(champArr, argv[3],size);
	computeWinRate(champArr, size);
	heapSort(champArr, size,argv[1]);
	printLeaderboard(champArr,size);
	
	free(champArr);
}


void swap(struct Champion* e, struct Champion* f)
{
	struct Champion temp = *e;
	*e = *f;
	*f = temp;
}


void heapify(struct Champion  arr[], int n, int i,char *criteria)
{
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	// actual winrate
	
	if (!(strcmp(criteria,"1")))
	{
		if (left < n && arr[left].actualWinRate < arr[largest].actualWinRate)
			largest = left;

		if (right < n && arr[right].actualWinRate < arr[largest].actualWinRate)
			largest = right;

	}
	// expected winrate
	if (!(strcmp(criteria, "2")))
	{
		if (left < n && arr[left].expectedWinRate < arr[largest].expectedWinRate)
			largest = left;

		if (right < n && arr[right].expectedWinRate < arr[largest].expectedWinRate)
			largest = right;

	}
	//expectation skew
	if (!(strcmp(criteria, "3")))
	{
		if (left < n && arr[left].expectationSkew < arr[largest].expectationSkew)
			largest = left;

		if (right < n && arr[right].expectationSkew < arr[largest].expectationSkew)
			largest = right;

	}


	if (largest != i)
	{
		swap(&arr[i], &arr[largest]);
		heapify(arr, n, largest, criteria);
	}
}

void heapSort(struct Champion arr[], int size,char *choice)
{
	for (int i = size / 2 - 1; i >= 0; i--)
		heapify(arr, size, i,choice);
	for (int i = size - 1; i >= 0; i--) {
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0,choice);
	}
}

struct Champion* initializeChampions(char filename[20], int *size)
{
	
	int lines = 0;
	char ch;

	struct Champion* champArr;

	FILE* myFile;
	myFile = fopen(filename, "r");

	while ((ch = fgetc(myFile)) != EOF)
	{
		if (ch == '\n')
		{
			lines++;
		}
	}
	rewind(myFile);
	
	*size = lines+1;

	champArr = (struct Champion*)malloc((lines + 1) * sizeof(struct Champion)); // lines + 1 is because there is no \n at the end of the file

	for (int i = 0; i < lines+1; i++)
	{
		fscanf(myFile, "%s %f\n", champArr[i].name, &champArr[i].expectedWinRate);
		//printf("%s %.2f\n", champArr[i].name, champArr[i].expectedWinRate);   // to check loading of file
	}
	fclose(myFile);
	return champArr;
	free(champArr);
}

void getBattleData(struct Champion arr[],char filename[20], int size)
{
	int lines = 0;
	char ch;
	char tmp1[10], tmp2[10], tmp3[10], tmp4[10];
	int k = 0;
	FILE* myFile;
	myFile = fopen(filename, "r");

	while ((ch = fgetc(myFile)) != EOF)
	{
		if (ch == '\n')
		{
			lines++;
		}
	}
	rewind(myFile);
	lines++;

	for (int i = 0; i < size; i++)
	{
		arr[i].numberOfBattles = 0;
		arr[i].numberOfWins = 0;
	}


	for (int i = 0; i < lines; i++)
	{
		fscanf(myFile, "%s %s %s %s\n", tmp1, tmp2, tmp3, tmp4);
		for (int j = 0; j < size; j++)
		{
			if (!(strcmp(tmp2, arr[j].name)))
			{
				arr[j].numberOfBattles = arr[j].numberOfBattles + 1;
			}

		}
		for (int j = 0; j < size; j++)
		{
			if (!(strcmp(tmp3, arr[j].name)))
			{
				
				arr[j].numberOfBattles = arr[j].numberOfBattles + 1;
			}

		}
		for (int j = 0; j < size; j++)
		{
			if (!(strcmp(tmp4, arr[j].name)))
			{
				arr[j].numberOfWins = arr[j].numberOfWins + 1;
			}

		}
	}

	fclose(myFile);
}
void printLeaderboard(struct Champion arr[],int size)
{

	printf("Champion\tBattles\tWin\tAWR\tEWR\tSkew\n");
	
	for (int i = 0; i < size; i++)
	{
		printf("%s\t\t%0.0f\t%0.0f\t%.2f\t%.2f\t%.2f\n", arr[i].name, arr[i].numberOfBattles, arr[i].numberOfWins, arr[i].actualWinRate, arr[i].expectedWinRate, arr[i].expectationSkew);
	}

}

void computeWinRate(Champion arr[], int size)
{

	for (int i = 0; i < size; i++)
	{
		float wrr = 0.0;
		arr[i].actualWinRate = arr[i].numberOfWins / arr[i].numberOfBattles;
		wrr = arr[i].actualWinRate / arr[i].expectedWinRate;
		arr[i].expectationSkew = fabs(wrr - 1.0);
	}
}