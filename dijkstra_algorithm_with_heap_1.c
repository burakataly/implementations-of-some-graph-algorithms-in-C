#include <stdio.h>
#include <stdlib.h>

/*
Ana mant�k �u: source node'un kom�ular�ndan minimum uzakl�kta olan i�in bu uzakl�k kesinlikle source'a olan minimum uzakl�kt�r. src'nin ba�ka kom�usu
�zerinden bu node'a daha k�sa mesafeyle ula�amazs�n. Di�er kom�ular i�in min mesafe �u anki mesafe de olabilir, ara bir node'dan ge�en yolun mesafesi de...
Mesela src'nin iki kom�usu olsun biri 2 di�eri 4 mesafede. bu ikisi aras�nda da 1 mesafe olsun. 4 mesafedeki kom�uya di�er kom�u �zerinden 3 birimle ula�r�z.
Sonu� olarak min mesafedeki kom�uyu mesafesi bilinen nodelar k�mesine alabiliriz. Ve onun kom�ular� i�in ayn� tarifeyi uygular�z ta ki t�m nodelar� bilinen 
b�lgeye alana kadar. Bilinen b�lgeye her zaman yeni al�nan node'un min mesafedeki kom�usunu alm�yoruz. B�yle yapmam�z laz�m yoksa sonu� hatal� olabilir. mesela
src'nin kom�ular�ndan min olan� ald�k. Buna u diyelim. Ancak u'nun kendi kom�ular�n�n mesafelerini g�ncelledikten sonra bu node'lar�n src'ye mesafeleri, 
src'nin u ile aras�nda edge olmayan ba�ka bir kom�usunun src'ye mesafesinden k���k olmayabilir. Bu durumda minheap'in tepesinde bu node olur ve bunu extract
ederiz. E�er b�yle de�il de u'nun komu�ular�ndan min olan� se�seydik bu o node'un ger�ek min de�eri olmayabilirdi. ��nk� bu se�ilen node ile asl�nda minimum
durumda olan node aras�nda bu node'un min de�erini de�i�tirebilecek bir edge olabilir.

Complexity: her turda bir kere extractMin yap�l�yor logV kadarl�k bir karma��kl��� var. V tur olaca��ndan V*logV eder. ayr�ca se�ilen node'un kom�ular� i�in
mesafe g�ncellemesi yap�l�yor ve yine logV karma��l�kta. Ancak heapte bu node'u bulmak i�in bir linear search yap�l�yor. Bu sebeple asl�nda V + logV 'lik bir
karma��kl�k var. En k�t� durumda her edge i�in yaparsak toplamda E * (V + logV) lik karma��kl�k olur. Bu O(E * V) olur. Yani totalde O(E * V). priority
g�ncellemek yerine insert i�lemi yapsayd�k karma��kl�k O(E * logV) olurdu ancak memoryden �d�n vermi� olurduk ��nk� heapte ayn� node'dan birden fazla say�da
bulunaca��ndan V'den fazla say�da node bulunabilirdi. Ancak bu heapify operasyonlar�n�n s�resini etkilemezdi ��nk� insert edilen node ayn� etiketteki node'un
�st�nde yer alacak yani di�er node'a hi� bir �ekilde rastgelmeyiz.
array kullan�lan hali i�in ise her tur min mesafeliyi se�mek i�in V iterasyon yap�l�yor. Yani toplamda V^2. se�ilenin kom�ular�n� gezerken her bir kom�u
i�in sabit bi i� var. Yani toplamda sadece kom�ular� gezme i�i var, E. ama E en fazla V*(V-1)/2 olabilece�inden karma��kl�k O(V^2) olur. G�rebilece�in gibi
E, V^2' ye yakla�t�k�a array olan daha iyi ama tersi durum i�in heap olan hali �ok daha iyi.
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
	GRAPH* graph = createGraph(3);
	
	addEdges(graph, 0, 1, 7);
	addEdges(graph, 0, 2, 5);
	addEdges(graph, 1, 2, 2);
	
	//printAdjList(graph);
	dijkstra(graph, 2);
	
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
	
	return 0;
}

void dijkstra(GRAPH* graph, int src){
	int n = graph -> numOfNodes, i;
	int* dist = (int*) malloc(n * sizeof(int));
	
	for(i=0;i<n;i++) dist[i] = INT_MAX;
	dist[src] = 0;
	
	DIST* heap = makeHeap(n, dist);
	DIST u;
	u = heap[src];
	heap[src] = heap[0];
	heap[0] = u;
	NODE* temp;
	
	//for the route of each vertex
	int* route = (int*) malloc(n * sizeof(int));
	for(i=0;i<n;i++) route[i] = -1; 
	
	while(n){
		u = extractMin(heap, &n);
		
		temp = graph -> adjList[u.vertex];
		while(temp != NULL && u.d != INT_MAX){ // "u.d != INT_MAX" is for the case when the graph is not connected
			if(dist[temp -> vertex] > dist[u.vertex] + temp -> weight){
				dist[temp -> vertex] = dist[u.vertex] + temp -> weight;
				changePriority(heap, n, temp -> vertex, dist[temp -> vertex]);
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

//her bir node i�in yaln�zca son durak olan nodelar� kaydetti�imizden -1 g�rene kadar, yani srcye gelene kadar, ilerleyip sonra yazd�rmaya ba�l�yoruz.
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

void changePriority(DIST* heap, int n, int vertex, int p){
	int i = 0;
	while(i < n && heap[i].vertex != vertex) i++;
	if(i == n || heap[i].d == p) return;
	int oldP = heap[i].d;
	heap[i].d = p;
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
	//newNode = createNode(src, weight);
	//newNode->next = graph->adjList[dest];
	//graph->adjList[dest] = newNode;
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
