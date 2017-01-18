#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <memory.h>
#include "papi.h"

#define N 50
#define RANDOM 100
#define NUM_EVENTS 4

static void test_fail(char *file, int line, char *call, int retval);

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

void multMatriz(float **a, float **b, float **res, int n ) {
    
    int i, j, k, l;
    float valor;
    float *matrizC = malloc( n*sizeof( float));
    float** matrizBtrans = transposta(b,n);

    for ( i = 0; i < n; i++){
        for ( j = 0; j < n; j++){
            valor=res[i][j];
            
            for ( k = 0; k < n; k++){
                matrizC[k] = a[i][k] *  matrizBtrans[j][k];
            }
            
            for (l=0;l<n;l++) {
                valor += matrizC[l];
            }
            
            res[i][j]=valor;
        }
    }
    free(matrizBtrans);
    free(matrizC);
}





int main() {
    
	   /*Eventos Papi*/
    int Events[NUM_EVENTS]= {PAPI_L1_TCM, PAPI_L2_TCM, PAPI_L3_TCM, PAPI_TOT_INS};
    int EventSet = PAPI_NULL;
    long long values[NUM_EVENTS];
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
    
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        test_fail(__FILE__, __LINE__, "PAPI_LIBRARY_INIT_ERROR", retval);
 	  }
    
    /* Allocate space for the new eventset and do setup */
    if (PAPI_create_eventset(&EventSet) != PAPI_OK) {
        printf("Failed to allocate space for the new eventset and do setup\n ");
        exit(0);
    }
    
    /* Add events to the eventset */
    if (PAPI_add_events(EventSet,Events,NUM_EVENTS) != PAPI_OK) {
        printf("Failed  Add events to the eventset \n");
        exit(0);
    }
    
    
    
    //iniciar papi
    PAPI_start(EventSet);
    
    // Iniciar contador de tempo
    double start = PAPI_get_real_usec();
    
    // calcular produto das matrizes
    multMatriz(matrizA,matrizB,matrizR,N);
    //finalizar contador de tempo
    double end = PAPI_get_real_usec();
    
    //Finalizar Papi
    PAPI_stop(EventSet,values);
    
    
    //imprimir resultados
    long long int bytes = sizeof(float) * N * N;
    float segundos = (end-start)/1000000;
    long long int flops = ((long long int)N * (long long int)N * (long long int)N) * 2;
    float gflops = flops/segundos;
    gflops = gflops/(1000000000);
    float ramAccesses = (float) values[2]/values[3];
    
    
    
    printf("\n");
    printf("Bytes:\t%lld\nReal_time:\t%f\nGFLOPS:\t\t%f\nL1 Misses:\t%lld\nL2 Misses:\t%lld\nL3 Misses:\t%lld\nRam Acess:\t%f\nTotal Instructions:\t%lld\n",
           bytes,segundos, gflops,values[0],values[1],values[2],ramAccesses,values[3] );
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
