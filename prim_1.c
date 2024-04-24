#include <stdio.h>
#include <stdlib.h>

/* 
unlike kruskal, we are starting from an arbitraty node as a root of MST and span this tree one by one. Actually prim is so similar to dijkstra. like we do in
dijkstra, when we select a node to span our tree, we are traversing its neighbors and updating their costs in prim. But costs are not the distance to the root
like they are in dijkstra. costs are the minimum distance to the any of the the leaf node of the current tree, simply the edge between that node and a leaf node.
So if the current selected node has a lower weight edge to its neighbor than its current cost then we update it. because we want to select the node with minimum
cost, we use minheap.

while implementing I used two different approaches one of them is this code where I use changePriority when updating the cost of a node, the other one is simply
insert a new node to heap with updated cost. first one is better in terms of space complexity, while other one (prim_2.c) is better in terms of time complexity.

in this one, when we update a node's cost we call changePriority whose complexity is O(V) because we need to find that node's place in the heap with linear
search. So in the worst case if we do this for each edge, the overall time complexity for this operation would be E * V. extractMin operation for worst case
would be E*logE from log(E!). and E is proportional to V^2 in worst case so extractMin is E*logV in overall so total complexity of this prim impl is O(E * V)
But space complexity is O(V) if we ignore graph's own space complexity. prim_2.c has O(E * logV) time and O(E) space complexity...

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
void changePriority(COST* heap, int n, int vertex, int p);
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
	int totalCost = 0;
	
	for(i=0;i<n;i++){
		heap[i].vertex = i;
		heap[i].cost = cost[i] = INT_MAX;
		parent[i] = -1;
	}
	
	heap[0].cost = cost[0] = 0;
	buildHeap(heap, n);
	
	i = n;
	while(n){
		temp = extractMin(heap, &n);
		label[temp.vertex] = 1;
		totalCost += temp.cost;
		
		iter = graph->adjList[temp.vertex];
		while(iter != NULL){
			if(!label[iter->vertex] && cost[iter->vertex] > iter->weight){
				cost[iter->vertex] = iter->weight;
				changePriority(heap, n, iter->vertex, iter->weight);
				parent[iter->vertex] = temp.vertex; //parent of this node will be last updated edge
			}
			iter = iter->next;
		}
	}
	
	printf("\nMinimum Spanning Tree...\n");
	for(j=0;j<i;j++){
		printf("(%d, %d)\n", parent[j], j);
	}
	
	printf("Total Cost: %d\n", totalCost);
	
	free(label);
	free(parent);
	free(cost);
	free(heap);
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

void changePriority(COST* heap, int n, int vertex, int p){
	int i = 0;
	while(i < n && heap[i].vertex != vertex) i++;
	if(i == n || heap[i].cost == p) return;
	int oldP = heap[i].cost;
	heap[i].cost = p;
	if(p < oldP) siftUp(heap, i);
	else siftDown(heap, n, i);
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
