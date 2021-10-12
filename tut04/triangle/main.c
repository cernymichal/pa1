#include <stdio.h>
#include <math.h>

int main (void) {
    printf("Zadejte a, b, c (oddelene mezerami):\n");
    double a, b, c ;

    if (scanf(" %lf %lf %lf", &a, &b, &c) != 3) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    if (a <= 0 || b <= 0 || c <= 0) {
        printf("Trojuhelnik neexistuje.\n");
        return 1;
    }

    double circumference = a + b + c;

    double s = circumference * .5;
    double area = sqrt(s * (s - a) * (s - b) * (s - c));

    printf("obvod: %lf\nobsah: %lf\n", circumference, area);

    return 0;
}

