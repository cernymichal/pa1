#include <stdio.h>

void between(int a, int b) {
    for (int i = a + 1; i < b; i++)
        printf("%d ", i);

    printf("\n");
}

void between_r(int a, int b) {
    for (int i = b - 1; i > a; i--)
        printf("%d ", i);

    printf("\n");
}

void odds(int a, int b) {
    for (int i = a + ((a + 1) % 2); i <= b; i += 2)
        printf("%d ", i);

    printf("\n");
}

void divisible(int a, int b, int k) {
    for (int i = a + (a % k); i <= b; i += k)
        printf("%d ", i);

    printf("\n");
}

int main (void) {
    divisible (10, 32, 4);
    return 0;
}

