#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define VECTOR_INITIAL_SIZE 16
#define VECTOR_SIZE_FACTOR 2

// ============================== VectorChar
// arraylist wrapper for chars
// null terminated

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

// print vector with chars at indicies converted to upper case
void vector_char_print_upper(VectorChar * vector, int * indicies, int indicies_count) {
    int i = 0; // position in vector

    for (int j = 0; j < indicies_count; j++) {
        for (; i < indicies[j]; i++)
            printf("%c", vector->data[i]);

        printf("%c", toupper(vector->data[i]));
        i++;
    }

    for (; i < vector->n; i++)
        printf("%c", vector->data[i]);
}

// calls vector_char_print_upper but wraps it in quotes and adds a new line
void vector_char_print_upper_qnl(VectorChar * vector, int * indicies, int indicies_count) {
    printf("\"");
    vector_char_print_upper(vector, indicies, indicies_count);
    printf("\"\n");
}

// /============================= VectorChar

// ============================== State

typedef struct State_s {
    VectorChar * abbreviation;
} State;

// doesn't free state itself, only contents
void free_state(State * state) {
    vector_char_free(state->abbreviation);
}

// /============================= State

// ============================== Output

int print_invalid_input(void) {
    printf("Nespravny vstup.\n");
    return 1;
}

void print_abbreviation_count(long long count) {
    printf("> %lld\n", count);
}

void print_abbreviation_limit_count(int limit, long long count) {
    printf("> limit %d: %lld\n", limit, count);
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

    free(line);

    // check for empty abbreviation
    if (!state->abbreviation->n)
        return INPUT_INVALID;

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

// ============================== Phrase
// current phrase parsed from a user command

typedef struct Phrase_s {
    VectorChar * phrase;
    int show_solutions;
    int chars_in_word;
    int * indicies; // for tracing back solutions while printing
} Phrase;

// doesn't free phrase itself, only content
void free_phrase(Phrase * phrase) {
    vector_char_free(phrase->phrase);
    free(phrase->indicies);
}

// /============================= Phrase

// ran for each char of abbreviation
// current_index - starting index in phrase
// current_char - index of searched char in abbreviation
// word_left - chars left in current word
// returns the number of possible solutions
long long solve_abbreviation_rec(State * state, Phrase * phrase, int current_index, int current_char, int word_left) {
    long long solutions = 0;

    // walk the phrase while the abbreviation can fit
    for (; current_index < phrase->phrase->n - (state->abbreviation->n - current_char - 1); current_index++) {
        char phrase_char = phrase->phrase->data[current_index];

        if (
            word_left
            && phrase_char == state->abbreviation->data[current_char]
        ) {
            phrase->indicies[current_char] = current_index;

            // if last char was found
            if (!state->abbreviation->data[current_char + 1]) {
                if (phrase->show_solutions)
                    vector_char_print_upper_qnl(phrase->phrase, phrase->indicies, state->abbreviation->n);

                solutions++;
            }
            else
                solutions += solve_abbreviation_rec(state, phrase, current_index + 1, current_char + 1, word_left - 1);
        }
        else if (isspace(phrase_char))
            word_left = phrase->chars_in_word;
    }

    return solutions;
}

// wraps solve_abbreviation_rec with starting parameters
long long solve_abbreviation(State * state, Phrase * phrase) {
    return solve_abbreviation_rec(state, phrase, 0, 0, phrase->chars_in_word);
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
        Phrase phrase;

        // command input
        switch (input_command(&op, &phrase.chars_in_word, &phrase.phrase)) {
            case INPUT_INVALID:
                return print_invalid_input();
            case INPUT_EOF:
                return 0;
            default:
                break;
        }
        
        phrase.show_solutions = op == '?' ? 1 : 0;
        phrase.indicies = (int *) malloc(state->abbreviation->n * sizeof(int));

        if (phrase.show_solutions) {
            print_abbreviation_count(
                solve_abbreviation(state, &phrase)
            );
        }
        else {
            int max = phrase.chars_in_word;
            for (int i = 1; i <= max; i++) {
                phrase.chars_in_word = i;
                print_abbreviation_limit_count(
                    i,
                    solve_abbreviation(state, &phrase)
                );
            }
        }

        free_phrase(&phrase);
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

