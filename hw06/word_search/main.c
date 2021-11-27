#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define CHAR_COUNT (UCHAR_MAX + 1)

typedef struct HeapNode_s {
    int count;
    struct HeapNode_s * parent;
    struct HeapNode_s * children; // array of CHAR_COUNT
} HeapNode;

HeapNode * new_heap_node_row(HeapNode * parent) {
    HeapNode * array = (HeapNode *) malloc(CHAR_COUNT * sizeof(*array));
    
    for (int i = 0; i < CHAR_COUNT; i++) {
        HeapNode * node = &array[i];
        node->count = 0;
        node->parent = parent;
        node->children = NULL;
    }

    return array;
}

// ! doesn't actually free the node itself
void free_heap_node(HeapNode * node) {
    if (!node || !node->children)
        return;

    for (int i = 0; i < CHAR_COUNT; i++)
        free_heap_node(&node->children[i]);

    // since the nodes are stored in the array just free that
    free(node->children);
}

typedef struct CharNode_s {
    char character;
    struct CharNode_s * next; // horizontally
    struct CharNode_s * children; // linked list, vertically
    struct CharNode_s * children_end;
} CharNode;

CharNode * new_char_node(void) {
    return (CharNode *) calloc(1, sizeof(CharNode));
}

void free_char_node_list(CharNode ** first, CharNode ** last) {
    while (*first) {
        CharNode * next = (*first)->next;
        free_char_node_list(&(*first)->children, &(*first)->children_end);
        free(*first);
        *first = next;
    }

    *last = NULL;
}

typedef struct State_s {
    char ** grid;
    int n;
    HeapNode * heap;
    CharNode * words;
} State;

void free_state(State * state) {
    // grid
    for (int i = 0; i < state->n; i++)
        free(state->grid[i]);

    free(state->grid);

    // heap
    if (state->heap)
        for (int i = 0; i < CHAR_COUNT; i++)
            free_heap_node(&state->heap[i]);

    free(state->heap);

    // words
    CharNode * last;
    free_char_node_list(&state->words, &last);
}

typedef struct String_s {
    char * string;
    int capacity;
    int n;
} String;

String * new_string() {
    String * string = (String *) malloc(sizeof(String));

    string->capacity = 16;
    string->n = 0;
    string->string = (char *) malloc(string->capacity * sizeof(char));
    string->string[0] = 0;

    return string;
}

void free_string(String * string) {
    free(string->string);
    free(string);
}

String * string_copy(String * string) {
    String * copied = new_string();
    
    copied->capacity = string->capacity;
    copied->n = string->n;
    copied->string = (char *) malloc(copied->capacity * sizeof(char));
    strcpy(copied->string, string->string);

    return copied;
}

void string_push(String * string, char character) {
    if (string->n + 2 > string->capacity) {
        string->capacity *= 2;
        string->string = (char *) realloc(string->string, string->capacity * sizeof(char));
    }

    string->string[string->n++] = character;
    string->string[string->n] = 0;
}

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

// recursivelly search through the heap for the longes repeating words
CharNode * find_max_depth(HeapNode * children, int * max_depth, int current_depth) {
    CharNode * char_node = new_char_node();

    if (current_depth > *max_depth)
        *max_depth = current_depth;

    int my_max_depth = current_depth;

    if (children) {
        for (int i = 0; i < CHAR_COUNT; i++) {
            HeapNode * node = &children[i];
            if (node->count > 1) {
                int old_max_depth = my_max_depth;
                CharNode * child_char = find_max_depth(node->children, &my_max_depth, current_depth + 1);
                
                if (!child_char || (current_depth == 0 && node->count == 8 && my_max_depth == 1)) {
                    my_max_depth = old_max_depth;
                    continue;
                }

                child_char->character = i + CHAR_MIN;
            
                if (my_max_depth > old_max_depth && char_node->children)
                    free_char_node_list(&char_node->children, &char_node->children_end);

                if (!char_node->children)
                    char_node->children = child_char;
                else
                    char_node->children_end->next = child_char;

                char_node->children_end = child_char;
            }
        }
    }
    
    if (my_max_depth < *max_depth)
        return NULL;

    *max_depth = my_max_depth;

    return char_node;
}

int print_invalid_input(void) {
    printf("Nespravny vstup.\n");
    return 1;
}

int print_no_reoccurring_words(void) {
    printf("Zadne opakujici se slovo.\n");
    return 0;
}

// recusivelly print all words from node, storing the prefix in beggining
void print_words(String * beggining, CharNode * node) {
    if (!node->children) {
        printf("%s%c\n", beggining->string, node->character);
        return;
    }

    if (node->character) {
        beggining = string_copy(beggining);
        string_push(beggining, node->character);
    }

    for (CharNode * child = node->children; child; child = child->next)
        print_words(beggining, child);
    
    free_string(beggining);
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

    // add all possible words to heap
    for (int x = 0; x < state->n; x++)
        for (int y = 0; y < state->n; y++) {
            add_directions_to_heap(state, x, y);
            printf("%lf%%\n", (x * y) / (double)(state->n * state->n)); 
    }

    int max_depth = 0;
    state->words = find_max_depth(state->heap, &max_depth, 0);
    
    if (!state->words->children)
        return print_no_reoccurring_words();
    
    printf("Nejdelsi opakujici se slova:\n");
    print_words(new_string(), state->words);

    return 0;
}

int main (void) {
    State state;
    state.grid = NULL;
    state.heap = NULL;
    state.words = NULL;

    int exit_code = main_loop(&state);

    free_state(&state);

    return exit_code;
}

