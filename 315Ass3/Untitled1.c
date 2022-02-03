#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Typedefs
typedef struct graphHead graphHead;
typedef struct graphVertex graphVertex;
typedef struct graphArc graphArc;
//Structs
struct graphHead
{
	int count;
	struct graphVertex* first;
};

struct graphVertex
{
	struct graphVertex* next;
	struct graphArc* firstArc;
	char data[50];
	int id;
};

struct graphArc
{
	struct graphVertex* destination;
	struct graphArc* next;
	int value;
};

//Function Bodys
void addVertices(graphHead* graph, graphVertex* vertex);
graphHead* createVertices(graphHead* graph, char* filename1,char* filename2);
graphHead * createEdges(graphHead *graph);
void displayGraph(graphHead *graph);

int main()
{
    char filename1[50], filename2[50];
    printf("Enter AOIs file name >");
    scanf("%s", filename1);
    printf("Enter scanpaths file name > ");
    scanf("%s", filename2);

    //Create the graph in main
    graphHead *graph;
	graph = malloc(sizeof(graphHead));
	graph->count = 0;
	graph->first = NULL;
    //Create Vertices
    graph = createVertices(graph,filename1,filename2);
    //Create Edges
    graph = createEdges(graph);
    //Print
    displayGraph(graph);
    
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

graphHead* createVertices(graphHead* graph, char* filename1,char* filename2)	
{
	///File handling
	char ch;
	FILE* file1;
	file1 = fopen(filename1, "r");
	if (file1 == NULL)
	{
		printf("File not found... Terminating.");
		exit(1);
	}
    FILE* file2;
	file2 = fopen(filename2, "r");
	if (file2 == NULL)
	{
		printf("File not found... Terminating.");
		exit(1);
	}

    char elementName, dumStr[100] = "";
    int topX, width, topY, height, line1 = 1, line2 = 1, x, y ,dummy, index;
    int xCompare, yCompare;
    int i, j, idCount = 1;
    graphVertex* vertex;

    //Counting the lines
    while ((ch = fgetc(file1)) != EOF)
    {
        if (ch == '\n')
        {
            line1++;
        }
    }
    rewind(file1);

     while ((ch = fgetc(file2)) != EOF)
    {
        if (ch == '\n')
        {
            line2++;
        }
    }
    rewind(file2);
    
    //Matching fixations to aois
    for (i = 0; i < line2; i++)
    {
        fscanf(file2,"%d %d %d %d",&index,&x,&y,&dummy);
        //printf("%d \n",index);
		for (j = 0; j < line1; j++)
        {
            fscanf(file1, "%c %d %d %d %d",&elementName,&topX,&width,&topY,&height);
            xCompare = topX + width;
            yCompare = topY + height;

            if ((x>=topX && x <= xCompare) && (y >=topY && y <= yCompare))
            {
                strncat(dumStr,&elementName,1);
                rewind(file1);
                break;
            } 
        }
            if (index == 0) // adding new person and vertices
        {
            vertex = malloc(sizeof(graphVertex));
            strcpy(vertex->data, dumStr);
            vertex->firstArc = NULL;
            vertex->id = idCount;
            vertex->next = NULL;
            addVertices(graph, vertex);
            //puts(dumStr);
            strcpy(dumStr,"");// erasing the dummy string for each person
            idCount++;
        }
    }
    return graph;
}

int insertArc(graphVertex *fromVertex,graphVertex *toVertex,float value)
{
	graphArc * arc = malloc (sizeof(graphArc));
	arc->destination = toVertex;
	graphArc *tempArc = fromVertex->firstArc;
	if (tempArc == NULL){
		fromVertex->firstArc = arc;
		arc->value = value;
		arc->next = NULL;
	}
	else{
		while(tempArc->next != NULL){
			tempArc = tempArc->next;
		}
		tempArc->next = arc;
		arc->value = value;
		arc->next= NULL;
	}
}

int lcs(graphVertex *x,graphVertex *y) // make the matrix for lcs
{
    int m,n,j,i;
	
	m=strlen(x->data);
    n=strlen(y->data);
    int c[100][100];
    char b[100][100];
    for(i=0;i<=m;i++)
    c[i][0]=0;
    for(i=0;i<=n;i++)
    c[0][i]=0;
                    
    for(i=1;i<=m;i++)
        for(j=1;j<=n;j++)
        {
            if(x->data[i-1]==y->data[j-1])
            {
                c[i][j]=c[i-1][j-1]+1;
                b[i][j]='c';
            }
            else if(c[i-1][j]>=c[i][j-1])
            {
            c[i][j]=c[i-1][j];
            b[i][j]='u';
            }
            else
            {
            c[i][j]=c[i][j-1];
            b[i][j]='l';
            }
        }
    int result = lcsCounter(m,n,0,b);
    return result;
}

int lcsCounter(int i,int j,int count,char b[100][100]) // count matrix for lcs
{
                if(i==0 || j==0)
                    return count;

                if(b[i][j]=='c')
                {
                     count = count+1;
					lcsCounter(i-1,j-1,count,b);
                   
                }
                else if(b[i][j]=='u')
                    lcsCounter(i-1,j,count,b);
                else
                    lcsCounter(i,j-1,count,b);
                    
}
graphHead * createEdges(graphHead *graph) // create edges
{
	graphVertex * toVertex ;
	graphVertex * fromVertex = graph->first ;
    int lcsCount = 0;

	while(fromVertex!=NULL)
	{
		toVertex = fromVertex->next;
		while(toVertex!=NULL)
		{
           lcsCount = lcs(fromVertex,toVertex);
           //printf("lsc is %d",lcsCount);


			if(lcsCount>=5)			
				{
					//addArc(fromVertex,toVertex,lsCount);		
					//addArc(toVertex,fromVertex,lsCount);		
				}
			toVertex = toVertex->next;
		}
		fromVertex = fromVertex->next;
	}
	return graph;
}

void displayGraph(graphHead *graph)
{
    struct graphVertex *tempVertex = NULL;
	struct graphArc * tempArc = NULL;
    int count = 0;
	printf("\n");
	tempVertex = graph->first;
	
	

    while(tempVertex != NULL)	//vertex
    {
        printf("%d ->",tempVertex->id);
        tempArc = tempVertex->firstArc;	
        while(tempArc != NULL)	//edge
        {
            printf("%d ->",tempArc->destination->data);
            tempArc = tempArc->next;
        }
        tempVertex = tempVertex->next;
        printf("\n");
    }
}

