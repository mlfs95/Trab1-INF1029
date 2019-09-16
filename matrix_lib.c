#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
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
int scalar_matrix_mult(float scalar_value, struct matrix *matrix){
    int total = (matrix->height*matrix->width);
    for (int i = 0; i < total; i++) {
            matrix->rows[i] = matrix->rows[i]*scalar_value;
    }
}

/* Essa função recebe 3 matrizes como argumentos de entrada e calcula 
o valor do produto da matriz A pela matriz B. O resultado da operação 
deve ser retornado na matriz C. Em caso de sucesso, a função deve 
retornar o valor 1. Em caso de erro, a função deve retornar 0. */
int matrix_matrix_mult(struct matrix *matrixA, struct matrix * matrixB, struct matrix * matrixC){

    if (matrixA->width != matrixB->height) {
        return 0;
    }

    for(int matrixCColunm = 0; matrixCColunm < matrixB->width; matrixCColunm++) {
        for(int matrixCRow = 0; matrixCRow < matrixA->height; matrixCRow++) {
            
            float *alignedRowMatrixA = (float *)aligned_alloc(32, matrixA->width*sizeof(float));
            float *alignedColunmMatrixB = (float *)aligned_alloc(32, matrixA->width*sizeof(float));
            float *multiplicationArray = (float *)aligned_alloc(32, matrixA->width*sizeof(float));

            if ((alignedRowMatrixA == NULL) || (alignedColunmMatrixB == NULL) || (multiplicationArray == NULL)) {
                printf("vector allocation problem\n");
                return 0;
            }

            for(int i = 0, j = 0; j < (matrixB->width * matrixB->height); i++, j+=matrixB->width) {
                alignedRowMatrixA[i] = matrixA->rows[(matrixCRow*matrixA->width)+i];
                alignedColunmMatrixB[i] = matrixB->rows[matrixCColunm+j];
            }
            
            // De 8 em 8 vamos adicionando coisas no array de multiplicação
            for (int i = 0; i < matrixA->width; i+=8) {
                __m256 matrixALine = _mm256_load_ps(alignedRowMatrixA + i);
                __m256 matrixBColumn = _mm256_load_ps(alignedColunmMatrixB + i);
                
                _mm256_store_ps(multiplicationArray + i, _mm256_mul_ps(matrixALine,matrixBColumn));
            }

            // Soma todos os elementos do array de multiplicação
            float sum = 0;
            for (int i = 0; i < matrixA->width; i++) {
                sum += multiplicationArray[i];
            }
            matrixC->rows[(matrixCRow*matrixA->height)+matrixCColunm] = sum;
        }
    }

    return 1;
}

int main(int argc, char *argv[]) {
    float *rowsA = (float *)malloc(sizeof(float)*8*16);
    float *rowsB = (float *)malloc(sizeof(float)*8*16);
    for (int i = 0; i < 8*16; i++) {
        rowsA[i] = i+1;
        rowsB[i] = i+1;
    }
    struct matrix matrixA;
    struct matrix matrixB;
    struct matrix matrixC;

    matrixA.height = 8;
    matrixA.width = 16;
    matrixA.rows = rowsA;

    matrixB.height = 16;
    matrixB.width = 8;
    matrixB.rows = rowsB;  
    
    matrix_matrix_mult(&matrixA, &matrixB, &matrixC);

    printf("matrixC - row[0] = %f", matrixC.rows[0]);

    return 0;
 }