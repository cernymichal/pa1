#include <stdio.h>

void swap_vars(int * a, int * b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sort_vars(int * a, int * b) {
    if (*a > *b)
        swap_vars(a, b);
}

int intersection(int * as, int * ae, int bs, int be) {
    int a_start = *as;
    int a_end = *ae;
    int b_start = bs;
    int b_end = be;

    sort_vars(&a_start, &a_end);
    sort_vars(&b_start, &b_end);

    sort_vars(&b_start, &a_start);
    sort_vars(&a_end, &b_end);

    if (a_start >= a_end)
        return 0;
    
    *as = a_start;
    *ae = a_end;

    return 1;
}

int main (void) {
    int as = 10, ae = 15;
    int bs = 5, be = 12;

    if (intersection(&as, &ae, bs, be))
        printf("intersection - %d-%d\n", as, ae);
    else
        printf("no intersection.");

    return 0;
}

