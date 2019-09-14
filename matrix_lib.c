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
    int numberOfElementsInMatrixA = matrixA->width * matrixA->height;
    int numberOfElementsInMatrixB = matrixB->width * matrixB->height;

    if (matrixA->width != matrixB->height) {
        return 0;
    }

    float *a = (float*)aligned_alloc(32, matrixA->width*sizeof(float));
    float *b = (float*)aligned_alloc(32, matrixA->width*sizeof(float));
    float *result = (float*)aligned_alloc(32, matrixA->width*sizeof(float));

    if ((a == NULL) || (b == NULL) || (result == NULL)) {
        printf("vector allocation problem.");
        return 0;
    }

    // Calcula o primeiro elemento da matrixC
    // Carrega a primeira linha de A e coluna de B 
    float *tmpMatrixBColumn = malloc(8*sizeof(float));
    for(int i = 0, j = 0; i < numberOfElementsInMatrixB; i+=matrixB->width, j++) {
        tmpMatrixBColumn[j] = matrixB->rows[i];
    }

    float *multiplicationArray = malloc(matrixA->width*sizeof(float));

    // De 8 em 8 vamos adicionando coisas no array de multiplicação
    for (int i = 0; i < matrixA->width; i+=8) {
        __m256 matrixALine = _mm256_load_ps(matrixA->rows + i);
        __m256 matrixBColumn = _mm256_load_ps(tmpMatrixBColumn + i);

        _mm256_store_ps(multiplicationArray + i, _mm256_mul_ps(matrixALine,matrixBColumn));
    }

    // Soma todos os elementos do array de multiplicação
    float sum = 0;
    for (int i = 0; i < matrixA->width; i++) {
        sum += multiplicationArray[i];
    }
    matrixC->rows[0] = sum;

    return 1;
}

int main(int argc, char *argv[]) {
    float *rowsA = malloc(sizeof(float)*8*8);
    float *rowsB = malloc(sizeof(float)*8*8);
    for (int i = 0; i < 8*8; i++) {
        rowsA[i] = i+1;
        rowsB[i] = 1;
    }
    struct matrix matrixA;
    struct matrix matrixB;
    struct matrix matrixC;

    matrixA.height = 8;
    matrixA.width = 8;
    matrixA.rows = rowsA;

    matrixB.height = 8;
    matrixB.width = 8;
    matrixB.rows = rowsB;  
    
    matrix_matrix_mult(&matrixA, &matrixB, &matrixC);

    printf("%f", matrixC.rows[0]);

    return 0;
 }