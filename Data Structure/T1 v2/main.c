#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main(void) {
    //Inicializacao da aplicacao ...

    /* Declaração de ponteiros para matrizes */
    Matrix *A = NULL;
    Matrix *B = NULL;
    Matrix *C = NULL;

    /* Criação e impressão da matriz A */
    if (!matrix_create(&A)) {
        matrix_print(A);
    } else {
        fprintf(stderr, "Erro na alocação de A como listas encadeadas.\n");
        return 1;
    }

    /* Criação e impressão da matriz B */
    if (!matrix_create(&B)) {
        matrix_print(B);
    } else {
        fprintf(stderr, "Erro na alocação de B como listas encadeadas.\n");
        return 1;
    }

    /* Adição das matrizes A e B, e impressão do resultado em C */
    if (!matrix_add(A, B, &C)) {
        matrix_print(C);
    } else {
        fprintf(stderr, "Erro na soma C=A+B.\n");
    }

    /* Liberação da memória de C antes de reusar */
    matrix_destroy(C);

    /* Multiplicação das matrizes A e B, e impressão do resultado em C */
    if (!matrix_multiply(A, B, &C)) {
        matrix_print(C);
    } else {
        fprintf(stderr, "Erro na multiplicação C=A*B.\n");
    }

    /* Liberação da memória de C antes de reusar */
    matrix_destroy(C);

    /* Transposição da matriz A, e impressão do resultado em C */
    if (!matrix_transpose(A, &C)) {
        matrix_print(C);
    } else {
        fprintf(stderr, "Erro na transposição C=A^T.\n");
    }

    /* Liberação de todas as matrizes criadas */
    matrix_destroy(C);
    matrix_destroy(A);
    matrix_destroy(B);

    return 0;
}


