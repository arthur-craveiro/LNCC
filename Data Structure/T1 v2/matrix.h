typedef struct matrix Matrix;
//Obs: Toda operação retornará 0 em caso de sucesso e =/= 0 em caso de erro.

int matrix_create(Matrix** m);
int matrix_destroy(Matrix* m );

int matrix_print(const Matrix* m);
int matrix_add(const Matrix* m, const Matrix* n, Matrix** r);
int matrix_multiply(const Matrix* m, const Matrix* n, Matrix** r);
int matrix_transpose(const Matrix* m, Matrix** r);

int matrix_getelem(const Matrix *m, int x, int y, float *elem);
int matrix_setelem(Matrix *m, int x, int y, float elem);