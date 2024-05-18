#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

// STRUCT
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

// Parte 1 (Create & Destroy)
int matrix_create(Matrix **m) {
    int rows, cols;
    scanf("%d %d", &rows, &cols); // Ler o número de linhas e colunas

    // Criar o nó cabeça global
    *m = create_node(-1, -1, 0.0);
    if (!*m) return -1;

    // Criar nós cabeça para cada linha
    Matrix* lastRowHead = *m;
    for (int i = 1; i < rows+1; i++) {
        Matrix* newRowHead = create_node(-1, 0, 0.0);
        if (!newRowHead) return -1;
        lastRowHead->below = newRowHead;
        newRowHead->right = newRowHead; // Circular na linha
        lastRowHead = newRowHead;
    }
    lastRowHead->below = *m; // Conectar o último nó cabeça de linha ao nó cabeça global

    // Criar nós cabeça para cada coluna
    Matrix* lastColHead = *m;
    for (int j = 1; j < cols+1; j++) {
        Matrix* newColHead = create_node(0, -1, 0.0);
        if (!newColHead) {
            printf("Erro na alocação de memória!");
            return -1;
        }
        lastColHead->right = newColHead;
        newColHead->below = newColHead; // Circular na coluna
        lastColHead = newColHead;
    }
    lastColHead->right = *m; // Conectar o último nó cabeça de coluna ao nó cabeça global

    // Leitura e inserção de elementos
    int x, y;
    float elem;
    while (scanf("%d", &x) && x != 0) {
        scanf("%d %f", &y, &elem);
        // Inserir elemento na matriz
        matrix_setelem(*m, x, y, elem);
    }

    return 0; // Sucesso
}

int matrix_destroy(Matrix* m){
    if (!m) {
        printf("Matriz não encontrada!");
        return -1;  // Verifica se a matriz existe
    }

    // Libera todos os nós em cada linha
    Matrix* rowHead = m->below;
    while (rowHead != m) {
        Matrix* current = rowHead->right;
        while (current != rowHead) {
            Matrix* temp = current;
            current = current->right;
            free(temp);
        }
        Matrix* tempRow = rowHead;
        rowHead = rowHead->below;
        free(tempRow);
    }

    // Libera os nós cabeça de coluna
    Matrix* colHead = m->right;
    while (colHead != m) {
        Matrix* tempCol = colHead;
        colHead = colHead->right;
        free(tempCol);
    }

    // Libera o nó cabeça global
    free(m);

    return 0;  // Sucesso
}

// Parte 2 (Operações)
int matrix_print(const Matrix* m) {
    if (!m) {
        printf("Matriz não encontrada!");
        return -1;
    }

    // Primeiro, contar o número de linhas e colunas
    int rowCount = 0, colCount = 0;
    const Matrix* temp = m->below;
    while (temp != m) {
        rowCount++;
        temp = temp->below;
    }
    temp = m->right;
    while (temp != m) {
        colCount++;
        temp = temp->right;
    }

    // Imprimir número de linhas e colunas
    printf("%d %d \n", rowCount, colCount);

    // Agora, percorrer cada linha e cada elemento não-nulo
    const Matrix* rowHead = m->below;
    while (rowHead != m) {
        const Matrix* current = rowHead->right;
        while (current != rowHead) {
            if (current->info != 0) {  // Supondo que zeros não são armazenados
                printf("%d %d %.1f\n", current->line, current->column, current->info);
            }
            current = current->right;
        }
        rowHead = rowHead->below;
    }

    // Imprimir 0 ao final para indicar que acabou
    printf("0 \n");

    return 0;  // Sucesso
}

