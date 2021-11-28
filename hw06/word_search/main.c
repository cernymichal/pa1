#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define VECTOR_INITIAL_SIZE 16
#define VECTOR_SIZE_FACTOR 2

// a way of reading from grid moving in d_{x, y} from x, y for n characters
typedef struct GridReading_s {
    int x;
    int y;
    int d_x;
    int d_y;
    int n;
} GridReading;

// get the char in grid at index of reading
char grid_reading_get(char ** grid, GridReading * reading, int i) {
    return grid[reading->x + reading->d_x * i][reading->y + reading->d_y * i];
}

// compare two readings like strings
int grid_reading_compare(char ** grid, GridReading * reading_a, GridReading * reading_b) {
    if (reading_a->n != reading_b->n)
        return 0;

    for (int i = 0; i < reading_a->n; i++)
        if (grid_reading_get(grid, reading_a, i) != grid_reading_get(grid, reading_b, i))
            return 0;

    return 1;
}

void grid_reading_print(char ** grid, GridReading * reading) {
    for (int i = 0; i < reading->n; i++)
        printf("%c", grid_reading_get(grid, reading, i));

    printf("\n");
}

typedef struct VectorGridReading_s {
    GridReading * data;
    int capacity;
    int n;
} VectorGridReading;

VectorGridReading * vector_reading_new() {
    VectorGridReading * vector = (VectorGridReading *) malloc(sizeof(VectorGridReading));

    vector->capacity = VECTOR_INITIAL_SIZE;
    vector->n = 0;
    vector->data = (GridReading *) malloc(vector->capacity * sizeof(GridReading));

    return vector;
}

void vector_reading_free(VectorGridReading * vector) {
    if (!vector)
        return;

    free(vector->data);
    free(vector);
}

// mark the vector as empty, doesn't free readings since they arent allocated
void vector_reading_clear(VectorGridReading * vector) {
    vector->n = 0;
}

void vector_reading_push(VectorGridReading * vector, GridReading value) {
    if (vector->n + 1 > vector->capacity) {
        vector->capacity *= VECTOR_SIZE_FACTOR;
        vector->data = (GridReading *) realloc(vector->data, vector->capacity * sizeof(GridReading));
    }

    vector->data[vector->n++] = value;
}

// only push readings that dont have an equivalent in vector
void vector_reading_merge_unique(char ** grid, VectorGridReading * vector, VectorGridReading * to_merge) {
    // realloc vector as if all elements were to be merged
    if (vector->n + to_merge->n > vector->capacity) {
        while (vector->n + to_merge->n > vector->capacity)
            vector->capacity *= VECTOR_SIZE_FACTOR;
        vector->data = (GridReading *) realloc(vector->data, vector->capacity * sizeof(GridReading));
    }

    for (int i = 0; i < to_merge->n; i++) {
        // skip non unique elements
        int skip = 0;
        for (int j = 0; j < vector->n; j++) {
            if (grid_reading_compare(grid, &to_merge->data[i], &vector->data[j])) {
                skip = 1;
                break;
            }
        }
        if (skip) continue;

        vector->data[vector->n++] = to_merge->data[i];
    }
}

typedef struct State_s {
    char ** grid;
    int n;
    VectorGridReading * longest_words;
} State;

void free_state(State * state) {
    // grid
    for (int i = 0; i < state->n; i++)
        free(state->grid[i]);

    free(state->grid);

    // longest_words
    vector_reading_free(state->longest_words);
}

    }
}

// generates all possible readings of a given grid
// *skipping one character diagonals
//     horizonal           (forwards, backwards) * n
//     vertical            (forwards, backwards) * n
//     diagonal from left  (forwards, backwards) * (2n - 3)
//     diagonal from right (forwards, backwards) * (2n - 3)
VectorGridReading * create_grid_readings(State * state) {
    VectorGridReading * grid_readings = vector_reading_new();

    // horizonal
    for (int x = 0; x < state->n; x++) {
        GridReading reading = {
            x,
            0,
            0,
            1,
            state->n
        };
        GridReading reading_reverse = {
            x,
            state->n - 1,
            0,
            -1,
            state->n
        };
        vector_reading_push(grid_readings, reading);
        vector_reading_push(grid_readings, reading_reverse);
    }

    // vertical
    for (int y = 0; y < state->n; y++) {
        GridReading reading = {
            0,
            y,
            1,
            0,
            state->n
        };
        GridReading reading_reverse = {
            state->n - 1,
            y,
            -1,
            0,
            state->n
        };
        vector_reading_push(grid_readings, reading);
        vector_reading_push(grid_readings, reading_reverse);
    }

    // diagonal right
    for (int x = 0; x < state->n - 1; x++) {
        GridReading reading = {
            x,
            0,
            1,
            1,
            state->n - x
        };
        GridReading reading_reverse = {
            state->n - 1 - x,
            state->n - 1,
            -1,
            -1,
            state->n - x
        };
        vector_reading_push(grid_readings, reading);
        vector_reading_push(grid_readings, reading_reverse);
    }
    for (int y = 1; y < state->n - 1; y++) {
        GridReading reading = {
            0,
            y,
            1,
            1,
            state->n - y
        };
        GridReading reading_reverse = {
            state->n - 1,
            state->n - 1 - y,
            -1,
            -1,
            state->n - y
        };
        vector_reading_push(grid_readings, reading);
        vector_reading_push(grid_readings, reading_reverse);
    }

    // diagonal right
    for (int x = 0; x < state->n - 1; x++) {
        GridReading reading = {
            x,
            state->n - 1,
            1,
            -1,
            state->n - x
        };
        GridReading reading_reverse = {
            state->n - 1 - x,
            0,
            -1,
            1,
            state->n - x
        };
        vector_reading_push(grid_readings, reading);
        vector_reading_push(grid_readings, reading_reverse);
    }
    for (int y = 1; y < state->n - 1; y++) {
        GridReading reading = {
            state->n - 1,
            y,
            -1,
            1,
            state->n - y
        };
        GridReading reading_reverse = {
            0,
            state->n - 1 - y,
            1,
            -1,
            state->n - y
        };
        vector_reading_push(grid_readings, reading);
        vector_reading_push(grid_readings, reading_reverse);
    }

    return grid_readings;
}

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

    int max_length = 0;

    if (!max_length)
        return print_no_reoccurring_words();
    
    printf("Nejdelsi opakujici se slova:\n");
    for (int i = 0; i < state->longest_words->n; i++)
        grid_reading_print(state->grid, &state->longest_words->data[i]);

    return 0;
}

int main (void) {
    State state = {
        NULL,
        0,
        vector_reading_new()
    };

    int exit_code = main_loop(&state);

    free_state(&state);

    return exit_code;
}

