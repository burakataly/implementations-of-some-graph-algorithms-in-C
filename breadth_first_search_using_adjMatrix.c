#include<stdio.h>
#include<stdlib.h>

int** createGraph(int);
void addEdge(int**, int, int, int);
void removeEdge(int**, int, int, int);

void breadthFirstSearch(int**, int, int*, int);

int main(){
	int n = 5, i;
	int** adjMatrix = createGraph(n);
	int* label = (int*) calloc(n, sizeof(int));
	addEdge(adjMatrix, n, 0, 1);
	addEdge(adjMatrix, n, 0, 3);
	addEdge(adjMatrix, n, 0, 2);
	addEdge(adjMatrix, n, 1, 2);
	addEdge(adjMatrix, n, 2, 3);
	
	breadthFirstSearch(adjMatrix, n, label, 0);
	
	for(i=0;i<n;i++) printf("%d ", label[i]);
	
	return 0;
}

void breadthFirstSearch(int** adjMatrix, int n, int* label, int start){
	int temp, i, head = 0, tail = 0, count = 0;
	int* queue = (int*) malloc(n*sizeof(int));
	queue[tail++] = start;
	label[start] = 1;
	while(head < tail){
		temp = queue[head++];
		for(i=0;i<n;i++){
			if(adjMatrix[temp][i] != 0 && label[i] == 0){
				label[i] = 1;
				queue[tail++] = i;
				
				count++;
			}
		}
	}
	printf("%d\n", count);
	free(queue);
}

int** createGraph(int n){
	int i;
	int** adjMatrix = (int**) malloc(n*sizeof(int*));
	for(i=0;i<n;i++) adjMatrix[i] = (int*) calloc(n, sizeof(int));
	return adjMatrix;
}

void addEdge(int** adjMatrix, int n, int src, int dest){
	if(src == dest || src >= n || dest >= n) return;
	
	// Add edge from src to dest
	adjMatrix[src][dest] = 1;
	
	// Add edge from dest to src
	adjMatrix[dest][src] = 1;
}

void removeEdge(int** adjMatrix, int n, int src, int dest){
	if(src == dest || src >= n || dest >= n) return;
	
	// remove edge from src to dest
	adjMatrix[src][dest] = 0;
	
	// remove edge from dest to src
	adjMatrix[dest][src] = 0;
}
