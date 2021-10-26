#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

#define MAXS 10.
#define MINS -10.
#define MIN_DISTANCE .05

double randi(double min, double max) {
    return min + rand() / ((RAND_MAX + 1u) / fabs(min - max));
}

double pythagoras(double a, double b) {
    return sqrt(a * a + b * b);
}

int main (void) {
    srand(time(NULL));
    double x = randi(MINS, MAXS);
    double y = randi(MINS, MAXS);
    
    printf("Prosim hadej:\n");
    double distance = DBL_MAX;
    int i = 1;
    for ( ; ; i++) {
        double g_x, g_y;

        if (scanf(" %lf %lf", &g_x, &g_y) != 2) {
            printf("Nespravny vstup.\n");
            continue;
        }

        double new_distance = pythagoras(x - g_x, y - g_y);

        if (new_distance <= MIN_DISTANCE) {
            break;
        }
        
        if (new_distance < distance) {
            printf("Prihoriva.\n");
            distance = new_distance;
        }
        else
            printf("Sama voda.\n");

    }

    printf("Hori!\nPocet pokusu: %d", i);

    return 0;
}

