#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <cpuid.h>

struct matrix {
    unsigned long int height;/* linhas  */
    unsigned long int width; /* colunas */
    float *rows;
};

void scalar_matrix_mult(float scalar_value, struct matrix *matrix){
    
    __m256 result, escalar;
    int a = 0, i = 0, k = 0;
    //float teste[8] ;
    float *multiplicationArray = (float *)aligned_alloc(32, matrix->width*sizeof(float));

    escalar = _mm256_set1_ps ((float)scalar_value);

    printf("impressão da matriz original:\n");
    for (a=0; a<(matrix->height*matrix->width); a++){
        if(a%8==0){
            printf("\n");
        }
        printf("%.1f ", matrix->rows[a]);
    }   printf("\nFim da impressao da matriz original.\n\n");   /* OK */

/* Impressao do vetor escalar 
    printf("impressão do vetor escalar: ");
    for (int a=0; a<8; a++){
        printf("%.1f ", escalar[a]);
    }            
    printf("\nFim da impressao do vetor escalar.\n\n"); VETOR OK*/

    for (i = 0; i < matrix->height; i++) {
        __m256 linha;
        int k = i*8;
        linha = _mm256_setr_ps( matrix->rows[k], matrix->rows[k+1], matrix->rows[k+2], 
                                matrix->rows[k+3], matrix->rows[k+4], matrix->rows[k+5], 
                                matrix->rows[k+6], matrix->rows[k+7] );
/* *********************    OK para qualquer linha    ************   */
        printf("Impressao da linha %d: ", i);
        for (a=0; a<8; a++){
            printf("%.1f ", linha[a]);
        }
        printf("\n");
 /********************************************************************** */        
        result =  _mm256_mul_ps (escalar, linha);   /* OK */     
/**************************   OK*   *******************************/          
        printf("Impressao do vetor result: ");
        for (a=0; a<8; a++){
            printf("%.1f ", result[a]);
        }
        printf("\n"); 
/****************************************************************** */
        _mm256_store_ps( (multiplicationArray + k), result);
       
        printf("Valor de k: %d\nImpressao do vetor teste: ", k);   
        for (a = 0+k; a<8+k; a++){
            printf("%.1f ", multiplicationArray[a]);
        } 
        printf("\n");
    
    }   /* FIM FOR */
    
    /* 
    printf("impressão da matriz final:\n");
    for (a=0; a<(matrix->height*matrix->width); a++){
        if(a%8==0){
            printf("\n");
        }
        printf("%.1f ", matrix->rows[a]);
    }
    printf("\n fim da impressao.\n\n");*/
}



void main(int argc, char *argv[]) {
    float *rowsA = malloc(sizeof(float)*2*8);
    int i = 0;
    //float *rowsB = malloc(sizeof(float)*2*8);
    
    for (i = 0; i < 2*8; i++) {
        rowsA[i] = i+1;
        //rowsB[i] = 1;
    }
    struct matrix matrixA;
    //struct matrix matrixB;
    //struct matrix matrixC;

    matrixA.height = 2;
    matrixA.width = 8;
    matrixA.rows = rowsA;

    //matrixB.height = 2;
    //matrixB.width = 8;
    //matrixB.rows = rowsB;  
    
    scalar_matrix_mult(10.0f , &matrixA);
    
    /* matrix_matrix_mult(&matrixA, &matrixB, &matrixC);
    
    printf("%f", matrixC.rows[0]); */

    
}