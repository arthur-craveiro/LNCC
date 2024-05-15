#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

typedef struct matrix {
    struct matrix* right;
    struct matrix* below;
    int line;
    int column;
    float info;
} Matrix;

Matrix* create_node(int line, int column, float info) {
    Matrix* node = (Matrix*)malloc(sizeof(Matrix));
    if (!node) {
        return NULL;
    }
    node->line = line;
    node->column = column;
    node->info = info;
    node->right = node;
    node->below = node;
    return node;
}

int matrix_create(Matrix **m) {
    int numRows, numCols;
    scanf("%d %d", &numRows, &numCols);
    printf("\n Número de Linhas: %d \n Número de Colunas: %d \n \n", numRows, numCols);

    // Aloca memória para o nó cabeça da matriz.
    *m = create_node(-1, -1, 0.0);
    if (!*m) {
        printf("Erro na leitura do número de linhas e colunas.\n");
        return -1; // Erro na alocação de memória.
    }

    // Cria e conecta os nós cabeça para cada linha.
    Matrix *lastRowHead = *m;
    for (int i = 0; i < numRows; i++) {
        Matrix *rowHead = create_node(i + 1, -1, 0.0);
        if (!rowHead) {
            // Libera a memória alocada para os nós cabeça de linha até agora.
            while (lastRowHead != *m) {
                Matrix *temp = lastRowHead->below;
                free(lastRowHead);
                lastRowHead = temp;
            }
            free(*m);
            printf("Erro na criação do nó cabeça da linha %d.\n", i + 1);
            return -1;
        }
        lastRowHead->below = rowHead;
        lastRowHead = rowHead;
    }
    lastRowHead->below = *m; // Completa a circularidade da lista de linhas.

    // Cria e conecta os nós cabeça para cada coluna.
    Matrix *lastColHead = *m;
    for (int j = 0; j < numCols; j++) {
        Matrix *colHead = create_node(-1, j + 1, 0.0);
        if (!colHead) {
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
            printf("Erro na criação do nó cabeça da coluna %d.\n", j + 1);
            return -1;
        }
        lastColHead->right = colHead;
        lastColHead = colHead;
    }
    lastColHead->right = *m; // Completa a circularidade da lista de colunas.

    // Ler e inserir elementos na matriz.
    int row, col, result;
    result = 1;
    float value; 
    do {
        result = scanf("%d", &row);
        if (result != 1 || row == 0) {
            break;
        }
        result = scanf("%d %f", &col, &value);
        if (result != 2) {
            printf("Erro na leitura dos valores de coluna e valor.\n");
            return -1;
        }
        printf("Lido elemento: linha %d, coluna %d, valor %.2f\n", row, col, value); // Depuração
        if (matrix_setelem(*m, row, col, value) != 0) {
            printf("Erro na inserção do elemento na posição (%d, %d).\n", row, col);
            return -1;
        } 
    } while (result != 0);
    
    return 0; // Sucesso.
}

