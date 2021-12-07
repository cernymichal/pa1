#include <stdio.h>
#include <stdlib.h>

#define VECTOR_INITIAL_SIZE 10
#define VECTOR_SIZE_FACTOR 2

typedef struct VectorInt_s {
    int capacity;
    int n;
    int * data;
} VectorInt;

VectorInt * vector_int_new(void) {
    VectorInt * vector = (VectorInt *) malloc(sizeof(VectorInt));

    vector->capacity = VECTOR_INITIAL_SIZE;
    vector->n = 0;
    vector->data = (int *) malloc(vector->capacity * sizeof(int));

    return vector;
}

void vector_int_free(VectorInt * vector) {
    if (!vector)
        return;

    free(vector->data);
    free(vector);
}

void vector_int_clear(VectorInt * vector) {
    vector->n = 0;
}

void vector_int_push(VectorInt * vector, int value) {
    if (vector->n + 1 > vector->capacity) {
        vector->capacity *= VECTOR_SIZE_FACTOR;
        vector->data = (int *) realloc(vector->data, vector->capacity * sizeof(int));
    }

    vector->data[vector->n++] = value;
}

int compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
 
    if (arg1 < arg2) return 1;
    if (arg1 > arg2) return -1;
    return 0;
}

void vector_int_sort(VectorInt * vector) {
    qsort(vector->data, vector->n, sizeof(int), compare_ints);
}

int input_coins(VectorInt ** coin_values) {
    *coin_values = vector_int_new();

    int value;
    while (scanf(" %d", &value) == 1 && value > 0)
        vector_int_push(*coin_values, value);

    if (value || (*coin_values)->n == 0) {
        vector_int_free(*coin_values);
        return 0;
    }

    return 1;
}

int input_value(int * value) {
    if (scanf(" %d", value) != 1 || *value <= 0)
        return feof(stdin) + 1;

    return 0;
}

int print_invalid_input(void) {
    printf("Nespravny vstup.\n");
    return 1;
}

int solve_value(VectorInt * coins, int value, int index) {
    if (!value)
        return 0;

    if (index == coins->n && value)
        return -1;

    int coin = coins->data[index];
    int coin_count = value / coin;

    printf("%d - %dx\n", coin, coin_count);

    int rest = solve_value(coins, value % coin, index + 1);

    if (rest == -1)
        return solve_value(coins, value, index + 1);

    return rest + coin_count;
}

int main(void) {
    VectorInt * coins;

    printf("Mince:\n");

    if (!input_coins(&coins))
        return print_invalid_input();
    
    vector_int_sort(coins);
    
    printf("Castky:\n");

    while (1) {
        int value;
        switch (input_value(&value)) {
            case 1:
                vector_int_free(coins);
                return print_invalid_input();
            case 2:
                vector_int_free(coins);
                return 0; 
            default:
                break;
        }

        int min_coins = solve_value(coins, value, 0);

        printf("%d ", value);
        if (min_coins = -1)
            printf("= nema reseni\n");
        else
            printf("= %d\n", min_coins);
    }

    vector_int_free(coins);

    return 0;
}
