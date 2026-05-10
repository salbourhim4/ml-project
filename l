[1mdiff --git a/src/matrix.c b/src/matrix.c[m
[1mindex 24043d8..5b27519 100644[m
[1m--- a/src/matrix.c[m
[1m+++ b/src/matrix.c[m
[36m@@ -33,4 +33,46 @@[m [mvoid arena_destroy(Arena *a) {[m
     a->start = NULL;     [m
     a->head = NULL;[m
     a->capacity = 0;  [m
[32m+[m[32m}[m[41m[m
[32m+[m[41m[m
[32m+[m[32m// Matrix Operations[m[41m[m
[32m+[m[41m[m
[32m+[m[32mMatrix matrix_create(Arena *a, int rows, int cols) {[m[41m[m
[32m+[m[32m    Matrix m = {0};[m[41m[m
[32m+[m[32m    float *data = arena_alloc(a,  rows * cols * sizeof(float));[m[41m[m
[32m+[m[32m    if (!data) {[m[41m[m
[32m+[m[32m        return m;   // m is {0} so m.data will be NULL, caller can check for it[m[41m[m
[32m+[m[32m    }[m[41m[m
[32m+[m[32m    m.rows = rows;[m[41m[m
[32m+[m[32m    m.cols = cols;[m[41m[m
[32m+[m[32m    m.data = data;[m[41m[m
[32m+[m[32m    return m;[m[41m[m
[32m+[m[32m}[m[41m[m
[32m+[m[41m[m
[32m+[m[32mfloat matrix_get(Matrix *m, int i, int j) {[m[41m[m
[32m+[m[32m    int pos = i * m->cols + j;[m[41m[m
[32m+[m[32m    return m->data[pos];[m[41m[m
[32m+[m[32m}[m[41m[m
[32m+[m[41m[m
[32m+[m[32mvoid matrix_set(Matrix *m, int i, int j, float val) {[m[41m[m
[32m+[m[32m    int pos = i * m->cols + j;[m[41m[m
[32m+[m[32m    m->data[pos] = val;[m[41m[m
[32m+[m[32m}[m[41m[m
[32m+[m[41m[m
[32m+[m[32mvoid matrix_add(Matrix *a, Matrix *b, Matrix *result) { // assuming both are n x n matrices[m[41m [m
[32m+[m[32m    for (int i = 0; i < a->rows; i++) {[m[41m[m
[32m+[m[32m        for (int j = 0; j < a->cols; j++) {[m[41m[m
[32m+[m[32m            float num = matrix_get(a, i, j) + matrix_get(b, i, j);[m[41m[m
[32m+[m[32m            matrix_set(result, i, j, num);[m[41m[m
[32m+[m[32m        }[m[41m[m
[32m+[m[32m    }[m[41m[m
[32m+[m[32m}[m[41m[m
[32m+[m[41m[m
[32m+[m[32mvoid matrix_sub(Matrix *a, Matrix *b, Matrix *result) {[m[41m[m
[32m+[m[32m    for (int i = 0; i < a->rows; i++) {[m[41m[m
[32m+[m[32m        for (int j = 0; j < a->cols; j++) {[m[41m[m
[32m+[m[32m            float num = matrix_get(a, i, j) - matrix_get(b, i, j);[m[41m[m
[32m+[m[32m            matrix_set(result, i, j, num);[m[41m[m
[32m+[m[32m        }[m[41m[m
[32m+[m[32m    }[m[41m[m
 }[m
\ No newline at end of file[m
