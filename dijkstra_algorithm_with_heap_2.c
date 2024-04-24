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
DIST* insert(DIST* heap, int* heapSize, int* heapCapacity, int vertex, int dist);
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
	//dijkstra(graph2, 0);
	
	GRAPH* graph3 = createGraph(11);
	
	addEdges(graph3, 0, 1, 3);
	addEdges(graph3, 0, 4, 5);
	addEdges(graph3, 0, 7, 4);
	addEdges(graph3, 1, 2, 2);
	addEdges(graph3, 1, 5, 7);
	addEdges(graph3, 1, 4, 5);
	addEdges(graph3, 2, 5, 2);
	addEdges(graph3, 2, 3, 3);
	addEdges(graph3, 2, 6, 6);
	addEdges(graph3, 3, 6, 7);
	addEdges(graph3, 3, 10, 2);
	addEdges(graph3, 4, 5, 4);
	addEdges(graph3, 4, 7, 7);
	addEdges(graph3, 5, 7, 5);
	addEdges(graph3, 5, 8, 4);
	addEdges(graph3, 5, 9, 3);
	addEdges(graph3, 5, 6, 4);
	addEdges(graph3, 6, 10, 6);
	addEdges(graph3, 6, 9, 4);
	addEdges(graph3, 7, 8, 2);
	addEdges(graph3, 8, 9, 6);
	addEdges(graph3, 9, 10, 5);
	
	dijkstra(graph3, 1);
	
	return 0;
}

void dijkstra(GRAPH* graph, int src){
	int n = graph -> numOfNodes, i;
	int* dist = (int*) malloc(n * sizeof(int));
	
	for(i=0;i<n;i++) dist[i] = INT_MAX;
	dist[src] = 0;
	
	DIST* heap = makeHeap(n, dist);
	int heapSize = n, heapCapacity = n;
	DIST u;
	u = heap[src];
	heap[src] = heap[0];
	heap[0] = u;
	NODE* temp;
	
	//for the route of each vertex
	int* route = (int*) malloc(n * sizeof(int));
	for(i=0;i<n;i++) route[i] = -1;
	
	while(heapSize){
		u = extractMin(heap, &heapSize);
		
		temp = graph -> adjList[u.vertex];
		while(temp != NULL && u.d != INT_MAX){ // "u.d != INT_MAX" is for the case when the graph is not connected
			if(dist[temp -> vertex] > dist[u.vertex] + temp -> weight){
				dist[temp -> vertex] = dist[u.vertex] + temp -> weight;
				heap = insert(heap, &heapSize, &heapCapacity, temp -> vertex, dist[temp -> vertex]);
				route[temp -> vertex] = u.vertex;
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
	free(heap);
	free(dist);
}

//her bir node için yalnýzca son durak olan nodelarý kaydettiðimizden -1 görene kadar, yani srcye gelene kadar, ilerleyip sonra yazdýrmaya baþlýyoruz.
void printRoute(int* route, int dest){
	if(route[dest] == -1){
		printf("%d", dest);
		return;
	}
	printRoute(route, route[dest]);
	printf(" -> %d", dest);
}

DIST* makeHeap(int n, int* dist){
	DIST* heap = (DIST*) malloc(n * sizeof(DIST));
	int i;
	for(i=0;i<n;i++){
		heap[i].d = dist[i];
		heap[i].vertex = i;
	}
	
	return heap;
}

void siftUp(DIST* heap, int i){
	DIST temp;
	while(i){
		if(heap[i].d < heap[(i - 1) / 2].d){
			temp = heap[i];
			heap[i] = heap[(i - 1) / 2];
			heap[(i - 1) / 2] = temp;
		}
		i = (i - 1) / 2;
	}
}

void siftDown(DIST* heap, int n, int i){
	int index = i;
	DIST temp;
	
	do{
		i = index;
		if(2 * i + 1 < n && heap[2 * i + 1].d < heap[index].d){
			index = 2 * i + 1;
		}
		if(2 * i + 2 < n && heap[2 * i + 2].d < heap[index].d){
			index = 2 * i + 2;
		}
		temp = heap[i];
		heap[i] = heap[index];
		heap[index] = temp;
	} while(i != index);
}

DIST extractMin(DIST* heap, int* n){
	DIST result = heap[0];
	heap[0] = heap[*n - 1];
	(*n)--;
	siftDown(heap, *n, 0);
	return result;
}

DIST* insert(DIST* heap, int* heapSize, int* heapCapacity, int vertex, int dist){
	if(*heapSize == *heapCapacity){
		*heapCapacity *= 2;
		heap = realloc(heap, *heapCapacity * sizeof(DIST));
	}
	heap[*heapSize].vertex = vertex;
	heap[*heapSize].d = dist;
	siftUp(heap, *heapSize);
	(*heapSize)++;
	return heap;
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
	    printf("%d -> ", i); // hangi node'un komþularý.
	    NODE* temp = graph -> adjList[i];
	    while(temp != NULL) {
	        printf("(%d, w: %d), ", temp -> vertex, temp -> weight);
	        temp = temp -> next;
	    }
	    printf("\n");
	}
}
