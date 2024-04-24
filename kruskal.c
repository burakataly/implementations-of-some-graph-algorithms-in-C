#include <stdio.h>
#include <stdlib.h>

/* 
when selecting an edge with minimum possible weight, a union operation is happening for those two nodes or we should look whether the nodes of that edge are
already connected with a path or not. So disjoint sets are used to take care of these operations. And selecting miminum possible edge is done by using minheap.

Complexity: in each iteration an extractMin and setUnion operation is done. in worst case, extractMin will be done for E times. So time complexity for this is
E * logE from log(E!). all disjoint sets operations are nearly constant since union by rank and path compression are used. Total is O(E * logE).
*/

typedef struct {
	int parent;
	int rank;
}node;

typedef struct {
	int u;
	int v;
	int weight;
}EDGE;

void makeSet(node* nodes, int n, int i);
int find(node* nodes, int n, int i);
int setUnion(node* nodes, int n, int i, int j);
void buildHeap(EDGE* heap, int n);
EDGE extractMin(EDGE* heap, int* n);
void siftDown(EDGE* heap, int n, int i);
void kruskal(EDGE* edges, int E, int V);

int main(){
	int V, E, i;
	EDGE* edges;
	
	scanf("%d %d", &V, &E);
	
	edges = (EDGE*) malloc(E * sizeof(EDGE));
	
	for(i=0;i<E;i++){
		scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].weight);
	}
	
	kruskal(edges, E, V);
	
	return 0;
}

void kruskal(EDGE* edges, int E, int V){
	int i, totalWeight = 0;
	EDGE temp;
	node* nodes = (node*) malloc(V * sizeof(node));
	EDGE* MST = (EDGE*) malloc((V - 1) * sizeof(EDGE));
	
	for(i=0;i<V;i++) makeSet(nodes, V, i);
	buildHeap(edges, E);
	
	i = 0;
	while(i < V - 1){ // MST has V - 1 edges so when we select the V - 1 th edge we are done 
		temp = extractMin(edges, &E);
		
		if(setUnion(nodes, V, temp.u, temp.v)){ //if u and v belong to different sets then we can select that edge 
			MST[i] = temp;
			totalWeight += temp.weight;
			i++;
		}
	}
	
	printf("\nMinimum Spanning Tree...\n");
	for(i=0;i<V-1;i++){
		printf("(%d -> %d), w: %d\n", MST[i].u, MST[i].v, MST[i].weight);
	}
	
	printf("total weight of MST: %d\n", totalWeight);
}

void makeSet(node* nodes, int n, int i){
	if(i < 0 || i >= n) return;
	nodes[i].parent = i;
	nodes[i].rank = 0;
}

int find(node* nodes, int n, int i){
	if(i < 0 || i >= n) return;
	if(i != nodes[i].parent) nodes[i].parent = find(nodes, n, nodes[i].parent);
	return nodes[i].parent;
}

int setUnion(node* nodes, int n, int i, int j){
	if(i < 0 || i >= n || j < 0 || j >= n) return;
	int id1 = find(nodes, n, i);
	int id2 = find(nodes, n, j);
	if(id1 == id2) return 0;
	if(nodes[id1].rank > nodes[id2].rank){
		nodes[id2].parent = id1;
	}
	else{
		nodes[id1].parent = id2;
		if(nodes[id1].rank == nodes[id2].rank){
			nodes[id2].rank++;
		}
	}
	return 1;
}

void printNodes(node* nodes, int n){
	int i;
	printf("\n");
	for(i=0;i<n;i++) printf("(parent: %d - rank: %d) \n", nodes[i].parent, nodes[i].rank);
	printf("\n");
}

void buildHeap(EDGE* heap, int n){
	int i;
	for(i=n/2;i>=0;i--) siftDown(heap, n, i);
}

EDGE extractMin(EDGE* heap, int* n){
	EDGE result = heap[0];
	heap[0] = heap[*n - 1];
	(*n)--;
	siftDown(heap, *n, 0);
	return result;
}

void siftDown(EDGE* heap, int n, int i){
	int index = i;
	EDGE temp;
	
	do{
		i = index;
		if(2 * i + 1 < n && heap[2 * i + 1].weight < heap[index].weight){
			index = 2 * i + 1;
		}
		if(2 * i + 2 < n && heap[2 * i + 2].weight < heap[index].weight){
			index = 2 * i + 2;
		}
		temp = heap[i];
		heap[i] = heap[index];
		heap[index] = temp;
	} while(i != index);
}
