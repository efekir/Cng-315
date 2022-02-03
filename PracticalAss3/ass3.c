#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "avltree.h"
#include "queue.h"

AVLTree readData(char* filename, int *size);
AVLTree insertSong(char name[30], char genre[30], char album[30], char artist[30], char date[30], char time[30], int year, AVLTree);
void displaySongs(AVLTree mTree);
void popularArtist(AVLTree t, int size);
void oldestSong(AVLTree t, int size);

int main(int argc, char *argv[])
{
	int size,condition = 0;
	
	AVLTree myTree;
	myTree = CreateTree();
	myTree = readData(argv[1],&size);
		
	printf("Welcome to Song Indexing\n");
	while(!condition)
	{
		fflush(stdin);
		char menu;
		printf("\n-----------------------------------------------------------------------------\n1. Display the full index of songs\n2. Display the songs of popular artist\n3. Display the oldest song\n4. Exit\n\nOption: ");
		scanf("%c",&menu);
		fflush(stdin);
		
		switch(menu)
		{
			case '1':
				printf("\n");
				displaySongs(myTree);
				break;
			case '2':
				popularArtist(myTree,size);
				break;
			case '3':
				
				break;
			case '4':
				condition = 1;
				break;
			default:
				printf("invalid input!\n");
				break;
		}
	}
	
	return 0;
}

AVLTree readData(char* filename,int *size)
{
	FILE* sfile;
	char ch;
	int line=0, i;
	
	
	sfile = fopen(filename, "r");
	
	if (sfile == NULL)
    {
        printf("Failed to open the file!\n");
        exit(1);
    }
    else
    {
    	AVLTree t;
    	t = CreateTree();
	
        while ((ch = fgetc(sfile)) != EOF)
    	{
        	if (ch == '\n')
        	{
            	line++;
        	}
    	}
		rewind(sfile);
		
		*size = line+1;
	
		for (i = 0; i < line + 1; i++)
    	{
    		char name[30], genre[30], album[30], artist[30], date[30], time[30];//I added extra variable time to get the input
    		int year;
        	
			fscanf(sfile, "%[^;];%[^;];%[^;];%[^;];%d;%[^ ] %s\n",name,genre,album,artist,&year,date,time);
    		//printf("%s %s %s %s %d - %s",name,genre,album,artist,year,date);
    		t = insertSong(name,genre,album,artist,date,time,year,t);
		}
		printf(">>songIndexing %s\n\n",filename);
		return t;
    }

	fclose(sfile);
}

AVLTree insertSong(char name[30], char genre[30], char album[30], char artist[30], char date[30], char time[30], int year, AVLTree t)
{
	if (t == NULL)
	{
		t = malloc(sizeof(AVLTree));
		if (t == NULL)
		{
			printf("Out of memory!\n");			
		}
		else
		{
		    strcpy(t->SongName,name);
		    strcpy(t->SongGenre,genre);
		    strcpy(t->AlbumName,album);
		    strcpy(t->ArtistName,artist);
		    strcpy(t->Date,date);
		    strcpy(t->time,time);
		    t->ReleaseYear = year;
            t->height = 0;
            
			t->left = NULL;
			t->right = NULL;
			t->next = NULL;
		}
	}
	else
    {
        int c;
        c = strcmp(artist,t->ArtistName);
        
		if (c < 0)
        {
            t->left = insertSong(name,genre,album,artist,date,time,year,t->left);
            if (AVLTreeHeight(t->left) - AVLTreeHeight(t->right) == 2)
            {
                int c2;
                c2 = strcmp(artist,t->left->ArtistName);
                if (c2 < 0)
                {
                    t = SingleRotateWithLeft(t);
                }
                else
                {
                    t = DoubleRotateWithLeft(t);
                }
            }
        }
        else if (c > 0)
        {
            t->right = insertSong(name,genre,album,artist,date,time,year,t->right);
            if (AVLTreeHeight(t->right) - AVLTreeHeight(t->left) == 2)
            {
                int c3;
                c3 = strcmp(artist,t->right->ArtistName);
                if (c3 > 0)
                {
                    t = SingleRotateWithRight(t);
                }
                else
                {
                    t = DoubleRotateWithRight(t);
                }
            }
        }
        else
        {
            t->next = insertSong(name,genre,album,artist,date,time,year,t->next);
            return t;
        }
    }
    
	t->height = Max(AVLTreeHeight(t->left), AVLTreeHeight(t->right)) + 1;

	return t;
}

void displaySongs(AVLTree mTree)
{
	AVLTree t;
    t = (AVLTree)malloc(sizeof(struct Tree));
    t = mTree;
	
	if (t != NULL)
	{
		displaySongs(t->left);
        printf("%s;%s;%s;%s;%d;%s %s\n",t->SongName,t->SongGenre,t->AlbumName,t->ArtistName,t->ReleaseYear,t->Date,t->time);
        displaySongs(t->next);
		displaySongs(t->right);
	}
}

void popularArtist(AVLTree t, int size)
{
	Queue q;
	q = CreateQueue();
	enqueue(q,t);
	AVLTree tmp;
	
	char arr[size][30];
	int i,j,k,l, max = 0;
	char most_repeated[30];
		
	while(q->front != NULL)
    {
        strcpy(arr[i],q->front->data->ArtistName);
        tmp = dequeue(q);
        i++;
        if(tmp->next != NULL)
        {
            enqueue(q,tmp->next);
        }
        if(tmp->right !=NULL)
        {
            enqueue(q,tmp->right);
        }
        if(tmp->left != NULL)
        {
            enqueue(q,tmp->left);
        }
    }
    MakeEmptyQueue(q);

	
	for(i=0 ; i<size ; i++)
	{
		for(k=i+1 ; k<size ; k++)
		{
			int compare = strcmp(arr[i],arr[k]);
			
			if(compare == 0)
			{
				l++;
			}
		}
		if(l>max)
		{
			max = l;
			strcpy(most_repeated,arr[i]);
		}
		l = 1;
	}
	
	Queue q2;
	q2 = CreateQueue();
	enqueue(q2,t);
	AVLTree tmp2;
	
	while(q2->front != NULL)
	{
		int compare = strcmp(q2->front->data->ArtistName,most_repeated);
		
		if(compare == 0)
		{
			printf("%s;%s;%s;%s;%d;%s %s\n",q2->front->data->SongName,q2->front->data->SongGenre,q2->front->data->AlbumName,q2->front->data->ArtistName,q2->front->data->ReleaseYear,q2->front->data->Date,q2->front->data->time);
		}
		tmp2 = dequeue(q2);
		
		if(tmp2->left != NULL)
        {
            enqueue(q2,tmp2->left);
        }
        if(tmp2->next != NULL)
        {
            enqueue(q2,tmp2->next);
        }
        if(tmp2->right != NULL)
        {
            enqueue(q2,tmp2->right);
        }
	}
	MakeEmptyQueue(q2);
	
}
/*
void oldestSong(AVLTree t, int size)
{
	
}
*/

























