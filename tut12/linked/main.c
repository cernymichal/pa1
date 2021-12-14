#include <stdio.h>
#include <stdlib.h>

typedef struct LLNode_s {
    int value;
    struct LLNode_s * next;
    struct LLNode_s * prev;
} LLNode;

typedef struct LL_s {
    LLNode * first;
    LLNode * last;
} LL;

void ll_init(LL * ll) {
    ll->first = NULL;
    ll->last = NULL;
}

void ll_push(LL * ll, int value) {
    LLNode *node = (LLNode *) malloc(sizeof(LLNode));
    node->value = value;
    node->next = NULL;
    node->prev = ll->last;

    if (ll->last)
        ll->last->next = node;

    ll->last = node;

    if (!ll->first)
        ll->first = node;
}

void ll_print(LL * ll) {
    for (LLNode * node = ll->first; node; node = node->next )
        printf("-> %d ", node->value);

    printf("\n");
}

int ll_pop(LL * ll, int * value) {
    if (!ll->last)
        return 0;

    *value = ll->last->value;

    ll->last = ll->last->prev;

    if (ll->last->next) {
        free(ll->last->next);
        ll->last->next = NULL;
    }

    return 1;
}

int main(void) {
    LL list;
    ll_init(&list);

    ll_push(&list, 1);
    ll_push(&list, 2);
    ll_push(&list, 3);
    ll_push(&list, 4);

    ll_print(&list);

    int popped;
    ll_pop(&list, &popped);

    ll_push(&list, 5);

    ll_print(&list);

    ll_push(&list, popped);
    ll_print(&list);

    return 0;
}
