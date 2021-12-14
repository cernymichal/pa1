#ifndef __PROGTEST__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#endif /* __PROGTEST__ */

#define VECTOR_INITIAL_SIZE 10
#define VECTOR_SIZE_FACTOR 2

typedef struct Person_s {
    int id;
    char * name;
    struct Person_s * ancestor1;
    struct Person_s * ancestor2;
} Person;

// ============================== VectorPerson

typedef struct VectorPerson_s {
    Person * data;
    int capacity;
    int n;
} VectorPerson;

VectorPerson * vector_person_new() {
    VectorPerson * vector = (VectorPerson *) malloc(sizeof(VectorPerson));

    vector->capacity = VECTOR_INITIAL_SIZE;
    vector->n = 0;
    vector->data = (Person *) malloc(vector->capacity * sizeof(Person));

    return vector;
}

void vector_person_free(VectorPerson * vector) {
    if (!vector)
        return;

    free(vector->data);
    free(vector);
}

void vector_person_push(VectorPerson * vector, char value) {
    if (vector->n + 1 > vector->capacity) {
        vector->capacity *= VECTOR_SIZE_FACTOR;
        vector->data = (Person *) realloc(vector->data, vector->capacity * sizeof(Person));
    }

    vector->data[vector->n++] = value;
}

// /============================= VectorPerson

typedef struct TDatabase {
    VectorPerson * people
} TDATABASE;

#ifndef __PROGTEST__

typedef struct TResult {
    struct TResult *m_Next;
    int m_ID;
    char *m_Name;
} TRESULT;

#endif /* __PROGTEST__ */

void initAll(TDATABASE *db) {
    /* todo */
}

void doneAll(TDATABASE *db) {
    /* todo */
}

int addPerson(TDATABASE *db, int id, const char *name, int id1, int id2) {
    /* todo */
}

TRESULT *ancestors(TDATABASE *db, int id) {
    /* todo */
}

TRESULT *commonAncestors(TDATABASE *db, int id1, int id2) {
    /* todo */
}

void freeResult(TRESULT *res) {
    /* todo */
}

#ifndef __PROGTEST__

#include "./asserts.c"

#endif /* __PROGTEST__ */

