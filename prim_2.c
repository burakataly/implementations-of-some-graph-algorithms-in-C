#include <stdio.h>
#include <stdlib.h>

/*
the difference to the prim_1.c is that changePriority is not used here, new node with updated cost is inserted to the heap instead. So there will be more than 
V nodes in the heap but this wont change the complexity of the heap operations because new inserted node will have less cost than the nodes with same vertex so
it will be above those nodes. So that node will be extracted before those nodes. But the extracted node may be an already selected node because of this.
So the for time complexity extractMin and insert operations may be done E time in the worst case logV for each, O(E * logV). space complexity would be O(E) in
the worst case. 

Not: even if the heap operations would have logE complexity, overall complexity would be O(E * logV) because E is proportional to V^2 in a complete graph so
E * log(V^2) is 2 * E * logV which is O(E * logV)...
*/

typedef struct node{
	int vertex;
	int weight;
	struct node* next;
}NODE;

typedef struct graph{
	int numOfNodes;
	NODE** adjList;
}GRAPH;

typedef struct cost{
	int cost;
	int vertex;
}COST;

void prim(GRAPH* graph);
void buildHeap(COST* heap, int n);
void siftUp(COST* heap, int i);
void siftDown(COST* heap, int n, int i);
COST extractMin(COST* heap, int* n);
COST* insert(COST* heap, int* heapSize, int* heapCapacity, int vertex, int cost);
NODE* createNode(int vertex, int weight);
GRAPH* createGraph(int n);
void addEdges(GRAPH* graph, int src, int dest, int weight);
int hasEdge(GRAPH* graph, int src, int dest);

int main(){
	int n=9;
	GRAPH* graph = createGraph(n);
	
    addEdges(graph, 0, 1, 4);
    addEdges(graph, 0, 7, 8);
    addEdges(graph, 1, 2, 8);
    addEdges(graph, 1, 7, 11);
    addEdges(graph, 2, 3, 7);
    addEdges(graph, 2, 8, 2);
    addEdges(graph, 2, 5, 4);
    addEdges(graph, 3, 4, 9);
    addEdges(graph, 3, 5, 14);
    addEdges(graph, 4, 5, 10);
    addEdges(graph, 5, 6, 2);
    addEdges(graph, 6, 7, 1);
    addEdges(graph, 6, 8, 6);
    addEdges(graph, 7, 8, 7);
    
	prim(graph);
    
	return 0;
}

void prim(GRAPH* graph){
	int n = graph -> numOfNodes, i, j;
	int* label = (int*) calloc(n, sizeof(int));
	int* parent = (int*) malloc(n * sizeof(int));
	COST* heap = (COST*) malloc(n * sizeof(COST));
	int* cost = (int*) malloc(n * sizeof(int));
	COST temp;
	NODE* iter;
	int totalCost = 0, heapSize = n, heapCapacity = n;
	
	for(i=0;i<n;i++){
		heap[i].vertex = i;
		heap[i].cost = cost[i] = INT_MAX;
		parent[i] = -1;
	}
	
	heap[0].cost = cost[0] = 0;
	buildHeap(heap, n);
	
	i = 0;
	while(i < n){
		temp = extractMin(heap, &heapSize);
		if(!label[temp.vertex]){ //if it is not an already selected node.
			label[temp.vertex] = 1;
			totalCost += temp.cost;
			
			iter = graph->adjList[temp.vertex];
			while(iter != NULL){
				if(!label[iter->vertex] && cost[iter->vertex] > iter->weight){
					cost[iter->vertex] = iter->weight;
					heap = insert(heap, &heapSize, &heapCapacity, iter->vertex, iter->weight);
					parent[iter->vertex] = temp.vertex; //parent of this node will be last updated edge
				}
				iter = iter->next;
			}
			i++;
		}
	}
	
	printf("\nMinimum Spanning Tree...\n");
	for(i=0;i<n;i++){
		printf("(%d, %d)\n", parent[i], i);
	}
	
	printf("%d", totalCost);
}

void buildHeap(COST* heap, int n){
	int i;
	for(i=n/2;i>=0;i--) siftDown(heap, n, i);
}

void siftUp(COST* heap, int i){
	COST temp;
	while(i){
		if(heap[i].cost < heap[(i - 1) / 2].cost){
			temp = heap[i];
			heap[i] = heap[(i - 1) / 2];
			heap[(i - 1) / 2] = temp;
		}
		i = (i - 1) / 2;
	}
}

void siftDown(COST* heap, int n, int i){
	int index = i;
	COST temp;
	
	do{
		i = index;
		if(2 * i + 1 < n && heap[2 * i + 1].cost < heap[index].cost){
			index = 2 * i + 1;
		}
		if(2 * i + 2 < n && heap[2 * i + 2].cost < heap[index].cost){
			index = 2 * i + 2;
		}
		temp = heap[i];
		heap[i] = heap[index];
		heap[index] = temp;
	} while(i != index);
}

COST extractMin(COST* heap, int* n){
	COST result = heap[0];
	heap[0] = heap[*n - 1];
	(*n)--;
	siftDown(heap, *n, 0);
	return result;
}

COST* insert(COST* heap, int* heapSize, int* heapCapacity, int vertex, int cost){
	if(*heapSize == *heapCapacity){
		*heapCapacity *= 2;
		heap = realloc(heap, *heapCapacity * sizeof(COST));
	}
	heap[*heapSize].vertex = vertex;
	heap[*heapSize].cost = cost;
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
