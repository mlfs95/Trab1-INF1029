int scalar_matrix_mult(float scalar_value, Matrix *matrix);

int matrix_matrix_mult(Matrix *matrixA, Matrix *matrixB, Matrix *matrixC);

/* 
* height  = número de linhas da matriz (múltiplo de 8)
* width   = número de colunas da matriz (múltiplo de 8)
* rows= sequência de linhas da matriz (height*width elementos) 
*/
struct _Matrix {
    unsigned long int height;/* linhas  */
    unsigned long int width; /* colunas */
    float *rows;
};
typedef struct _Matrix Matrix