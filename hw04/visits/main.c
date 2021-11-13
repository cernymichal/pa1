#include <stdio.h>
#include <stdlib.h>

#define INPUT_LINE_LENGTH 256

#define MAX_VISITS 1000000
#define MAX_ID 99999
#define ID_COUNT (MAX_ID + 1)

typedef struct Visit_s {
    int client_id;
    int last_client_visit;
} Visit;

typedef struct Client_s {
    int visits;
    int last_visit;
} Client;

typedef struct State_s {
    int visits_total;
    Visit visits[MAX_VISITS];
    Client clients[ID_COUNT];
} State;

State * new_state(void) {
    State * state = (State *) malloc(sizeof(State));
    state->visits_total = 0;

    for (int i = 0; i < ID_COUNT; i++) {
        state->clients[i].visits = 0;
        state->clients[i].last_visit = -1;
    }

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
    Visit * visit = &state->visits[state->visits_total];
    Client * client = &state->clients[id];
    
    visit->client_id = id;
    visit->last_client_visit = client->last_visit;
    
    client->visits++;
    client->last_visit = state->visits_total;
    
    state->visits_total++;

    return client->visits;
}

int count_unique_visits(State * state, int start, int end) {
    int unique = 0;
    for (int i = start; i <= end; i++) {
        if (state->visits[i].last_client_visit < start)
            unique++;
    }

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

int main(void) {
    State * state = new_state();

    int return_code = main_loop(state);

    free(state);

    return return_code;
}

