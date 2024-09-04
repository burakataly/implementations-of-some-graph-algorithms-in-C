#include<stdio.h>
#include<stdlib.h>
/*bize verilen matriste alt, �st, sa�, sol ba�lant�l� ise onu tek bir par�a olarak g�r�yoruz ve etiketliyoruz. ilk par�ay� 1 ile, ikinciyi 2 ile vs.
 
 E�er 8 k��eye de bakmak istersek alttaki d�rtl� recursive �a�r�lar da al�nabilir. Ayr�ca bu recursive �a�r�lar for ile al�nabilir bunun i�in artma azalma 
 k�s�mlar�n� bir dizide tutup d�ng� d�nd�k�e recursive �a�r� yap�l�r. karma��kl�k olarak fark yok ama okunu� daha rahat olur.*/  

void labeler(int x, int y, int matrix[][10], int label[][10], int n, int m, int component){

	if(x < 0 || x >= n) return;
	if(y < 0 || y >= m) return;
	if(label[x][y] || !matrix[x][y]) return; //�oktan etiketlenmi�se veya matrix[x][y]==0
	
	label[x][y]=component;
	
	labeler(x+1, y, matrix, label, n, m, component);
	labeler(x-1, y, matrix, label, n, m, component);
	labeler(x, y+1, matrix, label, n, m, component);
	labeler(x, y-1, matrix, label, n, m, component);
	
/*	labeler(x+1, y+1, matrix, label, n, m, component); 
	labeler(x-1, y-1, matrix, label, n, m, component);
	labeler(x-1, y+1, matrix, label, n, m, component);
	labeler(x+1, y-1, matrix, label, n, m, component); */
}


int main(){
	int i, j, n=8, m=10, component=1;
	int matrix[8][10] ={{1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
						{1, 1, 0, 0, 1, 1, 1, 1, 0, 1},
						{0, 1, 1, 0, 1, 1, 1, 0, 1, 1},
						{0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
						{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
						{0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
						{1, 1, 1, 1, 0, 1, 1, 0, 1, 0},
						{0, 1, 0, 0, 0, 0, 1, 0, 0, 0}};
	int label[8][10] = {{0}};					
	
	/* bu for t�m matrisi dola�mak i�in. buradan yap�lan her bir �a�r� sonunda bir par�a bulunmu� olacak ��nk� buradan yap�lan bir �a�r� sonucunda 
	gidebilece�i yere kadar gidecek. ayr�ca if �art� da �a�r�y� h�cre mevcutsa ve h�cre i�in par�a bulunmam��sa yapmak i�in */
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			if(matrix[i][j] && !label[i][j]) labeler(i, j, matrix, label, n, m, component++);
		}
	}
	
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			printf("%d ", label[i][j]);
		}
		printf("\n");
	}
	return 0;
}
