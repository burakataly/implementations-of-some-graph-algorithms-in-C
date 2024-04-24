#include<stdio.h>
#include<stdlib.h>

/*
BFS fonksiyonu, baþlangýç node'unun (start) dahil olduðu connected componenti dolaþýr aslýnda. Mantýk þu: bir node'u ziyaret edip etmediðimizi
gösteren n boyutlu bir dizimiz (label) var. baþlangýç olarak verilen node'u label'da 1 yapýyoruz ve queue'ya ekliyoruz. queue'nun mantýðý ise
o an bulunduðumuz node'un tüm komþularýný gezmek. o an bulunduðumuz node'un tüm komþularýný gezince onu queue'dan siliyoruz. queue'ya node 
ekleme iþi þöyle: Eðer ki o an bulunduðumuz node'un bir komþusu hala 0 olarak etiketliyse, yani hala ziyaret edilmediyse,o komþuyu queue'ya
ekliyoruz. Ziyaret edildiyse eklenmeme sebebi þu: eðer ziyaret edildiyse demek ki önceki iterasyonlardan birinde bu node'a ulaþýlmýþ ve
iþaretlenmiþ. Bu sebeple de iþaretlendiði iterasyonda queue'ya eklenmiþ ve sonraki iterasyonlarda komþularý gezilmiþ veya gezilecektir.
Bu sebeple bir node queue'ya birden fazla kez eklenmemiþ oluyor.

queue'ya o anki node'un komþularýný eklediðimiz için ve bu bir queue olduðundan her iterasyonda head'i gezdiðimiz için baþladýðýmýz node'un
komþularý ile iþimizi bitirip sonraki turda komþularýnýn komþularý ile iþimizi bitirip böyle devam ediyoruz. Yani bulunduðumuz seviyedeki tüm
node'lara bakýyoruz ve bu seviyeyle hesabý kapatýyoruz. DFS ile fark bu.

Ayrýca baþlangýç node'unun dahil olduðu connected component'i dolaþýyor deme sebebim þu: Baþlangýç node'u ile arasýnda bir path olan bir
node fonksiyon bittiðinde kesinlikle iþaretlenmiþ olur. (Ýspatýný contradiction ile görebilirsin.) Eðer aralarýnda bir path yoksa o node
kesinlikle iþaretlenemez, ulaþýlamaz.

Analiz: En kötü durum için bir n-node complete graph düþün, yani her node birbiriyle komþu. Baþlangýç node'u n-1 iterasyon sonucunda tüm
komþularýný, yani aslýnda graphtaki tüm diðer node'larý, iþaretler. Dýþtaki while'ýn ilk iterasyonu sonucunda baþlangýç node'u hariç diðer
node'lar queue'ya eklenir. Ancak sonraki turlarda queue'ya ekleme olmaz her turda silinme olur. Ancak complete graph olduðundan yani her
node'un n-1 komþusu olduðundan bundan sonraki n-1 turun her birinde n-1 kez içerdeki loop çalýþýr. Yani karmaþýklýk n*(n-1)'den n^2 olur.
Bu da edge sayýsý ile orantýlýdýr zaten. Çünkü BFS'nin karmaþýklýðý O(V + E)'dir. Yani en kötü ihtimalde graphtaki her bir edge'i gezmiþ
olucaz. En kötü ihtimalde tüm edgeleri gezeriz diyorum çünkü bu bir connected graph deðilse bize verilen baþlangýç vertex'inin dahil olduðu
connected componentteki tüm edgeleri gezeriz. Aslýnda bu undirected bir graph olduðundan her bir edge'i iki kez gezeriz. Çünkü adjList
üzerinden geziyoruz. O yüzden n-complete graph n*(n-1)/2'nin iki katý bir runtime oluyor.

Yani her türlü baþlangýç vertex'inin dahil olduðu cc'nin tüm edgelerini gezeriz ama iþaretleme iþini veya o iþaretleme kýsmýnda baþka bir þey
de yapýyorsan farketmez o iþi o cc'deki vertex sayýsý kadar yaparýz daha fazlasý kadar deðil. Yani en içteki if'den söz ediyorum. Mesela count
sonucu o cc'deki vertex'lerin vertex numaralarý toplamý çýkar. 
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
