#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define VECTOR_INITIAL_SIZE 16
#define VECTOR_SIZE_FACTOR 2

// ============================== VectorChar

typedef struct VectorChar_s {
    char * data;
    int capacity;
    int n;
} VectorChar;

VectorChar * vector_char_new() {
    VectorChar * vector = (VectorChar *) malloc(sizeof(VectorChar));

    vector->capacity = VECTOR_INITIAL_SIZE;
    vector->n = 0;
    vector->data = (char *) malloc(vector->capacity * sizeof(char));
    
    if (vector->capacity > 0)
        vector->data[0] = 0;

    return vector;
}

void vector_char_free(VectorChar * vector) {
    if (!vector)
        return;

    free(vector->data);
    free(vector);
}

// mark the vector as empty, doesn't free data
void vector_char_clear(VectorChar * vector) {
    vector->n = 0;
    
    if (vector->capacity > 0)
        vector->data[0] = 0;
}

void vector_char_push(VectorChar * vector, char value) {
    if (vector->n + 2 > vector->capacity) {
        vector->capacity *= VECTOR_SIZE_FACTOR;
        vector->data = (char *) realloc(vector->data, vector->capacity * sizeof(char));
    }

    vector->data[vector->n++] = value;
    vector->data[vector->n] = 0;
}

// /============================= VectorChar

// ============================== State

typedef struct State_s {
    VectorChar * abbreviation;
} State;

void free_state(State * state) {
    vector_char_free(state->abbreviation);
}

// /============================= State

// ============================== Output

int print_invalid_input(void) {
    printf("Nespravny vstup.\n");
    return 1;
}

void print_abbreviation_count(int count) {
    printf("> %d\n", count);
}

void print_abbreviation_limit_count(int limit, int count) {
    printf("> limit %d: %d\n", limit, count);
}

// /============================= Output

// ============================== Input

typedef enum InputReturn_s {
    INPUT_OK,
    INPUT_INVALID,
    INPUT_EOF
} InputReturn;

// query user for the abbreviation
InputReturn input_abbreviation(State * state) {
    char * line = NULL;
    size_t line_capacity;

    getline(&line, &line_capacity, stdin);

    // check for eof
    if (feof(stdin)) {
        free(line);
        return INPUT_EOF;
    }

    int read = 0;

    // skip whitespace
    sscanf(line, " %n", &read);

    state->abbreviation = vector_char_new();

    for (; line[read] && line[read] != '\n'; read++) {
        char c = line[read];
        
        // fail if not uppercase letter
        if (!(isalpha(c) && isupper(c))) {
            free(line);
            return INPUT_INVALID;
        }

        vector_char_push(state->abbreviation, tolower(line[read]));
    }

    // check for excess characters
    char excess = 0;
    if (sscanf(&line[read], " %1c", &excess) && excess != 0) {
        free(line);
        return INPUT_INVALID;
    }

    free(line);

    return INPUT_OK;
}

// query user for command
InputReturn input_command(char * operation, int * chars_in_word, VectorChar ** phrase) {
    char * line = NULL;
    size_t line_capacity;

    getline(&line, &line_capacity, stdin);

    // check for eof
    if (feof(stdin)) {
        free(line);
        return INPUT_EOF;
    }

    int read = 0;

    // read operator and N until "
    if (
        sscanf(line, "%1c %d %n", operation, chars_in_word, &read) != 2
        || !(*operation == '?' || *operation == '#')
        || *chars_in_word < 1
        || line[read] != '"'
    ) {
        free(line);
        return INPUT_INVALID;
    }

    *phrase = vector_char_new();

    // read phrase
    for (read++; line[read] && line[read] != '\n' && line[read] != '"'; read++)
        vector_char_push(*phrase, tolower(line[read]));

    // check for ending " and excess characters
    char excess = 0;
    if (line[read] != '"' || (sscanf(&line[read], "\" %1c", &excess) && excess != 0)) {
        free(line);
        vector_char_free(*phrase);
        return INPUT_INVALID;
    }

    free(line);

    return INPUT_OK;
}

// /============================= Input

void solve_abbreviation(State * state, VectorChar * phrase, int chars_in_word, int show_solutions) {
    printf("TODO: Solve :)\n");
}

// main part of the program
//     moved here so that memory can be managed in main
// return programs exit code
int main_loop(State * state) {
    printf("Zkratka:\n");

    // try to get abbreaviation
    switch (input_abbreviation(state)) {
        case INPUT_INVALID:
        case INPUT_EOF:
            return print_invalid_input();
        default:
            break;
    }

    printf("Problemy:\n");

    while (1) {
        char op = 0;
        int chars_in_word = 0;
        VectorChar * phrase = NULL;

        // command input
        switch (input_command(&op, &chars_in_word, &phrase)) {
            case INPUT_INVALID:
                return print_invalid_input();
            case INPUT_EOF:
                return 0;
            default:
                break;
        }
        
        int show_solutions = op == '?' ? 1 : 0;
        solve_abbreviation(state, phrase, chars_in_word, show_solutions);

        vector_char_free(phrase);
    }

    return 0;
}

int main (void) {
    State state = {
        NULL
    };

    int exit_code = main_loop(&state);

    free_state(&state);

    return exit_code;
}

