#include <stdio.h>

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

long long powll(long long a, int n) {
    long long result = 1;

    for (int i = 1; i <= n; i *= 2) {
        if (n & i)
            result *= a;
        a *= a;
    }

    return result;
}

int print_incorrect_input(void) {
    printf("Nespravny vstup.\n");
    return 1;
}

void print_result(long long result, int base, int digit) {
    int result_length = 32 - (base - 2);
    int skip = 1;
    for (int i = 0; i < result_length; i++) {
        long long p = powll(base, result_length - i - 1);
        int number = result / p;
        result -= number * p;

        if (skip) {
            if (!number && i < result_length - 1)
                continue;
            else
                skip = 0;
        }

        char c = number + 48;
        c += c > 57 ? 39 : 0;
        printf("%c", c);
    }

    printf("\n");

    for (int i = 0; i < digit; i++)
        printf(" ");

    printf("^\n");
}

int input_pos(long long *pos, int *base) {
    if (scanf(" %lld %d", pos, base) != 2 || *pos < 0 || *base < 2 || *base > 36)
        return 1;

    return 0;
}

void solve(long long pos, int base, long long *result, int *digit) {
    *result = 0;
    
    pos++;

    int i;
    long long boundry = 1;
    for (i = 1; ; i++) {
        long long new_boundry = (powll(base, i) - powll(base, i -1)) * i + boundry;
        if (pos <= new_boundry) {
            i--;
            break;
        }
        boundry = new_boundry;
    }
    
    if (i > 0) {
        *result = powll(base, i);
        pos -= boundry;
    }

    i++;
    pos--;
    *result += pos / i;
    *digit = pos % i;
}

int main (void) {
    printf("Pozice a soustava:\n");

    while (1) {
        long long pos;
        int base;
        
        int input_err = input_pos(&pos, &base);
        
        if (clear_stdin())
            break;

        if (input_err)
            return print_incorrect_input();

        int digit;
        long long result;

        solve(pos, base, &result, &digit);

        print_result(result, base, digit);
    }

    return 0;
}

