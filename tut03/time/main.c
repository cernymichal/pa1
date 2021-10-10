#include <stdio.h>

int H_MS = 60 * 60 * 1000;
int M_MS = 60 * 1000;
int S_MS = 1000;

int fill_with_zeroes(char *string, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (string[i] == 0) {
            string[i] = '0';
        }
    }
    string[n - 1] = 0;
    return 0;
}

int input_time(int *t) {
    int h, m, s, ms;
    char ms_str[] = "000";

    if (scanf("%d : %d : %d , %3[0-9]", &h, &m, &s, ms_str) < 4 || fill_with_zeroes(ms_str, 4) || sscanf(ms_str, "%d", &ms) < 1 || h < 0 || h > 23 || m < 0 || m > 59 || s < 0 || s > 59 || ms < 0 || ms > 999) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    *t = h * H_MS + m * M_MS + s * S_MS + ms;
    
    return 0;
}

int main (void) {
    int t1, t2; //ms

    printf("Zadejte cas t1:\n");
    if (input_time(&t1) != 0) {
        return 1;
    }

    printf("Zadejte cas t2:\n");
    if (input_time(&t2) != 0) {
        return 1;
    }

    int diff = t2 - t1;

    if (diff < 0) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    int h = diff / H_MS;
    diff -= h * H_MS;
    
    int m = diff / M_MS;
    diff -= m * M_MS;
    
    int s = diff / S_MS;
    diff -= s * S_MS;

    int ms = diff;

    printf("Doba: %2d:%02d:%02d,%03d\n", h, m, s, ms);

    return 0;
}

