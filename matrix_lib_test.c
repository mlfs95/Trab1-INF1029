#include <stdio.h>
#include <stdlib.h>
#include "matrix_lib.h"

int main_func(int argc, char *argv[]) {
    if (argc < 10) {
        printf("Menos que 9 parâmetros passados");
        return 0;
    }

    float scalarValue = atof(argv[1]);
    int firstMatrixHeight = atoi(argv[2]);
    int firstMatrixWidth = atoi(argv[3]);
    int secondMatrixHeight = atoi(argv[4]);
    int secondMatrixWidth = atoi(argv[5]);
    char *firstFileName = argv[6];
    char *secondFileName = argv[7];
    char *thirdFileName = argv[8];
    char *fourthFileName = argv[9];

    FILE *firstMatrixFile = fopen(firstFileName, "r");
    FILE *secondMatrixFile = fopen(secondFileName, "r");
    FILE *firstResultFile = fopen(thirdFileName, "w");
    FILE *secondResultFile = fopen(fourthFileName, "w");
    
    Matrix firstMatrix;
    int error = createMatrix(&firstMatrix, firstMatrixHeight, firstMatrixWidth, firstMatrixFile);
    Matrix secondMatrix;
    error = createMatrix(&secondMatrix, secondMatrixHeight, secondMatrixWidth, secondMatrixFile);
    
    if (error==0) {
        printf("\nERRO!! Dimensões da matriz e arquivo não correspondem.\n");
        return;
    }

    fclose(firstMatrixFile);
    fclose(secondMatrixFile);

    scalar_matrix_mult(scalarValue, &firstMatrix);
    writeResultOnFile(&firstMatrix, &firstResultFile);
    
    Matrix secondTaskResultMatrix;
    matrix_matrix_mult(&firstMatrix, &secondMatrix, &secondTaskResultMatrix);
    writeResultOnFile(&secondTaskResultMatrix, &secondResultFile);
    
    return 1;
}

int createMatrix(Matrix *matrix, int height, int width, FILE *file) {
    int numberOfElements = height*width;
    float number;
    matrix->height = height;
    matrix->width = width;
    matrix->rows = (float *)malloc(numberOfElements*sizeof(float));
    for (int i = 0; fscanf(file, "%f", number) == 1; i++) {
        if (i == numberOfElements) {
            return 0;
        }
        matrix->rows[i] = number;
    }
    return 1;
}

int writeResultOnFile(Matrix *matrix, FILE *file) {
    for (int i = 0; i < matrix->height; i++) {
        for (int j = 0; j < matrix->width; j++) {
            fprintf(file, "%f", matrix->rows[(i*secondTaskResultMatrix.width)+j]);
        }
    }
    fclose(file);
}