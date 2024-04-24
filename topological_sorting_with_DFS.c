#include<stdio.h>
#include<stdlib.h>

/*
her seferinde sink olan node'u ordering dizisine ekliyorum. Sink olmasý için de gidecek komþusunun olmamasý gerekir. Bu yüzden DFS'nin sonunda
ekleme yapýyorum. ekledikten sonra ayrýca da iþaretlediðim için aslýnda graph'tan silmiþ oluyorum. Yani bir node ilk baþta sink deðilse ve 
komþularý için DFS çaðýrdýysa bile komþularý eklenip return olduklarýnda kendisi bir sink haline gelmiþ olacak ve onu gerekli index'e
yerleþtirmiþ olucaz. index için de þöyle bir durum var: baþlangýç node'undan ne kadar ilerlersek (sink'e gelene kadar ilerliycek) index o kadar
azalýcak ve bunu return ederek takip edebiliyoruz. pointer kullansan return ile uðraþmazdýn. Veya stack kullanabilirdin.

Eðer ki graph DAG deðilse bildiðin gibi bir ordering yapamayýz. Ayrýca etiketleme iþini DFS sonunda yapsaydýn DAG olmayan bir graph için sonsuz
döngüye girerdin. DAG deðilse de bir ordering üretir bu algo ama tabi ki de dikkate alýnmayacak bir ordering...

Correctness: u ve v diye iki node'umuz var ve arc u'dan v'ye doðru. Bu durumda v'nin index'i u'nunkinden büyük olmalý. Bunun için iki case var:
case 1: u, v'den önce ziyaret edildiyse; u, v için bir DFS çaðrýsý yapar ve bunun sonucunda v her türlü u'dan daha büyük bir index'e yerleþir.
case 2: v, u'dan önce ziyaret edildiyse ve bunun bir DAG olduðunu kabul ettiðimizde v'den u'ya bir path olmamasý lazým. Bu sebeple de v, u'ya
ulaþan bir recursive zincir baþlatamaz. v'yi indexine yerleþtirip v'yi çaðýran node'a geri döndükten sonra u'ya gidebiliriz ve sonuç olarark bu
case'te de u daha düþük bir index'e yerleþmiþ olur.

*/

int** createGraph(int);
void addEdge(int**, int, int, int);
void removeEdge(int**, int, int, int);

int depthFirstSearch(int**, int, int*, int*, int, int);
int* topologicalOrdering(int**, int);

int main(){
	int n = 5, i;
	int** adjMatrix = createGraph(n);
	int* ordering;
	addEdge(adjMatrix, n, 0, 1);
	addEdge(adjMatrix, n, 0, 3);
	addEdge(adjMatrix, n, 0, 2);
	addEdge(adjMatrix, n, 1, 2);
	addEdge(adjMatrix, n, 2, 3);
	
	ordering = topologicalOrdering(adjMatrix, n);
	
	for(i=0;i<n;i++){
		printf("%d ", ordering[i]);
		free(adjMatrix[i]);
	}
	free(adjMatrix);
	free(ordering);
	
	return 0;
}

int* topologicalOrdering(int** adjMatrix, int n){
	int* label = (int*) calloc(n, sizeof(int));
	int* ordering = (int*) malloc(n*sizeof(int));
	int i, index = n-1;
	
	for(i=0;i<n;i++){
		if(!label[i]){
			index = depthFirstSearch(adjMatrix, n, label, ordering, i, index);
		}
	}
	free(label);
	return ordering;
}

int depthFirstSearch(int** adjMatrix, int n, int* label, int* ordering, int start, int index){
	if(index < 0) return;
	int i;
	label[start] = 1;
	for(i=0;i<n;i++){
		if(adjMatrix[start][i] != 0 && label[i] == 0){
			index = depthFirstSearch(adjMatrix, n, label, ordering, i, index);
		}
	}
	ordering[index--] = start;
	return index;
}

int** createGraph(int n){
	int i;
	int** adjMatrix = (int**) malloc(n*sizeof(int*));
	for(i=0;i<n;i++) adjMatrix[i] = (int*) calloc(n, sizeof(int));
	return adjMatrix;
}

void addEdge(int** adjMatrix, int n, int src, int dest){
	if(src == dest || src >= n || dest >= n) return;
	adjMatrix[src][dest] = 1;
}

void removeEdge(int** adjMatrix, int n, int src, int dest){
	if(src == dest || src >= n || dest >= n) return;
	adjMatrix[src][dest] = 0;
}

