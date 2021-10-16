#include <stdio.h>

int main (void) {
    printf("Zadejte rovnici:\n");

    double a, b, c;
    char op[2];

    if (scanf(" %lf %1[+-*/] %lf = %lf", &a, op, &b, &c) != 4 || (op[0] == '/' && b == 0)) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    return 0;
}

