#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define CHAR_COUNT (UCHAR_MAX + 1)

typedef struct HeapNode_s {
    int count;
    struct HeapNode_s * children;
} HeapNode;

typedef struct State_s {
    char ** grid;
    int n;
    HeapNode * heap;
} State;

HeapNode * new_heap_node_row() {
    return (HeapNode *) calloc(CHAR_COUNT, sizeof(HeapNode));
}

// ! doesn't actually free the node itself
void free_heap_node(HeapNode * node) {
    if (!node->children)
        return;

    for (int i = 0; i < CHAR_COUNT; i++)
        free_heap_node(&node->children[i]);

    // since the nodes are stored in the array just free that
    free(node->children);
}

void free_state(State * state) {
    // grid
    for (int i = 0; i < state->n; i++)
        free(state->grid[i]);

    free(state->grid);

    // heap
    for (int i = 0; i < CHAR_COUNT; i++)
        free_heap_node(&state->heap[i]);

    free(state->heap);
}

int print_invalid_input() {
    printf("Nespravny vstup.\n");
    return 1;
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

    return 0;
}

int main (void) {
    State state;
    state.grid = NULL;
    state.heap = new_heap_node_row();

    int exit_code = main_loop(&state);

    free_state(&state);

    return exit_code;
}

