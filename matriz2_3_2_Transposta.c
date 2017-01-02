#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 2
#define RANDOM 5

/* Matriz transposta */
float** transposta(float ** temp,int n){
  float** aux = malloc( n * sizeof( float* ));
  int i,j;
  
  for ( i = 0; i < n; i++ ){
    aux[i] = malloc( n * sizeof(float ));
  }

  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      aux[i][j]=temp[j][i];
    }
  }

  return aux;
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

/* Multiplicador de matrizes*/

float** multMatriz(float **a, float **b, float **res, int n ) {
	
	int i, j, k;
	float** matrizBtrans = transposta(b,n);
    float** matrizAtrans = transposta(a,n);
    
	for ( j = 0; j < n; j++){
		for ( k = 0; k < n; k++){
			for ( i= 0; i < n; i++){
				res[j][i] += matrizAtrans[k][i] * b[j][k];
			}
		}
	}
    free(matrizBtrans);
   res=transposta(res,n);
    return res;
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
	matrizR=multMatriz(matrizA,matrizB,matrizR,N);
	imprimeMatriz(matrizA,N);
	imprimeMatriz(matrizB,N);
	imprimeMatriz(matrizR,N);
	free(matrizA);
	free(matrizB);
	free(matrizR);
 	return 1;
 }
