#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define NAME_LENGTH 100

typedef struct Product_s {
    char * name;
    long long count;
    struct Product_s * next;
    struct Product_s * prev;
} Product;

typedef struct ProductList_s {
    Product * first;
    Product * last;
} ProductList;

// allocate new ProductList
ProductList * new_product_list(void) {
    ProductList * list = (ProductList *) calloc(1, sizeof(*list));
    return list;
}

// free ProductList and its elements
void free_product_list(ProductList * list) {
    for (Product * product = list->first; product; ) {
        Product * next_product = product->next;
        free(product->name);
        free(product);
        product = next_product;
    }

    free(list);
}

// allocate new Product
Product * new_product(char * name) {
    Product * product = (Product *) malloc(sizeof(*product));
    //strcpy(product->name, name);
    product->name = name;
    product->count = 1;
    product->next = NULL;
    product->prev = NULL;

    return product;
}

// add a new product at the end of list
void product_list_add(ProductList * list, char * name) {
    Product * product = new_product(name);

    if (!list->first)
        list->first = product;
    else {
        list->last->next = product;
        product->prev = list->last;
    }

    list->last = product;
}

// remove product from list
void product_list_remove(ProductList * list, Product * product) {
    if (product->next)
        product->next->prev = product->prev;
    else
        list->last = product->prev;

    if (product->prev)
        product->prev->next = product->next;
    else
        list->first = product->next;
}

// add product in front of before_product in list
void product_list_insert_before(ProductList * list, Product * product, Product * before_product) {
    product->next = product->prev = NULL;
    
    if (before_product->prev) {
        product->prev = before_product->prev;
        product->prev->next = product;
    }
    else
        list->first = product;

    before_product->prev = product;
    product->next = before_product;
}

// increment the count of product in list if it exists or add a new one
// returns 1 if a new product was created
int product_increment(ProductList * list, char * name) {
    // try to find the product
    Product * product = list->first;
    while (product) {
        if (!strcmp(product->name, name))
            break;
        else
            product = product->next;
    }

    if (!product) {
        product_list_add(list, name);
        return 1;
    }

    product->count++;
    
    // move product to keep the list sorted
    Product * prev = product->prev;
    while (prev) {
        if (prev->count < product->count)
            prev = prev->prev;
        else
            break;
    }

    if (prev == product->prev)
        return 0;

    product_list_remove(list, product);

    if (!prev)
        prev = list->first;
    else
        prev = prev->next;
    
    product_list_insert_before(list, product, prev);

    return 0;
}

int print_incorrect_input() {
    printf("Nespravny vstup.\n");
    return 1;
}

void print_top_products(ProductList * list, int leaderboard, int top_count) {
    long long sum = 0;
    int counted = 0;
    
    for (Product * product = list->first; product && counted < top_count; ) {
        int current_count = product->count;
        int starting_count = counted;
        int places = 0;

        for (Product * p = product; p && p->count == current_count; p = p->next)
            places++;

        sum += places * current_count;
        counted += places;

        for (; product && product->count == current_count; product = product->next) {
            if (leaderboard) {
                if (places > 1)
                    printf("%d.-%d. ", starting_count + 1, places + starting_count);
                else
                    printf("%d. ", counted);

                printf("%s, %lldx\n", product->name, product->count);
            }
        }
    }

    printf("Nejprodavanejsi zbozi: prodano %lld kusu\n", sum);
}

// clears stdin to the next newline or EOF
// return 2 for EOF
//        1 for excess characters
int clear_stdin(void) {
    int excess = 0;
    while (true) {
        char c = getc(stdin);
        if (c == EOF)
            return 2;
        else if (c == '\n')
            break;
        else if (!isspace(c))
            excess = 1;
    }

    return excess;
}

// query user for leaderboard count
int get_top_count(int * top_count) {
    if (scanf(" %d", top_count) != 1 || *top_count < 1)
        return 1;

    return clear_stdin();
}

// query user for command
int get_input(char * operation, char ** name) {
    int scanned = scanf(" %1c", operation);

    if (scanned == EOF)
        return 2;

    if (scanned < 1 || !(*operation == '+' || *operation == '#' || *operation == '?'))
        return 1;

    if (*operation == '+') {
        *name = (char *) malloc(sizeof(char) * NAME_LENGTH);
        if (scanf(" %99s", *name) != 1)
            return 1;
    }

    return clear_stdin();
}

#ifndef __PROGTEST__

void test_product_list(void) {
    char * name1 = (char *) malloc(sizeof(char) * NAME_LENGTH);
    char * name2 = (char *) malloc(sizeof(char) * NAME_LENGTH);
    char * name3 = (char *) malloc(sizeof(char) * NAME_LENGTH);

    strcpy(name1, "Mleko");
    strcpy(name2, "Pecivo");
    strcpy(name3, "Maso");

    ProductList * product_list = new_product_list();

    product_increment(product_list, name1);
    product_increment(product_list, name1);
    product_increment(product_list, name3);
    product_increment(product_list, name1);
    product_increment(product_list, name3);
    product_increment(product_list, name2);
    product_increment(product_list, name3);
    product_increment(product_list, name3);

    Product * p = product_list->first;
    assert(!strcmp(p->name, name3));
    assert(p->count == 4);
    assert(!p->prev);
    assert(!strcmp(p->next->name, name1));
    
    p = p->next;
    assert(!strcmp(p->name, name1));
    assert(p->count == 3);
    assert(!strcmp(p->prev->name, name3));
    assert(!strcmp(p->next->name, name2));

    p = p->next;
    assert(!strcmp(p->name, name2));
    assert(p->count == 1);
    assert(!strcmp(p->prev->name, name1));
    assert(!p->next);
    assert(p == product_list->last);

    free_product_list(product_list);
}

#endif

// main program loop, returns program exit code
int main_loop(ProductList * product_list) {
    int top_count;
    printf("Pocet sledovanych:\n");
    if (get_top_count(&top_count))
        return print_incorrect_input();

    printf("Pozadavky:\n");
    while (1) {
        char operation;
        char * name = NULL;

        int input_result = get_input(&operation, &name);

        if (input_result == 1) {
            free(name);
            return print_incorrect_input();
        }
        else if (input_result == 2) { // EOF
            free(name);
            break;
        }
    
        // free name if it wasn't linked to a product
        int free_name = 1;

        switch (operation) {
            case '+':
                free_name = !product_increment(product_list, name);
                break;
            case '#':
                print_top_products(product_list, 1, top_count); // top products sum with leaderboard
                break;
            case '?':
                print_top_products(product_list, 0, top_count); // just sum of top products
                break;
            default:
                return 1;
        }

        if (free_name)
            free(name);
    }

    return 0;
}

int main (void) {
    #ifndef __PROGTEST__
        test_product_list();
    #endif

    ProductList * product_list = new_product_list();
    int exit_code = main_loop(product_list);

    free_product_list(product_list);

    return exit_code;
}

