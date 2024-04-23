#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

struct matrix {
    struct matrix* right;
    struct matrix* below;
    int line;
    int column;
    float info;
}

/* Explicações:
right = aponta p/ próx. elem. dif. de 0 na mesma linha;
below = aponta p/ próx. elem. dif. de 0 na mesma coluna;
Dado uma matrix A(i,j) = x temos:
    line = i;
    column = j;
    info = x; */

int matrix_create(Matrix **m) {
    int numRows, numCols;
    scanf("%d %d", &numRows, &numCols);

    // Aloca memória para a matriz e os nós cabeça das listas de linhas e colunas.
    *m = (Matrix *)malloc(sizeof(Matrix));
    if (*m == NULL) {
        return -1; // Retorna erro se a alocação de memória falhar.
    }

    // Inicializar o nó cabeça da matriz.
    (*m)->right = (*m)->below = *m;
    (*m)->line = -1;
    (*m)->column = -1;
    (*m)->info = -1.0;

    // Cria e conecta os nós cabeça para cada linha.
    Matrix *rowHeads = NULL;
    Matrix *lastRowHead = *m;
    for (int i = 0; i < numRows; i++) {
        Matrix *rowHead = (Matrix *)malloc(sizeof(Matrix));
        if (rowHead == NULL) {
            // Libera a memória alocada para os nós cabeça de linha até agora.
            while (lastRowHead != *m) {
                Matrix *temp = lastRowHead->below;
                free(lastRowHead);
                lastRowHead = temp;
            }
            free(*m);
            return -1;
        }
        rowHead->right = rowHead; // Lista circular.
        rowHead->below = NULL;
        rowHead->line = i + 1; // Indexação começa em 1.
        rowHead->column = -1; // Indica nó cabeça de linha.
        rowHead->info = -1.0;
        lastRowHead->below = rowHead;
        if (i == 0) rowHeads = rowHead; // Guarda o primeiro nó cabeça de linha.
        lastRowHead = rowHead;
    }
    lastRowHead->below = rowHeads; // Completa a circularidade da lista de linhas.

    // Cria e conecta os nós cabeça para cada coluna.
    Matrix *colHeads = NULL;
    Matrix *lastColHead = *m;
    for (int j = 0; j < numCols; j++) {
        Matrix *colHead = (Matrix *)malloc(sizeof(Matrix));
        if (colHead == NULL) {
            // Libera a memória alocada para os nós cabeça de coluna até agora.
            while (lastColHead != *m) {
                Matrix *temp = lastColHead->right;
                free(lastColHead);
                lastColHead = temp;
            }
            // Libera a memória alocada para os nós cabeça de linha.
            while (lastRowHead != *m) {
                Matrix *temp = lastRowHead->below;
                free(lastRowHead);
                lastRowHead = temp;
            }
            free(*m);
            return -1;
        }
        colHead->right = NULL;
        colHead->below = colHead; // Lista circular.
        colHead->line = -1; // Indica nó cabeça de coluna.
        colHead->column = j + 1; // Indexação começa em 1.
        colHead->info = -1.0;
        lastColHead->right = colHead;
        if (j == 0) colHeads = colHead; // Guarda o primeiro nó cabeça de coluna.
        lastColHead = colHead;
    }
    lastColHead->right = colHeads; // Completa a circularidade da lista de colunas.

    // Ler e inserir elementos na matriz.
    int row, col;
    float value;
    while (scanf("%d %d %f", &row, &col, &value) == 3 && (row != 0 || col != 0)) {
        matrix_setelem(*m, row, col, value);
    }

    return 0; // Sucesso
}

int matrix_destroy(Matrix *m) {
    if (m == NULL) {
        return -1; // Retorna erro se a matriz for nula.
    }

    // Liberar os nós da matriz que não são nós cabeça de linha ou coluna.
    Matrix *row = m->below; // Começa pelo primeiro nó cabeça de linha.
    while (row != m) { // Enquanto não volta ao nó cabeça da matriz.
        Matrix *node = row->right;
        while (node != row) { // Enquanto não volta ao nó cabeça de linha.
            Matrix *temp = node->right;
            free(node);
            node = temp;
        }
        Matrix *tempRow = row->below;
        free(row); // Libera o nó cabeça de linha.
        row = tempRow;
    }

    // Liberar os nós cabeça das colunas.
    Matrix *col = m->right; // Começa pelo primeiro nó cabeça de coluna.
    while (col != m) { // Enquanto não volta ao nó cabeça da matriz.
        Matrix *tempCol = col->right;
        free(col);
        col = tempCol;
    }

    // Por fim, liberar o nó cabeça da matriz.
    free(m);

    return 0; // Sucesso
}

