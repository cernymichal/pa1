#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define VECTOR_INITIAL_SIZE 16
#define VECTOR_SIZE_FACTOR 2

typedef struct State_s {
    char ** grid;
    int n;
} State;

void free_state(State * state) {
    // grid
    for (int i = 0; i < state->n; i++)
        free(state->grid[i]);

    free(state->grid);
}

typedef struct VectorChar_s {
    char * string;
    int capacity;
    int n;
} VectorChar;

VectorChar * vector_char_new() {
    VectorChar * vector = (VectorChar *) malloc(sizeof(VectorChar));

    vector->capacity = VECTOR_INITIAL_SIZE;
    vector->n = 0;
    vector->string = (char *) malloc(vector->capacity * sizeof(char));
    vector->string[0] = 0;

    return vector;
}

void vector_charfree(VectorChar * vector) {
    free(vector->string);
    free(vector);
}

VectorChar * vector_char_copy(VectorChar * vector) {
    VectorChar * copied = vector_char_new();

    copied->capacity = vector->capacity;
    copied->n = vector->n;
    copied->string = (char *) malloc(copied->capacity * sizeof(char));
    strcpy(copied->string, vector->string);

    return copied;
}

void vector_char_push(VectorChar * vector, char character) {
    if (vector->n + 2 > vector->capacity) {
        vector->capacity *= VECTOR_SIZE_FACTOR;
        vector->string = (char *) realloc(vector->string, vector->capacity * sizeof(char));
    }

    vector->string[vector->n++] = character;
    vector->string[vector->n] = 0;
}

typedef struct VectorInt_s {
    int * data;
    int capacity;
    int n;
} VectorInt;

VectorInt * vector_int_new() {
    VectorInt * vector = (VectorInt *) malloc(sizeof(VectorInt));

    vector->capacity = VECTOR_INITIAL_SIZE;
    vector->n = 0;
    vector->data = (int *) malloc(vector->capacity * sizeof(int));

    return vector;
}

void vector_int_free(VectorInt * vector) {
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

void vector_int_merge(VectorInt * vector, VectorInt * to_merge) {
    if (vector->n + to_merge->n > vector->capacity) {
        while (vector->n + to_merge->n > vector->capacity)
            vector->capacity *= VECTOR_SIZE_FACTOR;
        vector->data = (int *) realloc(vector->data, vector->capacity * sizeof(int));
    }

    for (int i = 0; i < to_merge->n; i++)
        vector->data[vector->n++] = to_merge->data[i];
}

/*
// add one of the readings (x_d, y_d) from x, y to the heap
void add_word_to_heap(State * state, int x, int y, int x_d, int y_d) {
    HeapNode ** children = &state->heap;
    HeapNode * node = NULL;

    while (x >= 0 && x < state->n && y >= 0 && y < state->n) {
        if (!*children)
            *children = new_heap_node_row(node);

        char c = state->grid[x][y];
        node = &(*children)[c - CHAR_MIN];
        node->count++;

        children = &node->children;

        x += x_d;
        y += y_d;
    }
}

// add all possible readings from x, y to the heap
void add_directions_to_heap(State * state, int x, int y) { 
    add_word_to_heap(state, x, y, 0, 1);   // right
    add_word_to_heap(state, x, y, 0, -1);  // left
    add_word_to_heap(state, x, y, 1, 0);   // down
    add_word_to_heap(state, x, y, -1, 0);  // up
    add_word_to_heap(state, x, y, 1, 1);   // right-down
    add_word_to_heap(state, x, y, 1, -1);  // left-down
    add_word_to_heap(state, x, y, -1, 1);  // right-up
    add_word_to_heap(state, x, y, -1, -1); // left-up
}
*/

int print_invalid_input(void) {
    printf("Nespravny vstup.\n");
    return 1;
}

int print_no_reoccurring_words(void) {
    printf("Zadne opakujici se slovo.\n");
    return 0;
}

// load grid from stdin
// return 1 if successful
int get_grid(State * state) {
    char * first_line = NULL;
    size_t first_line_capacity;

    state->n = getline(&first_line, &first_line_capacity, stdin);
    state->n--; // \n

    if (state->n <= 0)
        return 0;

    state->grid = (char **) calloc(1, state->n * sizeof(*state->grid));

    state->grid[0] = first_line;
    state->grid[0][state->n] = '\0'; // hide \n

    for (int i = 1; i < state->n; i++) {
        size_t line_capacity;

        if (getline(&state->grid[i], &line_capacity, stdin) - 1 != state->n)
            return 0;

        state->grid[i][state->n] = '\0';
    }

    // check for more input
    char c = getc(stdin);
    if (!(c == EOF || c == '\n'))
        return 0;

    return 1;
}

// main part of program
//     moved here so that memory can be managed in main
// return programs exit code
int main_loop(State * state) {
    printf("Hlavolam:\n");

    if (!get_grid(state))
        return print_invalid_input();

    if (!1)
        return print_no_reoccurring_words();
    
    printf("Nejdelsi opakujici se slova:\n");

    return 0;
}

int main (void) {
    State state;
    state.grid = NULL;

    int exit_code = main_loop(&state);

    free_state(&state);

    return exit_code;
}

