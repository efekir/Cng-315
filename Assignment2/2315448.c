#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// Efe Kır 2315448


struct Student
{
	char studentID[4];
	char name[41];
	char depCode[4];
};

struct HashTable
{
	struct Student** students;
	int count;
	int size;
};

int key(char* str)
{
	return (str[0] - 65 + str[1] - 48 + str[2] - 48);
}

int h1(int key, int size)
{
	return (2 * key) % size;
}

int h2(int key, int size)
{
	return (7 - (key % 7));
}

struct HashTable* create_table(int size)
{
	// Creates a new HashTable
	int i;
	struct HashTable* table = (struct HashTable*)malloc(sizeof(struct HashTable));
	table->size = size;
	table->count = 0;
	table->students = (struct Student**)calloc(table->size, sizeof(struct Student*));
	for (i = 0; i < table->size; i++)
		table->students[i] = NULL;

	return table;
}

struct HashTable* AddStudent(struct HashTable* table, int method)
{
	// Creates a pointer to a new hash table item
	struct Student* student = (struct Student*)malloc(sizeof(struct Student));

	char studentID[4], name[41], depCode[4];
	int index = 0, i;
	char temp;
	float loadFactor = 0.0;

	printf("Enter the details of the student\n");
	printf("Student id :");
	scanf("%s", studentID);
	printf("Student name :");
	scanf("%c", &temp);
	scanf("%[^\n]", name);
	printf("Department Code :");
	scanf("%s", depCode);

	for (i = 0; i < table->size; i++)
	{
		if ((table->students[i] != NULL && (strcmp(studentID, table->students[i]->studentID)) == 0))
		{
			printf("ID must be unique!\n");
			return table;
		}
		
	}


	strcpy(student->studentID, studentID);
	strcpy(student->name, name);
	strcpy(student->depCode, depCode);

	index = (h1(key(studentID), table->size)) % table->size;

	if (table->students[index] == NULL)
	{
		table->students[index] = student;
		table->count++; // used for calculating load factor
	}
	else
	{
		if (method == 1) // Double hashing
		{
			for (i = 0; i < table->size; i++)
			{
				index = (h1(key(studentID), table->size) + i * h2(key(studentID), table->size)) % table->size;
				if (table->students[index] == NULL)
				{
					table->students[index] = student;
					table->count++;
					break;
				}
			}
		}
		else if (method == 2) // quadratic probing
		{
			for (i = 0; i < table->size; i++)
			{
				index = ((h1(key(studentID), table->size) + (int)(pow(i, 2))) % table->size);
				if (table->students[index] == NULL)
				{
					table->students[index] = student;
					table->count++;
					break;
				}
			}

		}
	}

	// calculating the load factor
	loadFactor = table->count / table->size;

	// rehashign


	if (loadFactor > 0.500)
	{
		printf("Rehasing !\n");
		struct HashTable* newTable = create_table(table->size * 2);

		for (i = 0; i < table->size; i++)
		{
			index = (h1(key(table->students[i]->studentID), table->size)) % table->size;

			if (newTable->students[index] == NULL)
			{
				newTable->students[index] = table->students[i];
				newTable->count++; // used for calculating load factor
			}
			else
			{


				if (method == 1) // Double hashing
				{
					for (i = 0; i < table->size; i++)
					{
						index = (h1(key(table->students[i]->studentID), table->size) + i * h2(key(studentID), table->size)) % table->size;
						if (newTable->students[index] == NULL)
						{
							newTable->students[index] = table->students[i];
							newTable->count++; // used for calculating load factor
							break;
						}
					}
				}
				else if (method == 2) // quadratic probing
				{
					for (i = 0; i < table->size; i++)
					{
						index = ((h1(key(table->students[i]->studentID), table->size) + (int)(pow(i, 2))) % table->size);
						if (newTable->students[index] == NULL)
						{
							newTable->students[index] = table->students[i];
							newTable->count++; // used for calculating load factor
							break;
						}
					}

				}
			}
		}
		return newTable;
		

	}
	else
		return table;





}

void printTable(struct HashTable* table)
{
	int i;
	printf("\n-------------------\n");
	printf("Index\tID\tName\tDepartment\n");
	for (i = 0; i < table->size; i++)
	{
		if (table->students[i])
		{
			printf("%d\t%s\t%s\t%s\n", i, table->students[i]->studentID, table->students[i]->name, table->students[i]->depCode);
		}
		else
		{
			printf("%d\n", i);
		}
	}
	printf("-------------------\n\n");
}

void searchStudent(struct HashTable* table, int method)
{

	char studentID[4];
	int index;
	int i;

	printf("Enter Student ID to be searched :");
	//scanf("%c", &temp);
	scanf("%s", studentID);

	index = (h1(key(studentID), table->size)) % table->size;

	if (table->students[index] == NULL)
	{
		printf("Student can not be found !\n");
		return;
	}

	else
	{
		if (method == 1) // Double hashing
		{
			for (i = 0; i < table->size; i++)
			{
				index = (h1(key(studentID), table->size) + i * h2(key(studentID), table->size)) % table->size;
				if (table->students[index] == NULL)
				{
					printf("Student can not be found !\n");
					return;
				}
				else if (!(strcmp(table->students[index]->studentID, studentID)))
				{
					printf("Student ID : %s\n", table->students[index]->studentID);
					printf("Student name : %s\n", table->students[index]->name);
					printf("Student department : %s\n", table->students[index]->depCode);
					return;
				}
			}
		}
		else if (method == 2) // quadratic probing
		{
			for (i = 0; i < table->size; i++)
			{
				index = ((h1(key(studentID), table->size) + (int)(pow(i, 2))) % table->size);
				if (table->students[index] == NULL)
				{
					
					printf("Student can not be found !\n");
					return;
				}
				else if (!(strcmp(table->students[index]->studentID, studentID)))
				{
					printf("Student ID : %s\n", table->students[index]->studentID);
					printf("Student name : %s\n", table->students[index]->name);
					printf("Student department : %s\n", table->students[index]->depCode);
					return;
				}
			}

		}
	}


}

int main()
{

	int choice, method;

	struct HashTable* table = create_table(11);
	printf("Chose your hashing method. (1) Double Hashing (2) Quadratic Probing : ");
	scanf("%d", &method);

	while (1)
	{
		printf("1. Add a student.\n2. Search for a student using ID.\n3. Print Table.\n4. Exit.\n");
		scanf("%d", &choice);

		if (choice == 1)
		{
			table = AddStudent(table, method);
		}
		else if (choice == 2)
		{
			searchStudent(table, method);
		}
		else if (choice == 3)
		{
			printTable(table);
		}
		else if (choice == 4)
		{
			return 0;
		}
		else
			printf("Wrong input \n");

	}



	free(table);
	return 0;
}