// Função para obter o elemento na posição (x, y) da matriz.
int matrix_getelem(const Matrix *m, int x, int y, float *elem) {
    // Supondo que 'm' é o nó cabeça da matriz e que a lista de cabeçalhos de linhas
    // está em 'm->right' e a lista de cabeçalhos de colunas está em 'm->below'.

    // Navegue até a linha x.
    Matrix *row = m->right;
    while (row != m && row->line < x) {
        row = row->right;
    }
    
    // Se chegamos ao fim ou a linha não é x, o elemento é zero.
    if (row->line != x) {
        *elem = 0.0;
        return 0;
    }
    
    // Navegue até a coluna y.
    Matrix *colNode = row->below;
    while (colNode != row && colNode->column < y) {
        colNode = colNode->below;
    }
    
    // Se chegamos ao fim ou a coluna não é y, o elemento é zero.
    if (colNode->column != y) {
        *elem = 0.0;
        return 0;
    }
    
    // Se encontramos o elemento, retornamos seu valor.
    *elem = colNode->info;
    return 1;
}

// Função para definir o elemento na posição (x, y) da matriz.
int matrix_setelem(Matrix *m, int x, int y, float elem) {
    // Encontre a linha x.
    Matrix *row = m->below;
    while (row != m && row->line < x) {
        row = row->below;
    }
    
    // Se não encontramos a linha correta, erro.
    if (row == m || row->line != x) {
        return -1;
    }
    
    // Encontre a coluna y na linha x.
    Matrix *prev = row; // Para manter o nó anterior na linha.
    Matrix *current = row->right;
    while (current != row && current->column < y) {
        prev = current;
        current = current->right;
    }
    
    // Se o elemento já existe na coluna, atualize o valor.
    if (current != row && current->column == y) {
        if (elem == 0.0) {
            // Remover o elemento se o novo valor é zero.
            prev->right = current->right; // Desvincula o nó da linha.
            // Agora, desvincular da coluna.
            Matrix *colPrev = m;
            Matrix *colCurrent = m->right;
            while (colCurrent->column != y) {
                colPrev = colCurrent;
                colCurrent = colCurrent->below;
            }
            while (colCurrent != current) {
                colPrev = colCurrent;
                colCurrent = colCurrent->below;
            }
            colPrev->below = current->below;
            free(current);
        } else {
            // Atualize o valor do elemento.
            current->info = elem;
        }
        return 0;
    }
    
    // Se estamos inserindo um novo elemento diferente de zero, criar e inserir o nó.
    if (elem != 0.0) {
        Matrix *newNode = (Matrix *)malloc(sizeof(Matrix));
        if (newNode == NULL) {
            return -1; // Falha na alocação de memória.
        }
        newNode->line = x;
        newNode->column = y;
        newNode->info = elem;
        
        // Inserir na linha.
        newNode->right = prev->right;
        prev->right = newNode;
        
        // Encontrar a posição correta na coluna e inserir.
        prev = m; // Começa no nó cabeça da matriz para colunas.
        Matrix *colHead = m->right;
        while (colHead != m && colHead->column < y) {
            // Mova-se para o nó de cabeça da coluna correspondente.
            prev = colHead;
            colHead = colHead->below;
        }
        
        while (prev->below != colHead && prev->below->line < x) {
            // Mova-se para o ponto de inserção correto na coluna.
            prev = prev->below;
        }

        // Inserir o novo nó na coluna.
        newNode->below = prev->below;
        prev->below = newNode;

        return 0;
    }
    
    // Se o novo valor é zero e não havia elemento, não precisamos fazer nada.
    return 0;
}