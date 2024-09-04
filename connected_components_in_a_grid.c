#include<stdio.h>
#include<stdlib.h>
/*bize verilen matriste alt, üst, sað, sol baðlantýlý ise onu tek bir parça olarak görüyoruz ve etiketliyoruz. ilk parçayý 1 ile, ikinciyi 2 ile vs.
 
 Eðer 8 köþeye de bakmak istersek alttaki dörtlü recursive çaðrýlar da alýnabilir. Ayrýca bu recursive çaðrýlar for ile alýnabilir bunun için artma azalma 
 kýsýmlarýný bir dizide tutup döngü döndükçe recursive çaðrý yapýlýr. karmaþýklýk olarak fark yok ama okunuþ daha rahat olur.*/  

void labeler(int x, int y, int matrix[][10], int label[][10], int n, int m, int component){

	if(x < 0 || x >= n) return;
	if(y < 0 || y >= m) return;
	if(label[x][y] || !matrix[x][y]) return; //çoktan etiketlenmiþse veya matrix[x][y]==0
	
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
	
	/* bu for tüm matrisi dolaþmak için. buradan yapýlan her bir çaðrý sonunda bir parça bulunmuþ olacak çünkü buradan yapýlan bir çaðrý sonucunda 
	gidebileceði yere kadar gidecek. ayrýca if þartý da çaðrýyý hücre mevcutsa ve hücre için parça bulunmamýþsa yapmak için */
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
