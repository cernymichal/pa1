#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NAME_LENGTH 100

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

    Product * p = product_list->first;
    assert(!strcmp(p->name, name1) && p->count == 3);
    
    p = p->next;
    assert(!strcmp(p->name, name3) && p->count == 2);

    p = p->next;
    assert(!strcmp(p->name, name2) && p->count == 1);

    free_product_list(product_list);
}

int main (void) {
    asserts();
    return 0;
}

