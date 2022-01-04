#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_str(char * str) {
    for (int i = 0; str[i] != 0; i++)
        printf("%c", str[i]);

    printf("\n");
}

void print_str_rev(char * str) {
    for (int i = strlen(str); i >= 0; i--)
        printf("%c", str[i]);

    printf("\n");
}

int str_is_rev(char * a, char * b) {
    int len_a = strlen(a);
    int len_b = strlen(b);

    if (len_a != len_b)
        return 0;

    for (int i = 0; i < len_a; i++)
        if (a[i] != b[len_b - 1 - i])
            return 0;

    return 1;
}

int str_is_palindrome(char * str) {
    int len_str = strlen(str);

    char * no_spaces = (char *) malloc((len_str + 1) * sizeof(char));
    int len_ns = 0;
    for (int i = 0; i < len_str; i++) {
        if (str[i] != ' ') {
            no_spaces[len_ns] = str[i];
            i++;
        }
    }

    no_spaces[len_ns] = 0;

    int r = 1;
    for (int i = 0; i < len_ns / 2; i++)
        if (no_spaces[i] != no_spaces[len_ns - 1 - i])
            r = 0;

    free(no_spaces);

    return r;
}

int main (void) {
    char str[] = "jelenovi pivo nelej";

    if (str_is_palindrome(str))
        printf("\"%s\" is a palindrome. \n", str); 

    return 0;
}

