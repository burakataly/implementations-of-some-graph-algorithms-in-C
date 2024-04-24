#include<stdio.h>
#include<stdlib.h>

/*
BFS'deki queue mantýðýnýn aksine DFS'de stack mantýðý var. Yani bir node'un komþularýna bakarken iþaretlenmemiþ bir tane görürsen bunu
iþaretleyip þu anda komþularýna baktýðýmýz node'daki iþimizi durdurup yeni iþaretlenen node'un komþularýna bakmaya baþlýycaz. Ve ancak bu yeni
node'la iþimiz bitince önceki node'un komþularýna bakmaya devam edebiliriz. Ayrýca önceki node'a geri döndüðümüzde kendi komþularý, baþkalarý ile
komþuluklarý varsa, çoktan ziyaret edilmiþ olur.
stack mantýðýný recursion bizim yerimize görür.

Tam da bu sebeple shortest path distance bulmak için DFS'yi kullanamazsýn. Mesela þu senaryoyu düþün: src'nin bir komþusu için, a, bir recursion
çaðrý yaptýn ve bu çaðrý sýrasýnda src'nin baþka bir komþusu, b, a'nýn komþusu ise a'nýn çaðrýsý sýrasýnda iþ src'ye kalmadan b iþaretlenmiþ
olur. Yani src'ye geri döndüðümüzde a veya a'nýn komþularý b'yi nasýl etiketlediyse src ve b mesafesi o olacak. Yani her türlü 1'den büyük
olacak. Ancak src-b arasý mesafe 1 olmalý çünkü komþular.

Analiz: baþlangýç olarak verilen node'un dahil olduðu cc'nin tüm node'larýna ulaþýr ve eðer undirected graphsa her bir edge'i iki kez kontrol
eder. Yani fonksiyon çaðrýlarýný constant kanul edersen, karmaþýklýk BFS'de olduðu gibi O(V + E).

Bir undirected graph'taki cc'leri bulmak için BFS ve DFS ayný iþlevi görürler.
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
