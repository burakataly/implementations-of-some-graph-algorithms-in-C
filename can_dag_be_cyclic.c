#include<stdio.h>
#include<stdlib.h>

//finding whether a dag can be cyclic when an edge is reversed

struct node{
	int vertex;
	struct node* next;
};

typedef struct node NODE;

struct graph{
	int numOfNodes;
	NODE** adjList;
	NODE** tails;
};

typedef struct graph GRAPH;

NODE* createNode(int);
GRAPH* createGraph(int);
void addEdge(GRAPH*, int, int);
void addEdgeToTail(GRAPH*, int, int);
int detectCycle(GRAPH*);
void removeEdge(GRAPH* graph, int src);
int depthFirstSearch(GRAPH*, int*, int);

int main(){
	int n, m, i, u, v, first, current;
	NODE* temp;
	scanf("%d %d", &n, &m);
	GRAPH* graph = createGraph(n);

	for(i=0;i<m;i++){
		scanf("%d %d", &u, &v);
		addEdge(graph, u-1, v-1);
	}
	
	for(i=0;i<n;i++){
		if(graph->adjList[i] != NULL){
			first = graph->adjList[i]->vertex;
			do{
				current = graph->adjList[i]->vertex;
				removeEdge(graph, i);
				addEdge(graph, current, i);
				if(detectCycle(graph)){
					printf("Yes");
					return 0;
				}
				removeEdge(graph, current);
				addEdgeToTail(graph, i, current);
			}while(current != first);
		}
	}
	printf("No");
	return 0;
}

int detectCycle(GRAPH* graph){
	int n = graph->numOfNodes, i;
	int* label = (int*) calloc(n, sizeof(int));
	
	for(i=0;i<n;i++){
		if(!label[i]){
			if(depthFirstSearch(graph, label, i)){
				free(label);
				return 1;
			}
		}
	}
	free(label);
	return 0;
}

int depthFirstSearch(GRAPH* graph, int* label, int start){
	if(label[start] == 2) return 0;
	if(label[start] == 1) return 1;
	
	NODE* temp = graph -> adjList[start];
	
	label[start] = 1;
	
	while(temp != NULL){
		if(depthFirstSearch(graph, label, temp -> vertex)) return 1;
		temp = temp -> next;
	}
	
	label[start] = 2;
	
	return 0;
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
	graph->numOfNodes = n;
	graph->adjList = (NODE**) calloc(n, sizeof(NODE*));
	graph->tails = (NODE**) calloc(n, sizeof(NODE*));
	return graph;
}

void addEdge(GRAPH* graph, int src, int dest){
	if(src == dest || src >= graph->numOfNodes || dest >= graph->numOfNodes) return;
	NODE* newNode = createNode(dest);
	if(graph->adjList[src] == NULL){
		graph->adjList[src] = newNode;
		graph->tails[src] = newNode;
	}
	else{
		newNode->next = graph->adjList[src];
		graph->adjList[src] = newNode;
	}
}

void addEdgeToTail(GRAPH* graph, int src, int dest){
	if(src == dest || src >= graph->numOfNodes || dest >= graph->numOfNodes) return;
	
	NODE* newNode = createNode(dest);
	if(graph->adjList[src] == NULL){
		graph->adjList[src] = newNode;
		graph->tails[src] = newNode;
	}
	else{
		graph->tails[src]->next = newNode;
		graph->tails[src] = newNode;
	}
}

void removeEdge(GRAPH* graph, int src){
	NODE* temp = graph->adjList[src];
	if(temp == NULL) return;
	graph->adjList[src] = temp->next;
	if(graph->adjList[src] == NULL) graph->tails[src] = NULL;
	free(temp);
}
