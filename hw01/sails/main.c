#include <stdio.h>
#include <math.h>
#include <limits.h>

void clear_stdin() {
    while (true) {
        char c = getc(stdin);
        if (c == '\n' || c == EOF)
            break;
    }
}

int print_incorrect_input() {
    printf("Nespravny vstup.\n");
    return 1;
}

int print_impossible() {
    printf("Nelze vyrobit.\n");
    return 1;
}

int print_result(int pieces) {
    printf("Pocet kusu latky: %d\n", pieces);
    return 0;
}

int input_sizes(double *fabric_w, double *fabric_h, double *sail_w, double *sail_h) {
    printf("Velikost latky:\n");

    if (scanf(" %lf %lf", fabric_w, fabric_h) != 2 || *fabric_w <= .0 || *fabric_h <= .0)
        return 1;

    clear_stdin();

    printf("Velikost plachty:\n");
    
    if (scanf(" %lf %lf", sail_w, sail_h) != 2 || *sail_w <= .0 || *sail_h <= .0)
        return 1;

    clear_stdin();

    return 0;
}

int input_overlap(double *overlap) {
    printf("Prekryv:\n");

    if (scanf(" %lf", overlap) != 1 || *overlap < .0)
        return 1;

    clear_stdin();

    return 0;
}

int solve(double fabric_w, double fabric_h, double sail_w, double sail_h, double overlap, int *result) {
    int x = (int) ceil((sail_w - overlap) / (fabric_w - overlap));
    int y = (int) ceil((sail_h - overlap) / (fabric_h - overlap));

    if (x < 0 || y < 0)
        return 1;

    *result = x * y;
    return 0;
}

int solve_rotate(double fabric_w, double fabric_h, double sail_w, double sail_h, double overlap, int *result) {
    int a, b = INT_MAX;
    
    int err_a = solve(fabric_w, fabric_h, sail_w, sail_h, overlap, &a);
    int err_b = solve(fabric_h, fabric_w, sail_w, sail_h, overlap, &b);

    if (err_a && err_b)
        return 1;

    *result = a < b ? a : b;
    return 0;
}

int main (void) {
    double fabric_w, fabric_h, sail_w, sail_h, overlap;

    if (input_sizes(&fabric_w, &fabric_h, &sail_w, &sail_h))
        return print_incorrect_input();

    int result;
    if (solve_rotate(fabric_w, fabric_h, sail_w, sail_h, .0, &result))
        return print_impossible();
    
    if (result == 1)
        return print_result(result);

    if (input_overlap(&overlap))
        return print_incorrect_input();

    if (solve_rotate(fabric_w, fabric_h, sail_w, sail_h, overlap, &result))
        return print_impossible();

    return print_result(result);
}

