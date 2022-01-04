#include <stdio.h>
#include <stdlib.h>

typedef struct Vector_s {
    double * array;
    int n;
    int capacity;
} Vector;

void reallocate_vector(Vector * vec, int capacity) {
    vec->array = (double *) realloc(vec->array, capacity * sizeof(*vec->array));
    vec->capacity = capacity;
}

void free_vector(Vector * vec) {
    free(vec->array);
    free(vec);
}

Vector * new_vector() {
    Vector * vec = (Vector *) calloc(1, sizeof(*vec));
    reallocate_vector(vec, 10);    
    return vec;
}

void vector_push(Vector * vec, double value) {
    if (vec->capacity >= vec->n)
        reallocate_vector(vec, vec->capacity * 2);

    vec->array[vec->n] = value;
    vec->n++;
}

double vector_pop(Vector * vec) {
    vec->n--;
    double value = vec->array[vec->n];
    
    if (vec->n <= vec->capacity / 2)
        reallocate_vector(vec, vec->capacity / 2);

    return value;
}

int double_cmp(const void *a, const void *b) {
    double arg1 = *(const double *)a;
    double arg2 = *(const double *)b;

    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    
    return 0;
}

void vector_sort(Vector * vec) {
    qsort(vec->array, vec->n, sizeof(double), double_cmp);
}

void print_vector(Vector * vec) {
    for (int i = 0; i < vec->n; i++)
        printf("%lf, ", vec->array[i]);

    printf("\n");
}

int main (void) {
    Vector * v = new_vector();

    vector_push(v, 10.);
    vector_push(v, 16.);
    vector_push(v, 0.);
    vector_push(v, 1.5);

    vector_pop(v);

    vector_sort(v);

    print_vector(v);

    return 0;
}

