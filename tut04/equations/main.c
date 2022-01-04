#include <stdio.h>
#include <math.h>

int print_invalid_input() {
    printf("Nespravny vstup.\n");
    return 1;
}

int print_equal_result() {
    printf("Rovnice je spravne.\n");
    return 0;
}

int print_unequal_result(double result, double c) {
    printf("%g != %g", result, c);
    return 0;
}

double sum(double a, double b) {
    return a + b;
}

double sub(double a, double b) {
    return a - b;
}

double mul(double a, double b) {
    return a * b;
}

double div(double a, double b) {
    double result = a / b;
    return result < 0 ? ceil(result) : floor(result);
}

int main (void) {
    printf("Zadejte rovnici:\n");

    double a, b, c;
    char op[2];

    if (scanf(" %lf %1[+-*/] %lf = %lf", &a, op, &b, &c) != 4 || (op[0] == '/' && b == 0))
        return print_invalid_input();
    
    double result;
    switch(op[0]) {
        case '+': result = sum(a, b); break;
        case '-': result = sub(a, b); break;
        case '*': result = mul(a, b); break;
        case '/': result = div(a, b); break;
        default: return print_invalid_input(); break;
    }
    
    if (fabs(result - c) > __DBL_EPSILON__ * 1e4 * fabs(result + c))
        return print_unequal_result(result, c);

    return print_equal_result();
}

