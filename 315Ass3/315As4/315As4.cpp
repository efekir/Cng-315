#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct graphHead graphHead;
typedef struct graphVertex graphVertex;
typedef struct graphArc graphArc;
////Structs////
struct graphHead
{
	int count;
	struct graphVertex* first;
};
////////////////////
struct graphVertex
{
	struct graphVertex* next;
	struct graphArc* firstArc;
	char datapath[50];
	int flag;
};
////////////////////
struct graphArc
{
	struct graphVertex* destination;
	struct graphArc* next;
	float value;
};


void addVertices(graphHead* graph, graphVertex* vertex);
graphHead* createVertices(graphHead* graph, char filename[50]);

int main()
{
    char filename1[50], filename2[50];
    printf("Enter AOIs file name >");
    scanf("%s", filename1);
    printf("Enter scanpaths file name > ");
    scanf("%s", filename2);


    
}

void addVertices(graphHead* graph, graphVertex* vertex)		///Utility function for inserting the vertex
{
	graph->count++;
	if (graph->first == NULL)
		graph->first = vertex;
	else
	{
		graphVertex* temp = graph->first;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = vertex;
	}
}

graphHead* createVertices(graphHead* graph, char filename[50])	
{
	///File handling
	FILE* file;
	file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("File %s not found.", filename);
		exit;
	}
	///
	else
	{
		int count = 1;
		char datapath[50];
		graphVertex *vertex;
		while (fgets(datapath, 60, file) != NULL) 	/////looping file line by line
		{
			datapath[strlen(datapath) - 1] = '\0';		///deleting the new line(\n) from fgets
			vertex = malloc(sizeof(graphVertex));
			strcpy(vertex->datapath, datapath);
			vertex->firstArc = NULL;
			vertex->flag = 1;
			vertex->next = NULL;
			addVertices(graph, vertex);		//// adds vertex to the main graph structure
		}
		return graph;
	}
}