#include <stdio.h>
#include <stdlib.h>
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

    for (int i = 0; i < matrixA->height; i++) {
        float *multipliedMatrix = multiplyLineElementsInFirstMatrixWithRowOfSecondMatrix(matrixA, matrixB, i);
        matrixC->rows[0] = sumAllElementsInVector(multipliedMatrix, matrixA->width);
    }
    return 1;
}

/*
* Esta Função funciona somente para matrizes de tamanho 8 devido a função '_mm256_mul_ps'
*/
float *multiplyLineElementsInFirstMatrixWithRowOfSecondMatrix(struct matrix *firstMatrix, struct matrix *secondMatrix, int lineNumber) {
    float * currentLine = malloc(sizeof(float) * firstMatrix->width);
    for (int j = 0; j < firstMatrix->width; j++) {
        currentLine[j] = firstMatrix->rows[lineNumber];
    }
    return _mm256_mul_ps(currentLine, secondMatrix->rows);
} 

float sumAllElementsInVector(float *vector, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum = sum + vector[i];
    }
    return sum;
}

int main(int argc, char *argv[]) {

    



    return 0;
 }