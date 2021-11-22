#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>

#define NAME_LENGTH 100
#define PRODUCT_INDEX_SIZE (UCHAR_MAX + 1)

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

typedef struct ProductIndexNode_s {
    Product * product;
    struct ProductIndexNode_s * next;
} ProductIndexNode;

typedef struct ProductIndexList_s {
    ProductIndexNode * first;
    ProductIndexNode * last;
} ProductIndexList;

typedef struct State_s {
    ProductList * product_list;
    ProductIndexList * product_index[PRODUCT_INDEX_SIZE]; // Products indexed by first letter
} State;

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
    product->name = name;
    product->count = 1;
    product->next = NULL;
    product->prev = NULL;

    return product;
}

// allocate new ProductIndexList
ProductIndexList * new_product_index(void) {
    ProductIndexList * index = (ProductIndexList *) calloc(1, sizeof(*index));
    return index;
}

// free ProductIndexList and its elements
void free_product_index(ProductIndexList * product_index) {
    for (ProductIndexNode * node = product_index->first; node; ) {
        ProductIndexNode * next_node = node->next;
        free(node);
        node = next_node;
    }

    free(product_index);
}

// allocate new ProductIndexNode
ProductIndexNode * new_product_index_node(Product * product) {
    ProductIndexNode * node = (ProductIndexNode *) malloc(sizeof(*node));
    node->product = product;
    node->next = NULL;

    return node;
}

// allocate new State
State * new_state(void) {
    State * state = (State *) malloc(sizeof(*state));
    state->product_list = new_product_list();
    for (int i = 0; i < PRODUCT_INDEX_SIZE; i++)
        state->product_index[i] = new_product_index();
    
    return state;
}

// free State nad its memebers
void free_state(State * state) {
    free_product_list(state->product_list);
    for (int i = 0; i < PRODUCT_INDEX_SIZE; i++)
        free_product_index(state->product_index[i]);

    free(state);
}

// add a new product at the end of list
void product_list_add(State * state, char * name) {
    Product * product = new_product(name);

    if (!state->product_list->first)
        state->product_list->first = product;
    else {
        state->product_list->last->next = product;
        product->prev = state->product_list->last;
    }

    state->product_list->last = product;

    // also add a product reference to index by the first char of its name
    ProductIndexNode * index_node = new_product_index_node(product);
    ProductIndexList * index = state->product_index[name[0] - CHAR_MIN];
    
    if (!index->first)
        index->first = index_node;
    else
        index->last->next = index_node;

    index->last = index_node;
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
int product_increment(State * state, char * name) {
    // try to find the product in index
    ProductIndexNode * index_node = state->product_index[name[0] - CHAR_MIN]->first;
    while (index_node) {
        if (!strcmp(index_node->product->name, name))
            break;
        else
            index_node = index_node->next;
    }

    if (!index_node) {
        // such product doesn't exist yet
        product_list_add(state, name);
        return 1;
    }

    Product * product = index_node->product;

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

    product_list_remove(state->product_list, product);

    if (!prev)
        prev = state->product_list->first;
    else
        prev = prev->next;
    
    product_list_insert_before(state->product_list, product, prev);

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

// main program loop, returns program exit code
int main_loop(State * state) {
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
                free_name = !product_increment(state, name);
                break;
            case '#':
                print_top_products(state->product_list, 1, top_count); // top products sum with leaderboard
                break;
            case '?':
                print_top_products(state->product_list, 0, top_count); // just sum of top products
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
    State * state = new_state();
    int exit_code = main_loop(state);

    free_state(state);

    return exit_code;
}

