#include <stdio.h>
#include <stdlib.h>

#define ARRAYLIST_INITIAL 10
#define ARRAYLIST_MULTIPLIER 2

char * get_line() {
    char n = ARRAYLIST_INITIAL;
    char * array = (char *) malloc(sizeof(*array) * n);
    array[0] = 0;

    for (int i = 0; ; i++) {
        char character = getc(stdin);
        if (character == EOF || character == '\n')
            break;

        if (i >= n - 1) {
            n *= ARRAYLIST_MULTIPLIER;
            array = (char *) realloc(array, sizeof(*array) * n);
        }

        array[i] = character;
        array[i + 1] = 0;
    }
    
    return array;
}

void print_array(char * array) {
    for (int i = 0; array[i] != 0; i++)
        printf("%c", array[i]);

    printf("\n");
}

int main (void) {
    char * array = get_line();
    print_array(array);
    free(array);
    return 0;
}

