#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <cpuid.h>

/* 
height  = número de linhas da matriz (múltiplo de 8)
width   = número de colunas da matriz (múltiplo de 8)
rows= sequência de linhas da matriz (height*width elementos) */
struct matrix {
    unsigned long int height;/* linhas  */
    unsigned long int width; /* colunas */
    float *rows;
};

/* Essa função recebe um valor escalar e uma matriz como argumentos 
de entrada e calcula o produto do valor escalar pela matriz. O 
resultado da operação deve ser retornado na matriz de entrada. Em 
caso de sucesso, a função deve retornar o valor 1. Em caso de erro,
a função deve retornar 0. */

void scalar_matrix_mult(float scalar_value, struct matrix *matrix){
    
    __m256 result, escalar;
    int k = 0;
    float teste[8] ;
    
    escalar = _mm256_set1_ps ((float)scalar_value);

    printf("impressão da matriz original:\n");
    for (int a=0; a<(matrix->height*matrix->width); a++){
        if(a%8==0){
            printf("\n");
        }
        printf("%.1f ", matrix->rows[a]);
    }
    printf("\nFim da impressao da matriz original.\n\n");   /* OK */

/* Impressao do vetor escalar 
    printf("impressão do vetor escalar: ");
    for (int a=0; a<8; a++){
        printf("%.1f ", escalar[a]);
    }            
    printf("\nFim da impressao do vetor escalar.\n\n"); VETOR OK*/

    for (int i = 0; i < matrix->height; i++) {
        
        for(int j = 0; j < matrix->width; j++){
            __m256 linha;
            linha = _mm256_setr_ps( matrix->rows[k], matrix->rows[k+1], matrix->rows[k+2], 
                                    matrix->rows[k+3], matrix->rows[k+4], matrix->rows[k+5], 
                                    matrix->rows[k+6], matrix->rows[k+7]);
/* **********************************************************************   *                                  
            printf("impressão da linha %d da matriz: ", i);
            for (int a=0; a<8; a++){
                printf("%.1f ", linha[a]);
            }
            printf("\n");
 ********************************************************************** */           
            result =  _mm256_mul_ps (escalar, linha);   /* OK */
         
            //matrix->rows[ (i * matrix->width) + j ] = _mm256_cvtss_f32(result[j]);
            *teste = _mm256_cvtss_f32(result);
            /* Impressao do teste 
            for (int a = 0; a<8; a++){
                printf("teste = %.1f", teste[a]);
            }*/
        }
    }
     printf("impressão da matriz final:\n");
    for (int a=0; a<(matrix->height*matrix->width); a++){
        if(a%8==0){
            printf("\n");
        }
        printf("%.2f ", matrix->rows[a]);
    }
    printf("\n fim da impressao.\n\n");
    
}



void main(int argc, char *argv[]) {
    float *rowsA = malloc(sizeof(float)*2*8);
    float *rowsB = malloc(sizeof(float)*2*8);
    
    for (int i = 0; i < 2*8; i++) {
        rowsA[i] = i+1;
        rowsB[i] = 1;
    }
    struct matrix matrixA;
    struct matrix matrixB;
    struct matrix matrixC;

    matrixA.height = 2;
    matrixA.width = 8;
    matrixA.rows = rowsA;

    matrixB.height = 2;
    matrixB.width = 8;
    matrixB.rows = rowsB;  
    
    scalar_matrix_mult(10.0f , &matrixA);
    printf('Foi.');
    
    /* matrix_matrix_mult(&matrixA, &matrixB, &matrixC);
    
    printf("%f", matrixC.rows[0]); */

    
 }

 /*            float tmp;

            _mm256_store_ps(&tmp, );
            __m256 _mm256_load_ps (float const * mem_addr);
            *result = _mm256_mul_ps();
                        //matrix->rows[ (i * matrix->width) + j ] = (float)result[j];
            //for (int t = 0; t < ((t+1)*8); t++)
            //aux[j] = matrix->rows[j];
*/