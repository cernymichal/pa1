#include <stdio.h>

#define INPUT_LINE_LENGTH 256
#define MAX_VISITS 1000000

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

int get_input(char * op, int * arg1, int * arg2, int visits) {
    *op = 0;
    *arg1 = *arg2 = -1;
    
    char line[INPUT_LINE_LENGTH];
    fgets(line, INPUT_LINE_LENGTH, stdin);
    int scanned = sscanf(line, " %c %d %d", op, arg1, arg2);

    if (scanned == EOF)
        return 2;

    if (scanned < 1 || !(*op == '+' || *op == '?'))
        return 1;

    if (*op == '+' && (scanned != 2 || *arg1 < 0 || *arg1 >= MAX_VISITS || visits >= MAX_VISITS))
        return 1;

    if (*op == '?' && (scanned != 3 || *arg1 < 0 || *arg2 >= visits || *arg1 > *arg2))
        return 1;

    return 0;
}

int add_visit(int * visits, int log[], int id) {
    log[*visits] = id;
    *visits += 1;

    int count = 0;
    for (int i = 0; i < *visits; i++) {
        if (log[i] == id)
            count++;
    }

    return count;
}

int count_unique_visits(int visits, int log[], int start, int end) {
    return end - (start - 1);
}

int main (void) {
    int visits = 0;
    int log[MAX_VISITS];

    printf("Pozadavky:\n");
    
    while (1) {
        char operation;
        int arg1, arg2;
        
        int input_result = get_input(&operation, &arg1, &arg2, visits);

        if (input_result == 1)
            return print_incorrect_input();
        else if (input_result == 2)
            break;

        printf("> ");
        
        switch (operation) {
            case '+':
                print_visit(
                    add_visit(&visits, log, arg1)
                );
                break;
            case '?':
                print_unique_visits(
                    count_unique_visits(visits, log, arg1, arg2),
                    arg2 - (arg1 - 1)
                );
                break;
            default:
                return 1;
        }
    }

    return 0;
}

