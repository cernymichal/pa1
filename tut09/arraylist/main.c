#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAYLIST_INITIAL 10
#define ARRAYLIST_MULTIPLIER 2

int * new_array(int n) {
    return (int *) malloc(sizeof(int) * n);
}

int new_array_from_input(int ** array) {
    int n = ARRAYLIST_INITIAL;
    *array = (int *) calloc(n, sizeof(**array));
    
    for (int i = 0; ; i++) {
        int value;
        if (scanf(" %d", &value) != 1)
            break;

        if (i >= n) {
            n *= ARRAYLIST_MULTIPLIER;
            int * old = *array;
            *array = (int *) realloc(*array, sizeof(**array) * n);
            printf("%p %p\n", (void *)old, (void *)*array);
        }

        (*array)[i] = value;
    }
    
    return n;
}

void fill_array(int * array, int n, int value) {
    for (n -= 1; n >= 0; n--)
        array[n] = value;
}

void fill_array_rand(int * array, int n, int a, int b) {
    for (n -= 1; n >= 0; n--)
        array[n] = a + (double)rand() / RAND_MAX * (b - a);
}

void print_array(int * array, int n) {
    for (int i = 0; i < n; i++)
        printf("%d, ", array[i]);

    printf("\n");
}

void free_array(int * array) {
    free(array);
}

int main (void) {
    srand(time(NULL));
    int * array;
    int n = new_array_from_input(&array);
    print_array(array, n);
    free_array(array);
    return 0;
}

