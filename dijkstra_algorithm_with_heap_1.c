#include <stdio.h>
#include <stdlib.h>

/*
Ana mantýk þu: source node'un komþularýndan minimum uzaklýkta olan için bu uzaklýk kesinlikle source'a olan minimum uzaklýktýr. src'nin baþka komþusu
üzerinden bu node'a daha kýsa mesafeyle ulaþamazsýn. Diðer komþular için min mesafe þu anki mesafe de olabilir, ara bir node'dan geçen yolun mesafesi de...
Mesela src'nin iki komþusu olsun biri 2 diðeri 4 mesafede. bu ikisi arasýnda da 1 mesafe olsun. 4 mesafedeki komþuya diðer komþu üzerinden 3 birimle ulaþrýz.
Sonuç olarak min mesafedeki komþuyu mesafesi bilinen nodelar kümesine alabiliriz. Ve onun komþularý için ayný tarifeyi uygularýz ta ki tüm nodelarý bilinen 
bölgeye alana kadar. Bilinen bölgeye her zaman yeni alýnan node'un min mesafedeki komþusunu almýyoruz. Böyle yapmamýz lazým yoksa sonuç hatalý olabilir. mesela
src'nin komþularýndan min olaný aldýk. Buna u diyelim. Ancak u'nun kendi komþularýnýn mesafelerini güncelledikten sonra bu node'larýn src'ye mesafeleri, 
src'nin u ile arasýnda edge olmayan baþka bir komþusunun src'ye mesafesinden küçük olmayabilir. Bu durumda minheap'in tepesinde bu node olur ve bunu extract
ederiz. Eðer böyle deðil de u'nun komuþularýndan min olaný seçseydik bu o node'un gerçek min deðeri olmayabilirdi. Çünkü bu seçilen node ile aslýnda minimum
durumda olan node arasýnda bu node'un min deðerini deðiþtirebilecek bir edge olabilir.

Complexity: her turda bir kere extractMin yapýlýyor logV kadarlýk bir karmaþýklýðý var. V tur olacaðýndan V*logV eder. ayrýca seçilen node'un komþularý için
mesafe güncellemesi yapýlýyor ve yine logV karmaþýlýkta. Ancak heapte bu node'u bulmak için bir linear search yapýlýyor. Bu sebeple aslýnda V + logV 'lik bir
karmaþýklýk var. En kötü durumda her edge için yaparsak toplamda E * (V + logV) lik karmaþýklýk olur. Bu O(E * V) olur. Yani totalde O(E * V). priority
güncellemek yerine insert iþlemi yapsaydýk karmaþýklýk O(E * logV) olurdu ancak memoryden ödün vermiþ olurduk çünkü heapte ayný node'dan birden fazla sayýda
bulunacaðýndan V'den fazla sayýda node bulunabilirdi. Ancak bu heapify operasyonlarýnýn süresini etkilemezdi çünkü insert edilen node ayný etiketteki node'un
üstünde yer alacak yani diðer node'a hiç bir þekilde rastgelmeyiz.
array kullanýlan hali için ise her tur min mesafeliyi seçmek için V iterasyon yapýlýyor. Yani toplamda V^2. seçilenin komþularýný gezerken her bir komþu
için sabit bi iþ var. Yani toplamda sadece komþularý gezme iþi var, E. ama E en fazla V*(V-1)/2 olabileceðinden karmaþýklýk O(V^2) olur. Görebileceðin gibi
E, V^2' ye yaklaþtýkça array olan daha iyi ama tersi durum için heap olan hali çok daha iyi.
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
	    printf("%d -> ", i); // hangi node'un komþularý.
	    NODE* temp = graph -> adjList[i];
	    while(temp != NULL) {
	        printf("(%d, w: %d), ", temp -> vertex, temp -> weight);
	        temp = temp -> next;
	    }
	    printf("\n");
	}
}
