#include<stdio.h>
#include<stdlib.h>

/*
BFS fonksiyonu, ba�lang�� node'unun (start) dahil oldu�u connected componenti dola��r asl�nda. Mant�k �u: bir node'u ziyaret edip etmedi�imizi
g�steren n boyutlu bir dizimiz (label) var. ba�lang�� olarak verilen node'u label'da 1 yap�yoruz ve queue'ya ekliyoruz. queue'nun mant��� ise
o an bulundu�umuz node'un t�m kom�ular�n� gezmek. o an bulundu�umuz node'un t�m kom�ular�n� gezince onu queue'dan siliyoruz. queue'ya node 
ekleme i�i ��yle: E�er ki o an bulundu�umuz node'un bir kom�usu hala 0 olarak etiketliyse, yani hala ziyaret edilmediyse,o kom�uyu queue'ya
ekliyoruz. Ziyaret edildiyse eklenmeme sebebi �u: e�er ziyaret edildiyse demek ki �nceki iterasyonlardan birinde bu node'a ula��lm�� ve
i�aretlenmi�. Bu sebeple de i�aretlendi�i iterasyonda queue'ya eklenmi� ve sonraki iterasyonlarda kom�ular� gezilmi� veya gezilecektir.
Bu sebeple bir node queue'ya birden fazla kez eklenmemi� oluyor.

queue'ya o anki node'un kom�ular�n� ekledi�imiz i�in ve bu bir queue oldu�undan her iterasyonda head'i gezdi�imiz i�in ba�lad���m�z node'un
kom�ular� ile i�imizi bitirip sonraki turda kom�ular�n�n kom�ular� ile i�imizi bitirip b�yle devam ediyoruz. Yani bulundu�umuz seviyedeki t�m
node'lara bak�yoruz ve bu seviyeyle hesab� kapat�yoruz. DFS ile fark bu.

Ayr�ca ba�lang�� node'unun dahil oldu�u connected component'i dola��yor deme sebebim �u: Ba�lang�� node'u ile aras�nda bir path olan bir
node fonksiyon bitti�inde kesinlikle i�aretlenmi� olur. (�spat�n� contradiction ile g�rebilirsin.) E�er aralar�nda bir path yoksa o node
kesinlikle i�aretlenemez, ula��lamaz.

Analiz: En k�t� durum i�in bir n-node complete graph d���n, yani her node birbiriyle kom�u. Ba�lang�� node'u n-1 iterasyon sonucunda t�m
kom�ular�n�, yani asl�nda graphtaki t�m di�er node'lar�, i�aretler. D��taki while'�n ilk iterasyonu sonucunda ba�lang�� node'u hari� di�er
node'lar queue'ya eklenir. Ancak sonraki turlarda queue'ya ekleme olmaz her turda silinme olur. Ancak complete graph oldu�undan yani her
node'un n-1 kom�usu oldu�undan bundan sonraki n-1 turun her birinde n-1 kez i�erdeki loop �al���r. Yani karma��kl�k n*(n-1)'den n^2 olur.
Bu da edge say�s� ile orant�l�d�r zaten. ��nk� BFS'nin karma��kl��� O(V + E)'dir. Yani en k�t� ihtimalde graphtaki her bir edge'i gezmi�
olucaz. En k�t� ihtimalde t�m edgeleri gezeriz diyorum ��nk� bu bir connected graph de�ilse bize verilen ba�lang�� vertex'inin dahil oldu�u
connected componentteki t�m edgeleri gezeriz. Asl�nda bu undirected bir graph oldu�undan her bir edge'i iki kez gezeriz. ��nk� adjList
�zerinden geziyoruz. O y�zden n-complete graph n*(n-1)/2'nin iki kat� bir runtime oluyor.

Yani her t�rl� ba�lang�� vertex'inin dahil oldu�u cc'nin t�m edgelerini gezeriz ama i�aretleme i�ini veya o i�aretleme k�sm�nda ba�ka bir �ey
de yap�yorsan farketmez o i�i o cc'deki vertex say�s� kadar yapar�z daha fazlas� kadar de�il. Yani en i�teki if'den s�z ediyorum. Mesela count
sonucu o cc'deki vertex'lerin vertex numaralar� toplam� ��kar. 
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
void addEdges(GRAPH*, int, int, int);
int hasEdge(GRAPH*, int, int);

void breadthFirstSearch(GRAPH*, int, int*, int);

int main(){
	int n = 5;
	GRAPH* graph = createGraph(n);
	int* label = (int*) calloc(n, sizeof(int));
	
	addEdges(graph, n, 0, 1);
	addEdges(graph, n, 0, 3);
	addEdges(graph, n, 0, 2);
	addEdges(graph, n, 1, 2);
	addEdges(graph, n, 2, 3);
	
	breadthFirstSearch(graph, n, label, 0);
	
	return 0;
}

void breadthFirstSearch(GRAPH* graph, int n, int* label, int start){
	NODE* temp;
	int head = 0, tail = 0, count=0;
	int* queue = (int*) malloc(n*sizeof(int));
	queue[tail++] = start;
	label[start] = 1;
	while(head < tail){
		printf("%d -> ", queue[head]);
		
		temp = graph -> adjList[queue[head++]];
		while(temp != NULL){
			if(label[temp -> vertex] == 0){
				printf("%d, ", temp-> vertex);
				count += temp -> vertex;
				
				label[temp -> vertex] = 1;
				queue[tail++] = temp -> vertex;
			}
			temp = temp -> next;
		}
		printf("\n");
	}
	free(queue);
	
	printf("%d", count);
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

void addEdges(GRAPH* graph, int n, int src, int dest){
	if(src == dest || src >= n || dest >= n) return;
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
