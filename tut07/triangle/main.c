#include <stdio.h>
#include <math.h>

int incorrect_input() {
    printf("Nespravny vstup.\n");
    return 1;
}

int get_input(double * x, double * y) {
    if (scanf(" %lf %lf", x, y) != 2)
        return 0;

    return 1;
}

int on_side(double ax, double ay, double bx, double by, double cx, double cy, double xx, double xy) {
    double ux = bx - ax;
    double uy = by - ay;
    
    // TODO

    return 1;
}

int main (void) {
    printf("Zadejte body A, B, C ve formatu \"[x] [y]\":\n");

    double ax, ay;
    double bx, by;
    double cx, cy;

    if (!get_input(&ax, &ay) || !get_input(&bx, &by) || !get_input(&cx, &cy))
        return incorrect_input();

    printf("Zadejte bod X:\n");

    double xx, xy;

    if (!get_input(&xx, &xy))
        return incorrect_input();

    if (on_side(ax, ay, bx, by, cx, cy, xx, xy) && on_side(bx, by, cx, cy, ax, xy, xx, xy) && on_side(cx, cy, ax, ay, bx, by, xx, xy))
        printf("Bod X lezi v trojuhelniku.\n");
    else
        printf("Bod X nelezi v trojuhelniku.\n");

    return 0;
}

