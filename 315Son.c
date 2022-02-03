#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Node
{
    char string[100];
    char newString[100];
    int flag;
    struct Node* next;
};

void robinKarp(char* P, int d, int q, struct Node* tmp, int* count);
struct Node* createScanpathList(char* fileName);
void PrintList(struct Node* head, int count);
void InsertNode(struct Node** head_ref, char* string);


int main()
{
    char pattern[20];
    char file1[30], file2[30];
    int count = 0;

    int q = 11, d = 10;
    struct Node* autism = (struct Node*)malloc(sizeof(struct Node));
    autism->next = NULL;
    struct Node* noAutism = (struct Node*)malloc(sizeof(struct Node));
    noAutism->next = NULL;

    printf("Enter the file name for people with autism:");
    scanf("%s", file1);
    printf("Enter the file name for people without autism:");
    scanf("%s", file2);
    printf("Enter the pattern:");
    scanf("%s", pattern);
    autism = createScanpathList(file1);
    noAutism = createScanpathList(file2);

    autism = autism->next;
    noAutism = noAutism->next;
    robinKarp(pattern, d, q, autism, &count);
    printf("Detected patterns for people with autism:\n\n");
    PrintList(autism,count);
    count = 0;
    printf("Detected patterns for people without autism:\n\n");
    robinKarp(pattern, d, q, noAutism, &count);
    PrintList(noAutism, count);

}
void InsertNode(struct Node** head_ref, char* string)
{
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->flag = 0;
    new_node->newString[0] = '\0';
    new_node->string[0] = '\0';

    strcpy(new_node->string, string);


    if (*head_ref == NULL)
    {
        (*head_ref) = (struct Node*)malloc(sizeof(struct Node));
        *head_ref = new_node;
        (*head_ref)->next = NULL;
    }
    else
    {
        while (*head_ref != NULL)
        {
            *head_ref = (*head_ref)->next;
        }
        (*head_ref) = (struct Node*)malloc(sizeof(struct Node));
        *head_ref = new_node;
        (*head_ref)->next = NULL;
    }
}

void robinKarp(char* P, int d, int q, struct Node* tmp, int* count)
{
    while (tmp != NULL)
    {
        int m = strlen(P);
        int n = strlen(tmp->string);
        char pls1 = '[', pls2 = ']';
        int h;
        h = pow(d, m - 1);
        h = h % q;
        int i;
        int p = 0;
        int t0 = 0;
        char newStr[100];
        newStr[0] = '\0';
        int wait = 0; // used to prevent from printing the found substring multiple times
        char substr1[100];
        char substr2[100];
        substr1[0] = '\0';
        substr2[0] = '\0';
        substr1[m] = '\0';
        substr2[m] = '\0';

        for (i = 0; i < m; i++)
        {
            p = (d * p + P[i]) % q;
            t0 = (d * t0 + tmp->string[i]) % q;
        }
        for (i = 0; i <= n - m; i++)
        {
            if (p == t0)
            {
                strncpy(substr1, &P[0], m);
                strncpy(substr2, &tmp->string[i], m);
                if (strcmp(substr1, substr2) == 0)
                {
                    (*count)++;
                    strncat(newStr, &pls1, 1);
                    strncat(newStr, substr1, m);
                    strncat(newStr, &pls2, 1);
                    tmp->flag = 1;
                    wait += m;

                }
            }
            if (i < n - m)
            {
                t0 = (d * (t0 - tmp->string[i] * h) + tmp->string[i + m]) % q;
                if (t0 < 0)
                    t0 = (t0 + q);
            }
            // double printing prevention
            if (wait == 0)
                strncat(newStr, &tmp->string[i], 1);
            if (wait != 0)
                wait--;
        }
        strcpy(tmp->newString, newStr);
        tmp = tmp->next;
    }
    return;
}



struct Node* createScanpathList(char* fileName)
{
    int lines = 0, i;
    char c = NULL, tmpStr[100];
    tmpStr[0] = '\0';
    FILE* file;
    file = fopen(fileName, "r");
    struct Node* scanPath = NULL;
    struct Node* head;
    struct Node* tmp;
    tmp = (struct Node*)malloc(sizeof(struct Node));
    tmp->next = NULL;
    head = (struct Node*)malloc(sizeof(struct Node));
    head->next = NULL;


    if (file == NULL)
    {
        printf("Cant open the file\n");
    }

    while (c != EOF)
    {
        c = fgetc(file);
        if (c == '\n')
            lines++;
    }
    rewind(file);
    lines++;
    printf("The %s file has been loaded successfully!\n\n", fileName);

    for (i = 0; i < lines; i++)
    {
        fscanf(file, "%s\n", tmpStr);
        InsertNode(&scanPath, tmpStr);

        head = scanPath;
        head->next = tmp->next;
        tmp->next = head;
        head = head->next;
    }
    return tmp;
}

void PrintList(struct Node* head,int count)
{
    int c = 1;
    
    if(head == NULL)
    {
    	printf("None");
    	//return;
	}

    while (head != NULL )
    {
        
		
		printf("%d. %s\n", c,head->newString);
        c++;
        head = head->next;
    }
    c--;
    if (c!=0)
    	printf("%d patterns detected in %d scanpaths\n\n", count,c );
}	

