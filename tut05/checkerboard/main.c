#include <stdio.h>

#define CHAR_WHITE ' '
#define CHAR_BLACK 'X'

int print_invalid_input() {
    printf("Nespravny vstup.\n");
    return 1;
}

void clear_stdin() {
    while (true) {
        char c = getc(stdin);
        if (c == '\n' || c == EOF)
            break;
    }
}

int get_input(int *board_size, int *cell_size) {
    printf("Zadejte pocet poli:\n");

    if (scanf(" %d", board_size) != 1 || *board_size <= 0)
        return 1;

    clear_stdin();

    printf("Zadejte velikost pole:\n");

    if (scanf(" %d", cell_size) != 1 || *cell_size <= 0)
	    return 1;

    clear_stdin();   

    return 0;
}

void draw_border(int width) {
    printf("+");

    for (int i = 0; i < width; i++)
        printf("-");

    printf("+\n");
}

void draw_cell(int width, int color) {
    char c = color ? CHAR_BLACK : CHAR_WHITE;
    for (int i = 0; i < width; i++) {
        printf("%c", c);
    }
}

int main (void) {
    int board_size, cell_size;

    if (get_input(&board_size, &cell_size))
        return print_invalid_input();

    draw_border(board_size * cell_size);

    int color = 0;
    for (int y = 0; y < board_size; y++) {
        for (int yy = 0; yy < cell_size; yy++) {
            int cell_color = color;
            printf("|");
            for (int x = 0; x < board_size; x++) {
                draw_cell(cell_size, cell_color);
                cell_color = !cell_color;
            }
            printf("|\n");
        }
        color = !color;
    }

    draw_border(board_size * cell_size);

    return 0;
}

