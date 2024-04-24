#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int vertex;
	int weight;
	struct node* next;
}NODE;

typedef struct graph{
	int numOfNodes;
	NODE** adjList;
}GRAPH;

typedef struct dist{
	int d;
	int vertex;
}DIST;

NODE* createNode(int, int);
GRAPH* createGraph(int);
void addEdges(GRAPH*, int, int, int);
int hasEdge(GRAPH*, int, int);
void printAdjList(GRAPH* graph);
DIST* makeHeap(int n, int* dist);
void siftUp(DIST* heap, int i);
void siftDown(DIST* heap, int n, int i);
DIST extractMin(DIST* heap, int* n);
void changePriority(DIST* heap, int n, int i, int p);
void dijkstra(GRAPH* graph, int src);
void printRoute(int* route, int dest);

int main(){
	GRAPH* graph = createGraph(4);
	
	addEdges(graph, 0, 2, 1);
	addEdges(graph, 0, 3, 4);
	addEdges(graph, 2, 3, 2);
	addEdges(graph, 2, 1, 6);
	addEdges(graph, 3, 1, 3);
	
	//printAdjList(graph);
	//dijkstra(graph, 0);
	
	GRAPH* graph2 = createGraph(6);
	
	addEdges(graph2, 0, 1, 3);
	addEdges(graph2, 0, 3, 10);
	addEdges(graph2, 1, 2, 3);
	addEdges(graph2, 1, 4, 5);
	addEdges(graph2, 1, 3, 8);
	addEdges(graph2, 3, 1, 2);
	addEdges(graph2, 3, 4, 5);
	addEdges(graph2, 4, 5, 0);
	addEdges(graph2, 2, 3, 3);
	addEdges(graph2, 2, 4, 1);
	addEdges(graph2, 2, 5, 2);
	
	//printAdjList(graph2);
	dijkstra(graph2, 0);
	
	return 0;
}

void dijkstra(GRAPH* graph, int src){
	int n = graph -> numOfNodes, i, j, u, minDist;
	
	int* label = (int*) calloc(n, sizeof(int));
	int* dist = (int*) malloc(n * sizeof(int));
	for(i=0;i<n;i++) dist[i] = INT_MAX;
	dist[src] = 0;
	
	NODE* temp;
	
	//for the route of each vertex
	int* route = (int*) malloc(n * sizeof(int));
	for(i=0;i<n;i++) route[i] = -1; 
	
	//finding the minimum distance node out of unknown region
	for(i=0;i<n;i++){
		minDist = INT_MAX;
		for(j=0;j<n;j++){
			if(!label[j] && dist[j] < minDist){
				minDist = dist[j];
				u = j;
			}
		}
		
		label[u] = 1; //adding found node to known region because it is its correct distance. it cannot be less than that
		
		temp = graph -> adjList[u];
		while(temp != NULL){
			if(dist[temp -> vertex] > dist[u] + temp -> weight){
				dist[temp -> vertex] = dist[u] + temp -> weight;
				route[temp -> vertex] = u;
			}
			temp = temp -> next;
		}
	}
	
	//routes and costs
	for(i=0;i<graph -> numOfNodes;i++){
		printf("\nshortest path from %d to %d... ", src, i);
		if(dist[i] != INT_MAX){
			printRoute(route, i);
			printf("\ncost: %d", dist[i]);
		}
		else printf("\nthere is no path from %d to %d... ", src, i);
	}
	
	free(route);
	free(label);
	free(dist);
}

//her bir node i�in yaln�zca son durak olan nodelar� kaydetti�imizden -1 g�rene kadar, yani srcye gelene kadar, ilerleyip sonra yazd�rmaya ba�l�yoruz.
void printRoute(int* route, int dest){
	if(route[dest] == -1){
		printf("%d", dest);
		return;
	}
	printRoute(route, route[dest]);
	printf(" -> %d", dest);
}

NODE* createNode(int vertex, int weight){
	NODE* node = (NODE*) malloc(sizeof(NODE));
	node -> vertex = vertex;
	node -> weight = weight;
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

void addEdges(GRAPH* graph, int src, int dest, int weight){
	if(src == dest || src >= graph -> numOfNodes || dest >= graph -> numOfNodes) return;
	if(hasEdge(graph, src, dest)) return;
	
	// Add edge from src to dest
	NODE* newNode = createNode(dest, weight);
	newNode->next = graph->adjList[src];
	graph->adjList[src] = newNode;
	
	// Add edge from dest to src
	newNode = createNode(src, weight);
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

void printAdjList(GRAPH* graph){
	int i, n = graph -> numOfNodes;
	
	for(i=0; i<n; i++) {
	    printf("%d -> ", i); // hangi node'un kom�ular�.
	    NODE* temp = graph -> adjList[i];
	    while(temp != NULL) {
	        printf("(%d, w: %d), ", temp -> vertex, temp -> weight);
	        temp = temp -> next;
	    }
	    printf("\n");
	}
}
