#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NAME_LENGTH 100
#define INPUT_LINE_LENGTH 128

typedef struct Product_s {
    char name[NAME_LENGTH];
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
        free(product);
        product = next_product;
    }

    free(list);
}

// allocate new Product
Product * new_product(char name[NAME_LENGTH]) {
    Product * product = (Product *) malloc(sizeof(*product));
    strcpy(product->name, name);
    product->count = 1;
    product->next = NULL;
    product->prev = NULL;

    return product;
}

// add a new product at the end of list
void product_list_add(ProductList * list, char name[NAME_LENGTH]) {
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
void product_increment(ProductList * list, char name[NAME_LENGTH]) {
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
        return;
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
        return;

    product_list_remove(list, product);

    if (!prev)
        prev = list->first;
    else
        prev = prev->next;
    
    product_list_insert_before(list, product, prev);   
}

int print_incorrect_input() {
    printf("Nespravny vstup.\n");
    return 1;
}

void print_top_products(ProductList * list, int leaderboard, int count) {
    for (Product * product = list->first; product; product = product->next)
        printf("%s %lld\n", product->name, product->count);
}

// query user for leaderboard count
int get_top_count(int * top_count) {
    char line[INPUT_LINE_LENGTH];
    char * fgets_ret = fgets(line, INPUT_LINE_LENGTH, stdin);

    // if EOF
    if (fgets_ret == NULL)
        return 1;

    if (sscanf(line, " %d", top_count) != 1 || *top_count < 1)
        return 1;

    return 0;
}

// query user for command
int get_input(char * operation, char name[NAME_LENGTH]) {
    char line[INPUT_LINE_LENGTH];
    char * fgets_ret = fgets(line, INPUT_LINE_LENGTH, stdin);

    // if EOF
    if (fgets_ret == NULL)
        return 2;

    int scanned = sscanf(line, " %1c %99s ", operation, name);

    if (scanned < 1 || !(*operation == '+' || *operation == '#' || *operation == '?'))
        return 1;

    if (*operation == '+' && scanned != 2)
        return 1;

    if (*operation == '#' && scanned != 1)
        return 1;

    if (*operation == '?' && scanned != 1)
        return 1;

    return 0;
}

void asserts(void) {
    char name1[NAME_LENGTH];
    strcpy(name1, "Mleko");

    char name2[NAME_LENGTH];
    strcpy(name2, "Pecivo");

    char name3[NAME_LENGTH];
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

// main program loop, returns program exit code
int main_loop(ProductList * product_list) {
    int top_count;
    printf("Pocet sledovanych:\n");
    if (get_top_count(&top_count))
        return print_incorrect_input();

    printf("Pozadavky:\n");
    while (1) {
        char operation = 0;
        char name[NAME_LENGTH];

        int input_result = get_input(&operation, name);

        if (input_result == 1)
            return print_incorrect_input();
        else if (input_result == 2) // EOF
            break;

        switch (operation) {
            case '+':
                product_increment(product_list, name);
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
    }

    return 0;
}

int main (void) {
    //asserts();

    ProductList * product_list = new_product_list();
    int exit_code = main_loop(product_list);

    free_product_list(product_list);

    return exit_code;
}

