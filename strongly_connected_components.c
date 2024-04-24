#include<stdio.h>
#include<stdlib.h>

/*
Correctness of Kosaraju's SCC algortihm:

Bir directed graphta SCC'ler birbirlerine ya bir yönlü path'la baðlýlardýr ya da hiç baðlý deðillerdir. Ýki yönlü bir path'la baðlý olsalardý
bunlar SCC deðil bir SCC'nin iki tane strongly connected subgraph'ý olurlardý. Þimdi bir graphtaki her bir SCC'yi kendi içinde bir bütün
olarak düþünürsek ortaya bir DAG çýkar (metagraph). DAG olmamasý için bir cycle olmasý gerekirdi. Yani iki SCC bir directed cycle'ýn içinde
olmalýydý. Ama böyle bir durum bunlarýn SCC olmadýðýný gösterir bir çeliþki yaratýr. 

Þimdi DAG'larla ilgili özel duruma gelelim: Bildiðin gibi her DAG bir Topological Ordering (TO) ile ifade edilebilir. DAG, TO þeklinde ifade
edildiðinde bildiðin gibi en soldaki node her node'a ulaþýr ama en saðdaki node hiçbir node'a ulaþamaz. Eðer bu graph'ýn transpozunu alýrsak
yani arclarýn yönünü ters çevirirsek bu durumda az önce hiçbir node'a ulaþamayan en saðdaki node her node'a ulaþýrken en soldaki hiçbirine
ulaþamaz. TO'nun simetriði alýnmýþ olur aslýnda. Metagraph için düþünürsek transpoz iþlemi SCC'leri etkilemez. Çünkü SCC'lerin herbiri bir directed
cycle aslýnda. Transpoz sonrasý sadece bu cycle'ýn yönü deðiþir.

Þimdi DFS yardýmýyla graphtaki SCC'leri bulma kýsmýna gelirsek elimizde þöyle bir sýkýntý var: Bir undirected graphýn CC'lerini bulurken
izlediðimiz yolu burada uygularsak, TO'da en soldaki SCC'nin bir node'unu baþlangýç node'u alarak verirsen bu node kendi ait olduðu SCC'nin
elemanlarýna ek olarak graphtaki her node'a ulaþýr ve etiketler. Ve sonuçta hatalý olarak elimizdeki graph tek bir SCC'den oluþuyormuþ sonucu
çýkar. Yani sýradan DFS'yi kullanamayýz. Peki ne yapýcaz?

Eðer ki en saðdan sýralý þekilde TO'daki SCC'lerin node'larýndan birini DFS'ye versek sonuç doðru çýkardý. Çünkü saðdaki etiketlenirken
solundaki SCC'ye ulaþamazdý ve iþi bitince soldakinin node'u için DFS yaptýðýmýzda saðýndaki SCC'ler etiketlendiðinden oraya gidemezdi ve
solundaki SCC'ye de gidemediðinden doðru þekilde etiketleme yapardý. Bunu baþarmak için þöyle bir yol izleyebiliriz: Graph üzerinde klasik bir
DFS search yapýp bu DFS'lerin sonunda gönderilen baþlangýç node'unu bir stack'e eklersek, tüm graph gezildikten sonra stack'in en üstünde 
TO'nun en solundaki SCC'den bir node olur. Çünkü en soldaki SCC'den bir node verildiði zaman bu node graph'taki her node'u dolaþtýktan sonra
DFS'nin sonunda stack'e eklenir. Bu arada klasik DFS'ye nerden baþlandýðýnýn önemi yok. Yani ilk TO'nun en saðýndaki SCC'nin node'unu da versen
tüm graph tarandýðýnda stack'ýn en üstünde TO'nun en solundaki SCC'nin bir elemaný bulunur.

Þimdi en tatlý noktaya geldik: Tüm node'larý bu þekilde stack'e kaydettikten sonra elimizdeki graphýn transpozunu aldýðýmda TO'nun simetrisini
almýþ olurum ve stack'in en üstündeki node'un bulunduðu SCC artýk TO'nun en saðýndaki SCC olur. Ve ben bu transpoz graph için bir DFS
baþlattýrsam ve DFS'ye vereceðim baþlangýç node'larýný stack'ten verirsem üstteki paragrafýn baþýndaki olayý baþarmýþ olurum.

Yani üç aþama var: 1- transpoz alma, 2- stack'e node eklemek için bir DFS, 3- SCC'leri etiketlemek için bir DFS

NOT: 2. aþamadaki DFS'ye transpoz graphý, 3. aþamadakine normal graph'ý versen de sonuç deðiþmez çünkü transpoz iþlemi SCC'leri etkilemez.
*/

