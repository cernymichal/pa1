#include <stdio.h>

double print_dbl(double num) {
    printf("%lf\n", num);
    return num;
}

int main (void) {
    double a = 1.2;
    
    a += print_dbl(a);
   
    print_dbl(a);
    
    return 0;
}