int matrix_add(const Matrix* m, const Matrix* n, Matrix** r) {
    if (!m || !n) {
        printf("Umas das matrizes não existe");
        return -1;
    }

    int mRowCount = 0, mColCount = 0, nRowCount = 0, nColCount = 0;
    const Matrix* temp = m->below;
    while (temp != m) {
        mRowCount++;
        temp = temp->below;
    }
    temp = m->right;
    while (temp != m) {
        mColCount++;
        temp = temp->right;
    }
    temp = n->below;
    while (temp != n) {
        nRowCount++;
        temp = temp->below;
    }
    temp = n->right;
    while (temp != n) {
        nColCount++;
        temp = temp->right;
    }

    if (mRowCount != nRowCount || mColCount != nColCount) {
        printf("As dimensões das matrizes não coincidem para a adição!");
        return -1; 
    }

    *r = create_node(-1, -1, 0.0);
    if (!*r) {
        printf("Não foi possível criar a matriz resultante!");
        return -1;
    }

    Matrix* lastRowHead = *r;
    for (int i = 0; i < mRowCount; i++) {
        Matrix* newRowHead = create_node(-1, 0, 0.0);
        if (!newRowHead) {
            printf("Erro ao criar os nós cabeça de linha da matriz resultante!");
            return -1;
        }
        lastRowHead->below = newRowHead;
        newRowHead->right = newRowHead;
        lastRowHead = newRowHead;
    }
    lastRowHead->below = *r;  // Circular na linha

    Matrix* lastColHead = *r;
    for (int i = 0; i < mColCount; i++) {
        Matrix* newColHead = create_node(0, -1, 0.0);
        if (!newColHead) {
            printf("Erro ao criar os nós cabeça de coluna da matriz resultante!");
            return -1;
        }
        lastColHead->right = newColHead;
        newColHead->below = newColHead; 
        lastColHead = newColHead;
    }
    lastColHead->right = *r; // Circular na coluna

    // Adicionar elementos de m e n e armazenar em r
    Matrix* rowHeadM = m->below;
    Matrix* rowHeadN = n->below;
    Matrix* rowHeadR = (*r)->below;

    while (rowHeadM != m && rowHeadN != n) {
        Matrix* nodeM = rowHeadM->right;
        Matrix* nodeN = rowHeadN->right;
        Matrix* nodeR = rowHeadR;  // Inicializa nodeR como o nó cabeça da linha em r

        while (nodeM != rowHeadM || nodeN != rowHeadN) {
            float sum = 0.0;
            int targetRow = 0;
            int targetCol = 0;

            if (nodeM != rowHeadM && (nodeN == rowHeadN || nodeM->column < nodeN->column)) {
                // Caso em que só m tem um elemento nessa coluna
                sum = nodeM->info;
                targetRow = nodeM->line;
                targetCol = nodeM->column;
                nodeM = nodeM->right;
            } else if (nodeN != rowHeadN && (nodeM == rowHeadM || nodeN->column < nodeM->column)) {
                // Caso em que só n tem um elemento nessa coluna
                sum = nodeN->info;
                targetRow = nodeN->line;
                targetCol = nodeN->column;
                nodeN = nodeN->right;
            } else if (nodeM != rowHeadM && nodeN != rowHeadN && nodeM->column == nodeN->column) {
                // Caso em que ambos m e n têm elementos na mesma coluna
                sum = nodeM->info + nodeN->info;
                targetRow = nodeM->line;
                targetCol = nodeM->column;
                nodeM = nodeM->right;
                nodeN = nodeN->right;
            }

            if (sum != 0.0) {
                // Criar e adicionar novo nó em r se o resultado da soma não for zero
                Matrix* newNode = create_node(targetRow, targetCol, sum);
                if (!newNode) return -1; // Falha na alocação de memória

                nodeR->right = newNode;  // Conecta novo nó à lista
                newNode->right = rowHeadR;  // Mantém a circularidade
                nodeR = newNode;  // Atualiza nodeR para o último nó inserido
            }
        }

        // Avançar para a próxima linha
        rowHeadM = rowHeadM->below;
        rowHeadN = rowHeadN->below;
        rowHeadR = rowHeadR->below;
    }

    return 0;
}

int matrix_multiply(const Matrix* m, const Matrix* n, Matrix** r){
    if (!m || !n) {
        printf("Umas das matrizes não existe");
        return -1;
    }

    int mRowCount = 0, mColCount = 0, nRowCount = 0, nColCount = 0;
    const Matrix* temp = m->below;
    while (temp != m) {
        mRowCount++;
        temp = temp->below;
    }
    temp = m->right;
    while (temp != m) {
        mColCount++;
        temp = temp->right;
    }
    temp = n->below;
    while (temp != n) {
        nRowCount++;
        temp = temp->below;
    }
    temp = n->right;
    while (temp != n) {
        nColCount++;
        temp = temp->right;
    }

    if (mColCount != nRowCount) {
        printf("As dimensões das matrizes não coincidem para a multiplicação!");
        return -1; 
    }

    *r = create_node(-1, -1, 0.0);
    if (!*r) {
        printf("Não foi possível criar a matriz resultante!");
        return -1;
    }

    // Criar nós cabeça de linha para r
    Matrix* lastRowHead = *r;
    for (int i = 0; i < mRowCount; i++) {
        Matrix* newRowHead = create_node(-1, 0, 0.0);
        if (!newRowHead) {
            printf("Falha ao criar cabeça de linha para a matriz resultante!");
            return -1;
        }
        lastRowHead->below = newRowHead;
        newRowHead->right = newRowHead; // Circular na linha
        lastRowHead = newRowHead;
    }
    lastRowHead->below = *r;  // Fechar a lista circularmente

    // Criar nós cabeça de coluna para r
    Matrix* lastColHead = *r;
    for (int j = 0; j < nColCount; j++) {
        Matrix* newColHead = create_node(0, -1, 0.0);
        if (!newColHead) {
            printf("Falha ao criar cabeça de coluna para a matriz resultante!");
            return -1;
        }
        lastColHead->right = newColHead;
        newColHead->below = newColHead; // Circular na coluna
        lastColHead = newColHead;
    }
    lastColHead->right = *r;  // Fechar a lista circularmente
    
    //AQUI COMEÇA A MULTIPLICAÇÃO MESMO
    for (Matrix* rowM = m->below; rowM != m; rowM = rowM->below) {
        for (Matrix* colN = n->right; colN != n; colN = colN->right) {
            float sum = 0;
            for (Matrix* elemM = rowM->right; elemM != rowM; elemM = elemM->right) {
                Matrix* elemN = colN->below;  // Corrigido para começar da coluna correta em n
                while (elemN != colN && elemN->line < elemM->column) {
                    elemN = elemN->below;
                }
                if (elemN != colN && elemN->line == elemM->column) {
                    sum += elemM->info * elemN->info;  // Assumindo que os elementos estão alinhados
                }
            }
            if (sum != 0.0) {
                Matrix* newRowElem = create_node(rowM->line, colN->column, sum);
                if (!newRowElem) {
                    printf("Falha ao criar novo elemento para a matriz resultante!");
                    return -1;
                }
                // Inserir newRowElem na matriz r na posição correta
                Matrix* rowHeadR = *r;
                rowHeadR = rowHeadR->below;
                while (rowHeadR->line < newRowElem->line) {
                    rowHeadR = rowHeadR->below;
                }
                Matrix* lastElem = rowHeadR;
                while (lastElem->right != rowHeadR && lastElem->right->column < newRowElem->column) {
                    lastElem = lastElem->right;
                }
                newRowElem->right = lastElem->right;
                lastElem->right = newRowElem;
            }
        }
    }
    return 0;
}

