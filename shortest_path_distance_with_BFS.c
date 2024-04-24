#include<stdio.h>
#include<stdlib.h>

/*
Her iterasyonda queue'nun en baþýndaki node'un komþularýna baktýðýmýzdan ve etiketlenmemiþ komþu görünce queue'ya eklediðimizden dolayý
keþfedilmemiþ node bulduðumuzda o an komþularýný gezdiðimiz node'un src'ye uzaklýðýnýn bir fazlasýný yeni keþfedilene atamamýz yeterli.

Benim fonksiyon src'nin bulunduðu cc'deki tüm node'larýn src'ye uzaklýðýný hesaplýyor ama yalnýzca dest'e olaný bulacaksan BFS'ye dest'i
parametre olarak verip dest denk gelince labeler'ý return edersin.

Ayrýca shortest path'ý DFS ile bulamazsýn BFS ile bulabilirsin. Çünkü BFS'de dýþtaki döngünün i. iterasyonunda baþlangýç node'unun i. derece
komþularýný etiketliyosun. Yani her bir seviyede tüm komþularla hesabý kapatýyosun. DFS böyle deðil.
*/

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
void addEdges(GRAPH*, int, int);
int hasEdge(GRAPH*, int, int);

void breadthFirstSearch(GRAPH*, int*, int*, int);
void shortestPathDistance(GRAPH*, int);
void printRoute(int* route, int dest);

int main(){
	int n = 5;
	GRAPH* graph = createGraph(n);
	
	addEdges(graph, 0, 1);
	addEdges(graph, 0, 3);
	addEdges(graph, 0, 2);
	addEdges(graph, 1, 2);
	addEdges(graph, 2, 3);
	
	shortestPathDistance(graph, 1);
	
	return 0;
}

void shortestPathDistance(GRAPH* graph, int src){
	int i, n = graph -> numOfNodes;
	if(src < 0 || src >= n) return;
	int* label = (int*) calloc(n, sizeof(int));
	int* route = (int*) malloc(n * sizeof(int));
	for(i=0;i<n;i++) route[i] = -1;
	breadthFirstSearch(graph, label, route, src);
	
	for(i=0;i<n;i++){
		printf("\nshortest path distance from %d to %d... ", src, i);
		if(label[i]){
			printRoute(route, i);
			printf("\nthe number of edges: %d", label[i] - 1);
		}
		else printf("\nthere is no path from %d to %d... ", src, i);
	}
	free(label);
	free(route);
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

void breadthFirstSearch(GRAPH* graph, int* label, int* route, int start){
	NODE* temp;
	int head = 0, tail = 0, current, n = graph->numOfNodes;
	int* queue = (int*) malloc(n*sizeof(int));
	queue[tail++] = start;
	label[start] = 1;
	while(head < tail){
		current = queue[head++];
		temp = graph -> adjList[current];
		while(temp != NULL){
			if(label[temp -> vertex] == 0){
				label[temp -> vertex] = label[current] + 1;
				queue[tail++] = temp -> vertex;
				route[temp -> vertex] = current;
			}
			temp = temp -> next;
		}
	}
	free(queue);
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

void addEdges(GRAPH* graph, int src, int dest){
	if(src == dest || src >= graph->numOfNodes || dest >= graph->numOfNodes) return;
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
