#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 5
#define RANDOM 100



/* Multiplicador de matrizes*/

void multMatriz(float **a, float **b, float **res, int n ) {
	
	int i, j, k;
	for ( i = 0; i < n; i++){
		for ( k = 0; k < n; k++){
			for ( j= 0; j < n; j++){
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}




void imprimeMatriz(float **m, int n){
	int i, j;

	for( i = 0 ; i < n ; i++ ){
		for( j = 0 ; j < n ; j++ ){
			printf("%f ", m[i][j]);
		}
		printf("\n");
	}
}



 int main() {

	int i, j;
 	float **matrizA;
 	float **matrizB;
 	float **matrizR; //Matriz resultado


	if (( matrizA = malloc( N*sizeof( float* ))) == NULL ) return 0; 
	if (( matrizB = malloc( N*sizeof( float* ))) == NULL ) return 0; 
	if (( matrizR = malloc( N*sizeof( float* ))) == NULL ) return 0; 

	for ( i = 0; i < N; i++ ){
		if (( matrizA[i] = malloc( N*sizeof(float ) )) == NULL ) return 0; 
		if (( matrizB[i] = malloc( N*sizeof(float) )) == NULL ) return 0; 
		if (( matrizR[i] = malloc( N*sizeof(float) )) == NULL ) return 0; 
	}

	//Gerar matrizes com elementos aleatorios
	srand ( time(NULL) );
	for ( i = 0; i < N; i++) {
	 for ( j = 0; j < N; j++) {
		 matrizA[i][j] = ((float)rand()/(float)(RAND_MAX)) * RANDOM;
		 matrizB[i][j] = 1;
		 matrizR[i][j]=0;

	 }
	}
	printf("\n");
	multMatriz(matrizA,matrizB,matrizR,N);
	imprimeMatriz(matrizR,N);
	free(matrizA);
	free(matrizB);
	free(matrizR);
 	return 1;
 }