int matrix_destroy(Matrix *m) {
    if (!m) {
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

    return 0; // Sucesso.
}

int matrix_print(const Matrix* m) {
    if (!m) {
        printf("Matriz vazia\n");
        return -1; // Retorna erro se a matriz for nula.
    }

    Matrix *rowHead = m->below; // Começa pelo primeiro nó cabeça de linha.
    while (rowHead != m) {
        Matrix *node = rowHead->right;
        while (node != rowHead) {
            if (node->line != -1 && node->column != -1) { // Ignora nós cabeça
                printf("%d %d %f\n", node->line, node->column, node->info);
            }
            node = node->right;
        }
        rowHead = rowHead->below;
    }
    printf("Fim da matriz\n"); // Indicador do fim da matriz.
    return 0; // Sucesso.
}

int matrix_add(const Matrix* m, const Matrix* n, Matrix** r) {
    if (!m || !n) {
        return -1; // Erro se qualquer matriz for nula.
    }

    // Cria a matriz resultante
    *r = create_node(-1, -1, 0.0);
    if (!*r) {
        return -1; // Erro na alocação de memória.
    }

    // Itera pelas linhas das matrizes m e n
    Matrix *rowM = m->below;
    Matrix *rowN = n->below;
    while (rowM != m && rowN != n) {
        if (rowM->line < rowN->line) {
            Matrix *nodeM = rowM->right;
            while (nodeM != rowM) {
                matrix_setelem(*r, nodeM->line, nodeM->column, nodeM->info);
                nodeM = nodeM->right;
            }
            rowM = rowM->below;
        } else if (rowN->line < rowM->line) {
            Matrix *nodeN = rowN->right;
            while (nodeN != rowN) {
                matrix_setelem(*r, nodeN->line, nodeN->column, nodeN->info);
                nodeN = nodeN->right;
            }
            rowN = rowN->below;
        } else {
            Matrix *nodeM = rowM->right;
            Matrix *nodeN = rowN->right;
            while (nodeM != rowM && nodeN != rowN) {
                if (nodeM->column < nodeN->column) {
                    matrix_setelem(*r, nodeM->line, nodeM->column, nodeM->info);
                    nodeM = nodeM->right;
                } else if (nodeN->column < nodeM->column) {
                    matrix_setelem(*r, nodeN->line, nodeN->column, nodeN->info);
                    nodeN = nodeN->right;
                } else {
                    float sum = nodeM->info + nodeN->info;
                    if (sum != 0.0) {
                        matrix_setelem(*r, nodeM->line, nodeM->column, sum);
                    }
                    nodeM = nodeM->right;
                    nodeN = nodeN->right;
                }
            }
            while (nodeM != rowM) {
                matrix_setelem(*r, nodeM->line, nodeM->column, nodeM->info);
                nodeM = nodeM->right;
            }
            while (nodeN != rowN) {
                matrix_setelem(*r, nodeN->line, nodeN->column, nodeN->info);
                nodeN = nodeN->right;
            }
            rowM = rowM->below;
            rowN = rowN->below;
        }
    }

    // Processa as linhas restantes de m, se houver
    while (rowM != m) {
        Matrix *nodeM = rowM->right;
        while (nodeM != rowM) {
            matrix_setelem(*r, nodeM->line, nodeM->column, nodeM->info);
            nodeM = nodeM->right;
        }
        rowM = rowM->below;
    }

    // Processa as linhas restantes de n, se houver
    while (rowN != n) {
        Matrix *nodeN = rowN->right;
        while (nodeN != rowN) {
            matrix_setelem(*r, nodeN->line, nodeN->column, nodeN->info);
            nodeN = nodeN->right;
        }
        rowN = rowN->below;
    }

    return 0; // Sucesso.
}

int matrix_multiply(const Matrix* m, const Matrix* n, Matrix** r) {
    if (!m || !n) {
        return -1; // Erro se qualquer matriz for nula.
    }

    // Cria a matriz resultante
    *r = create_node(-1, -1, 0.0);
    if (!*r) {
        return -1; // Erro na alocação de memória.
    }

    // Multiplica as matrizes m e n
    Matrix *rowM = m->below;
    while (rowM != m) {
        Matrix *colN = n->right;
        while (colN != n) {
            float sum = 0.0;
            Matrix *nodeM = rowM->right;
            Matrix *nodeN = colN->below;
            while (nodeM != rowM && nodeN != colN) {
                if (nodeM->column < nodeN->line) {
                    nodeM = nodeM->right;
                } else if (nodeN->line < nodeM->column) {
                    nodeN = nodeN->below;
                } else {
                    sum += nodeM->info * nodeN->info;
                    nodeM = nodeM->right;
                    nodeN = nodeN->below;
                }
            }
            if (sum != 0.0) {
                matrix_setelem(*r, rowM->line, colN->column, sum);
            }
            colN = colN->right;
        }
        rowM = rowM->below;
    }

    return 0; // Sucesso.
}

int matrix_transpose(const Matrix* m, Matrix** r) {
    if (!m) {
        return -1; // Erro se a matriz de entrada for nula.
    }

    // Cria a matriz resultante.
    *r = create_node(-1, -1, 0.0);
    if (!*r) {
        return -1; // Erro na alocação de memória.
    }

    // Itera pelos elementos da matriz m e insere na matriz transposta.
    Matrix *rowHead = m->below; // Começa pelo primeiro nó cabeça de linha.
    while (rowHead != m) {
        Matrix *node = rowHead->right;
        while (node != rowHead) {
            // Define o elemento na posição transposta.
            matrix_setelem(*r, node->column, node->line, node->info);
            node = node->right;
        }
        rowHead = rowHead->below;
    }

    return 0; // Sucesso.
}

// Função para obter o elemento na posição (x, y) da matriz.
int matrix_getelem(const Matrix *m, int x, int y, float *elem) {
    Matrix *row = m->below;
    while (row != m && row->line < x) {
        row = row->below;
    }
    
    if (row->line != x) {
        *elem = 0.0;
        return 0;
    }
    
    Matrix *colNode = row->right;
    while (colNode != row && colNode->column < y) {
        colNode = colNode->right;
    }
    
    if (colNode->column != y) {
        *elem = 0.0;
        return 0;
    }
    
    *elem = colNode->info;
    return 1;
}

// Função para definir o elemento na posição (x, y) da matriz.
int matrix_setelem(Matrix *m, int x, int y, float elem) {
    printf("Inserindo elemento: linha %d, coluna %d, valor %.2f\n", x, y, elem); // Depuração
    Matrix *row = m->below;
    while (row != m && row->line < x) {
        row = row->below;
    }
    
    if (row == m || row->line != x) {
        printf("Erro: linha %d não encontrada\n", x); // Depuração
        return -1;
    }
    
    Matrix *prev = row;
    Matrix *current = row->right;
    while (current != row && current->column < y) {
        prev = current;
        current = current->right;
    }
    
    if (current != row && current->column == y) {
        if (elem == 0.0) {
            prev->right = current->right;
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
            current->info = elem;
        }
        return 0;
    }
    
    if (elem != 0.0) {
        Matrix *newNode = create_node(x, y, elem);
        if (!newNode) {
            printf("Erro na alocação de memória para o novo nó\n"); // Depuração
            return -1;
        }
        
        newNode->right = prev->right;
        prev->right = newNode;
        
        prev = m;
        Matrix *colHead = m->right;
        while (colHead != m && colHead->column < y) {
            prev = colHead;
            colHead = colHead->below;
        }
        
        while (prev->below != colHead && prev->below->line < x) {
            prev = prev->below;
        }
        
        newNode->below = prev->below;
        prev->below = newNode;

        return 0;
    }
    
    return 0;
}
