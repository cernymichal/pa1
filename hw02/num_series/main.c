#include <stdio.h>
#include <math.h>

#define RESULT_LENGTH 128

int clear_stdin(void) {
    while (true) {
        char c = getc(stdin);
        if (c == EOF)
            return 1;
        else if (c == '\n')
            break;
    }

    return 0;
}

int print_incorrect_input(void) {
    printf("Nespravny vstup.\n");
    return 1;
}

void print_result(int result[], int digit) {
    for (int i = 0; i < RESULT_LENGTH; i++) {
        if (result[i] == -1)
            break;

        char c = result[i] + 48;
        c += c > 57 ? 39 : 0;
        printf("%c", c);
    }

    printf("\n");

    for (int i = 1; i < digit; i++)
        printf(" ");

    printf("^\n");
}

int input_pos(long long *pos, int *sys) {
    if (scanf(" %lld %d", pos, sys) != 2 || *pos < 0 || *sys < 2 || *sys > 36)
        return 1;

    return 0;
}

void solve(long long pos, int sys, int result[], int *digit) {
    *digit = 1;
    result[0] = 1;
    result[1] = -1;
}

int main (void) {
    printf("Pozice a soustava:\n");

    while (1) {
        long long pos;
        int sys;

        if (input_pos(&pos, &sys))
            return print_incorrect_input();

        if (clear_stdin())
            break;

        int digit;
        int result[RESULT_LENGTH];

        solve(pos, sys, result, &digit);

        print_result(result, digit);
    }

    return 0;
}

