#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void print_array(int * array, int n) {
    for (int i = 0; i < n; i++)
        printf("%d, ", array[i]);

    printf("\n");
}

void fill_zero(int * array, int n) {
    for (int i = 0; i < n; i++)
        array[i] = 0;
}

void fill_numbers(int * array, int n) {
    for (int i = 0; i < n; i++)
        array[i] = i + 1;
}

void fill_range(int * array, int n, int a) {
    for (int i = 0; i < n; i++)
        array[i] = a + i;
}

void fill_rand(int * array, int n) {
    for (int i = 0; i < n; i++)
        array[i] = rand();
}

int main (void) {
    int array[10];

    fill_rand(array, 10);
    print_array(array, 10);

    return 0;
}

