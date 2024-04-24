#include<stdio.h>
#include<stdlib.h>

/*
BFS'deki queue mant���n�n aksine DFS'de stack mant��� var. Yani bir node'un kom�ular�na bakarken i�aretlenmemi� bir tane g�r�rsen bunu
i�aretleyip �u anda kom�ular�na bakt���m�z node'daki i�imizi durdurup yeni i�aretlenen node'un kom�ular�na bakmaya ba�l�ycaz. Ve ancak bu yeni
node'la i�imiz bitince �nceki node'un kom�ular�na bakmaya devam edebiliriz. Ayr�ca �nceki node'a geri d�nd���m�zde kendi kom�ular�, ba�kalar� ile
kom�uluklar� varsa, �oktan ziyaret edilmi� olur.
stack mant���n� recursion bizim yerimize g�r�r.

Tam da bu sebeple shortest path distance bulmak i�in DFS'yi kullanamazs�n. Mesela �u senaryoyu d���n: src'nin bir kom�usu i�in, a, bir recursion
�a�r� yapt�n ve bu �a�r� s�ras�nda src'nin ba�ka bir kom�usu, b, a'n�n kom�usu ise a'n�n �a�r�s� s�ras�nda i� src'ye kalmadan b i�aretlenmi�
olur. Yani src'ye geri d�nd���m�zde a veya a'n�n kom�ular� b'yi nas�l etiketlediyse src ve b mesafesi o olacak. Yani her t�rl� 1'den b�y�k
olacak. Ancak src-b aras� mesafe 1 olmal� ��nk� kom�ular.

Analiz: ba�lang�� olarak verilen node'un dahil oldu�u cc'nin t�m node'lar�na ula��r ve e�er undirected graphsa her bir edge'i iki kez kontrol
eder. Yani fonksiyon �a�r�lar�n� constant kanul edersen, karma��kl�k BFS'de oldu�u gibi O(V + E).

Bir undirected graph'taki cc'leri bulmak i�in BFS ve DFS ayn� i�levi g�r�rler.
*/

int** createGraph(int);
void addEdge(int**, int, int, int);
void removeEdge(int**, int, int, int);

void depthFirstSearch(int**, int, int*, int);

int main(){
	int n = 5, i;
	int** adjMatrix = createGraph(n);
	int* label = (int*) calloc(n, sizeof(int));
	addEdge(adjMatrix, n, 0, 1);
	//addEdge(adjMatrix, n, 0, 3);
	addEdge(adjMatrix, n, 0, 2);
	addEdge(adjMatrix, n, 1, 2);
	addEdge(adjMatrix, n, 2, 3);
	
	depthFirstSearch(adjMatrix, n, label, 0);
	
	for(i=0;i<n;i++){
		printf("%d ", label[i]);
		free(adjMatrix[i]);	
	}
	free(adjMatrix);
	free(label);
	
	return 0;
}

void depthFirstSearch(int** adjMatrix, int n, int* label, int start){
	int i, count = 0;
	
	label[start] = 1;
	for(i=0;i<n;i++){
		if(adjMatrix[start][i] != 0 && label[i] == 0){
			depthFirstSearch(adjMatrix, n, label, i);
			count++;
		}
	}
	printf("%d\n", count);
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
