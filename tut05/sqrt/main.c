#include <stdio.h>
#include <math.h>

int print_result(double x, double y, int i) {
    char c = i ? 'i' : '\0';
    x *= i ? -1 : 1;
    printf("%.16g%c ^ 2 = %g\n", y, c, x);
    return 0;
}

int dbl_eq(double a, double b) {
    return fabs(a - b) < __DBL_EPSILON__ * 1e4 * fabs(a + b);
}

int main (void) {
    double x;
    double y = 42;
    int i = 0;

    printf("Zadejte x:\n");

    if (scanf(" %lf", &x) != 1) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    if (dbl_eq(x, .0))
        return print_result(x, .0, i);

    if (x < 0) {
        i = 1;
        x *= -1;
    }

    while (!dbl_eq(y * y, x)) 
        y = (x / y + y) * .5;

    return print_result(x, y, i);
}

