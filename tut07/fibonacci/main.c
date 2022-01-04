#include <stdio.h>

long long fibonacci(int n) {
    long long a = 1, b = 1;
    
    for (; n > 2; n--) {
        long long sum = a + b;
        a = b;
        b = sum;
    }

    return b;
}

long long tribonacci(int n) {
    long long a = 1, b = 1, c = 1;

    for (; n > 3; n--) {
        long long sum = a + b + c;
        a = b;
        b = c;
        c = sum;
    }
    
    return c;
}

int get_input(char * o, int * n) {
    if (scanf(" %[ft] %d", o, n) != 2 || *n < 1)
        return 0;

    return 1;
}

int main (void) {
    printf("Zadejte funkci a n:\n");

    int n;
    char o;

    if (!get_input(&o, &n)) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    long long result;

    if (o == 'f')
        result = fibonacci(n);
    else
        result = tribonacci(n);

    printf("%lld\n", result);

    return 0;
}

