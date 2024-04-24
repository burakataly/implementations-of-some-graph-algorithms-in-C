#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int u;
	int v;
	int weight;
}EDGE;

void bellmanFord(EDGE* edges, int E, int V, int src);
void printRoute(int* route, int dest);

int main(){
	int V, E, i;
	EDGE* edges;
	
	scanf("%d %d", &V, &E);
	
	edges = (EDGE*) malloc(E * sizeof(EDGE));
	
	for(i=0;i<E;i++){
		scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].weight);
	}
	
	bellmanFord(edges, E, V, 0);
	
	return 0;
}

void bellmanFord(EDGE* edges, int E, int V, int src){
	int *dist = (int*) malloc(V * sizeof(int));
	int *parent = (int*) malloc(V * sizeof(int));
	int i, j;
	
	for(i=0;i<V;i++){
		dist[i] = INT_MAX;
		parent[i] = -1;
	}
	
	dist[src] = 0;
	
	for(i=0;i<V-1;i++){
		for(j=0;j<E;j++){
			if((dist[edges[j].u] != INT_MAX) && (dist[edges[j].v] > dist[edges[j].u] + edges[j].weight)){
				dist[edges[j].v] = dist[edges[j].u] + edges[j].weight;
				parent[edges[j].v] = edges[j].u;
			}
		}
	}
	
	//to look whether there is negative cycle
	for(i=0;i<E;i++){
		if((dist[edges[i].u] != INT_MAX) && (dist[edges[i].v] > dist[edges[i].u] + edges[i].weight)){
			printf("There is a negative cycle...\n");
			free(parent);
			free(dist);
			return;
		}
	}
	
	for(i=0;i<V;i++){
		printf("\nshortest path from %d to %d... ", src, i);
		if(dist[i] != INT_MAX){
			printRoute(parent, i);
			printf("\ncost: %d", dist[i]);
		}
		else printf("\nthere is no path from %d to %d... ", src, i);
	}
	
	free(parent);
	free(dist);
}

void printRoute(int* route, int dest){
	if(route[dest] == -1){
		printf("%d", dest);
		return;
	}
	printRoute(route, route[dest]);
	printf(" -> %d", dest);
}