typedef struct n{
	int x;
	struct n* next;
}STACK;

void push(STACK**, int);
int pop(STACK**);

int** createGraph(int);
void addEdge(int**, int, int, int);
void removeEdge(int**, int, int, int);

void depthFirstSearch1(int**, int, int*, int, STACK**);
void depthFirstSearch2(int**, int, int*, int, int);
int** transpose(int**, int);
int stronglyConnectedComponents(int**, int);

int main(){
	int n = 8, i, j;
	int** adjMatrix = createGraph(n);
	
	addEdge(adjMatrix, n, 0, 1);
	addEdge(adjMatrix, n, 2, 3);
	addEdge(adjMatrix, n, 2, 0);
	addEdge(adjMatrix, n, 1, 2);
	addEdge(adjMatrix, n, 3, 4);
	addEdge(adjMatrix, n, 4, 7);
	addEdge(adjMatrix, n, 4, 5);
	addEdge(adjMatrix, n, 5, 6);
	addEdge(adjMatrix, n, 6, 7);
	addEdge(adjMatrix, n, 6, 4);
	
	printf("\n%d", stronglyConnectedComponents(adjMatrix, n));
	
	for(i=0;i<n;i++){
		free(adjMatrix[i]);
	}
	free(adjMatrix);
	
	return 0;
}

int stronglyConnectedComponents(int** adjMatrix, int n){
	int i, labeler = 1;
	int* label = (int*) calloc(n, sizeof(int));
	STACK* head = 0;
	int** transAdj = transpose(adjMatrix, n);
	
	//stacki doldurmak için olan DFS.
	for(i=0;i<n;i++){
		if(label[i] == 0){
			depthFirstSearch1(adjMatrix, n, label, i, &head);
		}
	}
	
	for(i=0;i<n;i++) label[i] = 0;
	
	//etiketlemek için olan DFS. Baþlangýç nodelarýný burada stackten veriyoruz ama recursive çaðrýlarda normalde CC bulurken yaptýðýmýz gibi
	//tüm komþularýný geziyoruz.
	while(head != NULL){
		i = pop(&head);
		if(label[i] == 0){
			depthFirstSearch2(transAdj, n, label, i, labeler++);
		}
	}
	
	for(i=0;i<n;i++) printf("%d ", label[i]);
	free(label);
	for(i=0;i<n;i++) free(transAdj[i]);
	free(transAdj);
	
	return labeler-1; // graphta kaç tane SCC olduðunu döndürür.
}

void depthFirstSearch1(int** adjMatrix, int n, int* label, int start, STACK** head){
	int i;
	
	label[start] = 1;
	for(i=0;i<n;i++){
		if(adjMatrix[start][i] != 0 && label[i] == 0){
			depthFirstSearch1(adjMatrix, n, label, i, head);
		}
	}
	
	push(head, start);
}

void depthFirstSearch2(int** adjMatrix, int n, int* label, int start, int labeler){
	int i;
	
	label[start] = labeler;
	for(i=0;i<n;i++){
		if(adjMatrix[start][i] != 0 && label[i] == 0){
			depthFirstSearch2(adjMatrix, n, label, i, labeler);
		}
	}
}

int** transpose(int** adjMatrix, int n){
	int i, j;
	int **transAdj = (int**) malloc(n*sizeof(int*));
	for(i=0;i<n;i++){
		transAdj[i] = (int*) malloc(n*sizeof(int));
		for(j=0;j<n;j++) transAdj[i][j] = adjMatrix[j][i];
	}
	return transAdj;
}

int** createGraph(int n){
	int i;
	int** adjMatrix = (int**) malloc(n*sizeof(int*));
	for(i=0;i<n;i++) adjMatrix[i] = (int*) calloc(n, sizeof(int));
	return adjMatrix;
}

void addEdge(int** adjMatrix, int n, int src, int dest){
	if(src == dest || src >= n || dest >= n) return;
	adjMatrix[dest][src] = 1;
}

void removeEdge(int** adjMatrix, int n, int src, int dest){
	if(src == dest || src >= n || dest >= n) return;
	adjMatrix[src][dest] = 0;
}

void push(STACK **head, int i){
	STACK *temp = (STACK*) malloc(sizeof(STACK));
	temp -> x = i;
	temp -> next = *head;
	*head = temp;
}

int pop(STACK **head){
	if(*head != NULL){
		STACK *temp = *head;
		*head = (*head) -> next;
		int a = temp -> x;
		free(temp);
		return a;
	}
}
