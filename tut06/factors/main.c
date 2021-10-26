#include <stdio.h>



int main (void) {
    printf("Zadejte a:\n");

    int a;

    if (scanf(" %d", &a) != 1 || a < 0) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    if (a == 1) {
        printf("1\n");
        return 0;
    }

    int first = 1;
    while (a > 1) {
        for (int i = 2; ; i++) {
            if (a % i == 0) {
                if (!first)
                    printf("*");
                else
                    first = 0;
                
                printf("%d", i);
                a /= i;
                break;
            }
        }
    }

    printf("\n");

    return 0;
}

