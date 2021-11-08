#include <stdio.h>
#include <stdlib.h>

#define INPUT_LINE_LENGTH 256

#define MAX_VISITS 1000000
#define MAX_ID 99999
#define ID_COUNT (MAX_ID + 1)

typedef struct State_s {
    int visits_total;
    int visits_chrono[MAX_VISITS];
    int client_visits[ID_COUNT];
} State;

State * new_state(void) {
    State * state = (State *) malloc(sizeof(State));
    state->visits_total = 0;

    for (int i = 0; i < ID_COUNT; i++)
        state->client_visits[i] = 0;

    return state;
} 

void print_visit(int count) {
    if (count == 1)
        printf("prvni navsteva\n");
    else
        printf("navsteva #%d\n", count);
}

void print_unique_visits(int unique, int total) {
    printf("%d / %d\n", unique, total);
}

int print_incorrect_input() {
    printf("Nespravny vstup.\n");
    return 1;
}

int get_input(State * state, char * op, int * arg1, int * arg2) {
    char line[INPUT_LINE_LENGTH];
    char * fgets_ret = fgets(line, INPUT_LINE_LENGTH, stdin);
    
    if (fgets_ret == NULL)
        return 2;

    int scanned = sscanf(line, " %c %d %d", op, arg1, arg2);

    if (scanned < 1 || !(*op == '+' || *op == '?'))
        return 1;

    if (*op == '+' && (scanned != 2 || *arg1 < 0 || *arg1 > MAX_ID || state->visits_total >= MAX_VISITS))
        return 1;

    if (*op == '?' && (scanned != 3 || *arg1 < 0 || *arg2 >= state->visits_total || *arg1 > *arg2))
        return 1;

    return 0;
}

int add_visit(State * state, int id) {
    state->visits_chrono[state->visits_total] = id;
    state->visits_total++;
    
    state->client_visits[id]++;

    return state->client_visits[id];
}

int count_unique_visits(State * state, int start, int end) {
    int * clients = (int *) malloc(ID_COUNT * sizeof(int));

    for (int i = 0; i < ID_COUNT; i++)
        clients[i] = 0;

    for (int i = start; i <= end; i++)
        clients[state->visits_chrono[i]] += 1;

    int unique = 0;
    for (int i = 0; i < ID_COUNT; i++)
        if (clients[i] > 0)
            unique++;
    
    free(clients);

    return unique;
}

int main_loop(State * state) {
    printf("Pozadavky:\n");

    while (1) {
        char operation = 0;
        int arg1 = -1, arg2 = -1;
        
        int input_result = get_input(state, &operation, &arg1, &arg2);
        
        if (input_result == 1)
            return print_incorrect_input();
        else if (input_result == 2)
            break;
        
        printf("> ");
        
        switch (operation) {
            case '+':
                print_visit(
                    add_visit(state, arg1)
                );    
                break;
            case '?':
                print_unique_visits(
                    count_unique_visits(state, arg1, arg2),
                    arg2 - (arg1 - 1)
                );    
                break;
            default:
                return 1;
        }
    }

    return 0;
}

int main (void) {
    State * state = new_state();

    int return_code = main_loop(state);

    free(state);

    return return_code;
}

