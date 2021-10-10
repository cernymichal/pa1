#include <stdio.h>

int main (void) {
    int input;

    printf("ml' nob:\n");
    if (scanf("%d", &input) != 1 || input < 1 || input > 5) {
        printf("luj\n");
        return 1;
    }

    printf("Qapla'\n");

    if (input == 1) {
        printf("noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n");
    }
    else if(input == 2) {
        printf("Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n");
    }
    else if(input == 3) {
        printf("qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n");
    }
    else if(input == 4) {
        printf("Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n");
    }
    else {
        printf("leghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n");
    }

    return 0;
}

