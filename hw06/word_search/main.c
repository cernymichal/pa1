#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct State_s {
    char ** grid;
    int n;
} State;

void free_state(State * state) {
    for (int i = 0; i < state->n; i++)
        free(state->grid[i]);

    free(state->grid);
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

    if (state->n == -1)
        return 0;

    state->n--; // \n

    state->grid = (char **) calloc(1, state->n * sizeof(*state->grid));

    state->grid[0] = first_line;
    state->grid[0][state->n] = '\0'; // hide \n

    for (int i = 1; i < state->n; i++) {
        size_t line_capacity;

        if (getline(&state->grid[i], &line_capacity, stdin) - 1 != state->n)
            return 0;

        state->grid[i][state->n] = '\0';
    }

    return 1;
}

// main part of program
//     moved here so that memory can be managed in main
// return programs exit code
int main_loop(State * state) {
    if (!get_grid(state))
        return print_invalid_input();

    for (int i = 0; i < state->n; i++) {
        //for (int j = 0; j < state->n; j++)
        //    printf("%c", state->grid[i][j]);

        printf("%s\n", state->grid[i]);
    }

    return 0;
}

int main (void) {
    State state;
    state.grid = NULL;

    int exit_code = main_loop(&state);

    free_state(&state);

    return exit_code;
}

