#include<stdio.h>
#include<stdlib.h>

struct node{
	int vertex;
	struct node* next;
};

typedef struct node NODE;

struct graph{
	int numOfNodes;
	NODE** adjList;
};

typedef struct graph GRAPH;

NODE* createNode(int);
GRAPH* createGraph(int);
void addEdges(GRAPH*, int, int, int);
int hasEdge(GRAPH*, int, int);

void depthFirstSearch(GRAPH*, int, int*, int);

int main(){
	int n = 5, i;
	GRAPH* graph = createGraph(n);
	int* label = (int*) calloc(n, sizeof(int));
	addEdges(graph, n, 0, 1);
	//addEdges(adjMatrix, n, 0, 3);
	addEdges(graph, n, 0, 2);
	addEdges(graph, n, 1, 2);
	addEdges(graph, n, 2, 3);
	
	depthFirstSearch(graph, n, label, 0);
	
	for(i=0;i<n;i++) printf("%d ", label[i]);
	
	return 0;
}

void depthFirstSearch(GRAPH* graph, int n, int* label, int start){
	NODE* temp = graph -> adjList[start];
	int count = 0;
	
	label[start] = 1;
	while(temp != NULL){
		if(label[temp -> vertex] == 0){
			depthFirstSearch(graph, n, label, temp -> vertex);
			count++;
		}
		temp = temp -> next;
	}
	printf("%d\n", count);
}

NODE* createNode(int vertex){
	NODE* node = (NODE*) malloc(sizeof(NODE));
	node -> vertex = vertex;
	node -> next = NULL;
	return node;
}

GRAPH* createGraph(int n){
	int i;
	GRAPH* graph = (GRAPH*) malloc(sizeof(GRAPH));
	graph -> numOfNodes = n;
	graph -> adjList = (NODE**) calloc(n, sizeof(NODE*));
	return graph;
}

void addEdges(GRAPH* graph, int n, int src, int dest){
	if(src == dest || src >= n || dest >= n) return;
	if(hasEdge(graph, src, dest)) return;
	
	// Add edge from src to dest
	NODE* newNode = createNode(dest);
	newNode->next = graph->adjList[src];
	graph->adjList[src] = newNode;
	
	// Add edge from dest to src
	newNode = createNode(src);
	newNode->next = graph->adjList[dest];
	graph->adjList[dest] = newNode;
}

int hasEdge(GRAPH* graph, int src, int dest){
	NODE* temp = graph -> adjList[src];
	while(temp != NULL){
		if(temp -> vertex == dest) return 1;
		temp = temp -> next;
	}
	return 0;
}
