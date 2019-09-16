#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include "matrix_lib.h"

/* Essa função recebe um valor escalar e uma matriz como argumentos 
de entrada e calcula o produto do valor escalar pela matriz. O 
resultado da operação deve ser retornado na matriz de entrada. Em 
caso de sucesso, a função deve retornar o valor 1. Em caso de erro,
a função deve retornar 0. */
int scalar_matrix_mult(float scalar_value, Matrix *matrix){
    int total = (matrix->height*matrix->width);
    for (int i = 0; i < total; i++) {
            matrix->rows[i] = matrix->rows[i]*scalar_value;
    }
}

/* Essa função recebe 3 matrizes como argumentos de entrada e calcula 
o valor do produto da matriz A pela matriz B. O resultado da operação 
deve ser retornado na matriz C. Em caso de sucesso, a função deve 
retornar o valor 1. Em caso de erro, a função deve retornar 0. */
int matrix_matrix_mult(Matrix *matrixA, Matrix *matrixB, Matrix *matrixC) {
    if (matrixA->width != matrixB->height) {
        printf("Matrizes incompatíveis!");
        return 0;
    }

    float *multiplicationArray = (float *)aligned_alloc(32, matrixA->width*sizeof(float));
    if (multiplicationArray == NULL) {
        printf("vector allocation problem\n");
        return 0;
    }

    for(int matrixCColumn = 0; matrixCColumn < matrixB->width; matrixCColumn++) {
        for(int matrixCRow = 0; matrixCRow < matrixA->height; matrixCRow++) {
            
            // De 8 em 8 vamos adicionando coisas no array de multiplicação
            for (int i = 0; i < matrixA->width; i+=8) {
                __m256 matrixALine = _mm256_set_ps(matrixA->rows[matrixCRow*matrixA->width], matrixA->rows[(matrixCRow*matrixA->width)+1],
                                                     matrixA->rows[(matrixCRow*matrixA->width)+2], matrixA->rows[(matrixCRow*matrixA->width)+3],
                                                     matrixA->rows[(matrixCRow*matrixA->width)+4], matrixA->rows[(matrixCRow*matrixA->width)+5],
                                                     matrixA->rows[(matrixCRow*matrixA->width)+6], matrixA->rows[(matrixCRow*matrixA->width)+7]);
                __m256 matrixBColumn = _mm256_set_ps(matrixB->rows[(i*matrixB->width)+matrixCColumn], matrixB->rows[((i+1)*matrixB->width)+matrixCColumn],
                                                        matrixB->rows[((i+2)*matrixB->width)+matrixCColumn], matrixB->rows[((i+3)*matrixB->width)+matrixCColumn],
                                                        matrixB->rows[((i+4)*matrixB->width)+matrixCColumn], matrixB->rows[((i+5)*matrixB->width)+matrixCColumn],
                                                        matrixB->rows[((i+6)*matrixB->width)+matrixCColumn], matrixB->rows[((i+7)*matrixB->width)+matrixCColumn]);
                _mm256_store_ps(multiplicationArray + i, _mm256_mul_ps(matrixALine, matrixBColumn));
            }

            // Soma todos os elementos do array de multiplicação e salva em sua posição correta
            float sum = 0;
            for (int i = 0; i < matrixA->width; i++) {
                sum += multiplicationArray[i];
            }
            matrixC->rows[(matrixCRow*matrixA->height)+matrixCColumn] = sum;
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
    Matrix matrixA;
    Matrix matrixB;
    Matrix matrixC;

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