#include<stdio.h>
#include<stdlib.h>

/*
her seferinde sink olan node'u ordering dizisine ekliyorum. Sink olmas� i�in de gidecek kom�usunun olmamas� gerekir. Bu y�zden DFS'nin sonunda
ekleme yap�yorum. ekledikten sonra ayr�ca da i�aretledi�im i�in asl�nda graph'tan silmi� oluyorum. Yani bir node ilk ba�ta sink de�ilse ve 
kom�ular� i�in DFS �a��rd�ysa bile kom�ular� eklenip return olduklar�nda kendisi bir sink haline gelmi� olacak ve onu gerekli index'e
yerle�tirmi� olucaz. index i�in de ��yle bir durum var: ba�lang�� node'undan ne kadar ilerlersek (sink'e gelene kadar ilerliycek) index o kadar
azal�cak ve bunu return ederek takip edebiliyoruz. pointer kullansan return ile u�ra�mazd�n. Veya stack kullanabilirdin.

E�er ki graph DAG de�ilse bildi�in gibi bir ordering yapamay�z. Ayr�ca etiketleme i�ini DFS sonunda yapsayd�n DAG olmayan bir graph i�in sonsuz
d�ng�ye girerdin. DAG de�ilse de bir ordering �retir bu algo ama tabi ki de dikkate al�nmayacak bir ordering...

Correctness: u ve v diye iki node'umuz var ve arc u'dan v'ye do�ru. Bu durumda v'nin index'i u'nunkinden b�y�k olmal�. Bunun i�in iki case var:
case 1: u, v'den �nce ziyaret edildiyse; u, v i�in bir DFS �a�r�s� yapar ve bunun sonucunda v her t�rl� u'dan daha b�y�k bir index'e yerle�ir.
case 2: v, u'dan �nce ziyaret edildiyse ve bunun bir DAG oldu�unu kabul etti�imizde v'den u'ya bir path olmamas� laz�m. Bu sebeple de v, u'ya
ula�an bir recursive zincir ba�latamaz. v'yi indexine yerle�tirip v'yi �a��ran node'a geri d�nd�kten sonra u'ya gidebiliriz ve sonu� olarark bu
case'te de u daha d���k bir index'e yerle�mi� olur.

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