int matrix_transpose(const Matrix* m, Matrix** r){


    return 0;
}

// Parte 3 (Get e Set)
int matrix_getelem(const Matrix *m, int x, int y, float *elem) {
    if (!m) {
        printf("Matriz não existe");
        return -1;
    }

    const Matrix *rowHead = m->below; // Começa do primeiro nó cabeça de linha
    
    int countRow = 1; // Contador para percorrer as linhas e colunas
    while (rowHead != m && countRow < x) {
        rowHead = rowHead->below;
        countRow++;
    }
    if (rowHead == m || countRow != x) {
        printf("Linha não encontrada!");
        return -1; // Linha x não encontrada
    }

    int countCol = 1;
    Matrix *colHead = m->right; // Começa do primeiro nó cabeça de coluna
    while (colHead != m && countCol < y) {
        colHead = colHead->right;
        countCol++;
    }
    if (colHead == m || countCol != y) {
        printf("Coluna não encontrada!");
        return -1; // Coluna y não encontrada
    }

    // Assim eu verifico tanto se a linha quanto a coluna são válidas.

    // Navegar até a coluna correta na linha x
    const Matrix *current = rowHead->right;
    while (current != rowHead && current->column < y) {
        current = current->right;
    }

    // Verificar se o elemento existe na coluna y
    if (current == rowHead || current->column != y) {
        // *elem = 0.0; // Elemento não encontrado na coluna y, elemento é zero
        printf("Elemento em (%d, %d) é 0.0", x, y);
        return 0;
    } else {
        *elem = current->info; // Se encontrado, copiar o valor para *elem
        printf("Elemento em (%d, %d) é %f\n", x, y, *elem);
        return 0;
    }


}

int matrix_setelem(Matrix *m, int x, int y, float elem) {
    Matrix *rowHead = m->below; // Começa do primeiro nó cabeça de linha
    
    int countRow = 1; // Contador para percorrer as linhas e colunas
    while (rowHead != m && countRow < x) {
        rowHead = rowHead->below;
        countRow++;
    }
    if (rowHead == m || countRow != x) {
        printf("Linha não encontrada!");
        return -1; // Linha x não encontrada
    }

    int countCol = 1;
    Matrix *colHead = m->right; // Começa do primeiro nó cabeça de coluna
    while (colHead != m && countCol < y) {
        colHead = colHead->right;
        countCol++;
    }
    if (colHead == m || countCol != y) {
        printf("Coluna não encontrada!");
        return -1; // Coluna y não encontrada
    }
    
    // Aqui eu já tenho os nós cabeças de linha (rowHead) e de coluna (colHead)

    Matrix *currRow = rowHead->right;
    Matrix *prevRow = rowHead;
    while (currRow != rowHead && currRow->column < y) {
        prevRow = currRow;
        currRow = currRow->right;
    }

    Matrix *currCol = colHead->below;
    Matrix *prevCol = colHead;
    while (currCol != colHead && currCol->line < x) {
        prevCol = currCol;
        currCol = currCol->below;
    }

    /* Na primeira vez, os curr e prev vão ser iguais. Depois em Row eles vão andando para direita
    e em Col para baixo. Ao final o curr volta para o head se o nó não existir e o prev salva o último nó. */

    // Se o nó já existe na posição correta, atualize o valor
    if (currRow != rowHead && currRow->column == y && currCol != colHead && currCol->line == x) {
        currRow->info = elem;
        // ou currCol->info = elem;
        return 0;
    }
    // Se não, criar um novo nó e inserir antes de `currRow` e `currCol`
    else {
        Matrix *newNode = create_node(x, y, elem);
        if (!newNode) {
            printf("Erro na alocação de memória!");
            return -1; // Falha na alocação de memória
        }
        newNode->right = currRow;
        prevRow->right = newNode;
        newNode->below = currCol;
        prevCol->below = newNode;

        return 0;
    }
}

// Main Teste
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