#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <memory.h>
#include "papi.h"

#define N 200
#define RANDOM 100

static void test_fail(char *file, int line, char *call, int retval);

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
	 
	 //PAPI
	float real_time, proc_time,mflops;
  	long long flpins;
  	int retval;

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
	
	 /* Setup PAPI library and begin collecting data from the counters */
    if((retval=PAPI_flops( &real_time, &proc_time, &flpins, &mflops))<PAPI_OK)
    test_fail(__FILE__, __LINE__, "PAPI_flops", retval);

	multMatriz(matrizA,matrizB,matrizR,N);
	
	/* Collect the data into the variables passed in */
   if((retval=PAPI_flops( &real_time, &proc_time, &flpins, &mflops))<PAPI_OK)
    test_fail(__FILE__, __LINE__, "PAPI_flops", retval);

	printf("\n");
	printf("Real_time:\t%f\nProc_time:\t%f\nTotal flpins:\t%lld\nMFLOPS:\t\t%f\n",
    real_time, proc_time, flpins, mflops);
    printf("%s\tPASSED\n", __FILE__);
	free(matrizA);
	free(matrizB);
	free(matrizR);
 	PAPI_shutdown();
  	exit(0);
 }
 
  static void test_fail(char *file, int line, char *call, int retval){
    printf("%s\tFAILED\nLine # %d\n", file, line);
    if ( retval == PAPI_ESYS ) {
        char buf[128];
        memset( buf, '\0', sizeof(buf) );
        sprintf(buf, "System error in %s:", call );
        perror(buf);
    }
    else if ( retval > 0 ) {
        printf("Error calculating: %s\n", call );
    }
    else {
        printf("Error in %s: %s\n", call, PAPI_strerror(retval) );
    }
    printf("\n");
    exit(1);
}
