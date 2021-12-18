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

Person * person_new(int id, char * name, Person * ancestor1, Person * ancestor2) {
    Person * person = (Person *) malloc(sizeof(*person));

    person->id = id;
    person->name = (char *) malloc((strlen(name) + 1) * sizeof(char));
    strcpy(person->name, name);
    person->ancestor1 = ancestor1;
    person->ancestor2 = ancestor2;

    return person;
}

void person_free(Person * person) {
    free(person->name);
    free(person);
}

int person_compare(Person * l, Person * r)
{
    if (l->id < r->id)
        return -1;
    else if (l->id > r->id)
        return 1;

    return 0;
}

// ============================== VectorPerson

typedef struct VectorPerson_s {
    Person ** data;
    int capacity;
    int n;
} VectorPerson;

VectorPerson * vector_person_new(void) {
    VectorPerson * vector = (VectorPerson *) malloc(sizeof(VectorPerson));

    vector->capacity = VECTOR_INITIAL_SIZE;
    vector->n = 0;
    vector->data = (Person **) malloc(vector->capacity * sizeof(Person *));

    return vector;
}

void vector_person_free(VectorPerson * vector) {
    if (!vector)
        return;

    for (int i = 0; i < vector->n; i++)
        person_free(vector->data[i]);

    free(vector->data);
    free(vector);
}

void vector_person_push(VectorPerson * vector, Person * value) {
    if (vector->n + 1 > vector->capacity) {
        vector->capacity *= VECTOR_SIZE_FACTOR;
        vector->data = (Person **) realloc(vector->data, vector->capacity * sizeof(Person *));
    }

    // TODO speed up with binary search
    // insert into vector but keep it sorted
    int i = 0;
    for (; i < vector->n && vector->data[i]->id <= value->id; i++);

    Person * old = vector->data[i];
    vector->data[i] = value;

    for (i++; i < vector->n + 1; i++) {
        Person * temp = vector->data[i];
        vector->data[i] = old;
        old = temp;
    }

    vector->n++;
}

Person * vector_person_bsearch(VectorPerson * vector, int id) {
    Person key;
    key.id = id;

    return (Person *) bsearch(
        &key,
        vector->data,
        vector->n,
        sizeof(Person *),
        (int (*)(void const *, void const *)) person_compare
    );
}

// /============================= VectorPerson

typedef struct TDatabase {
    VectorPerson * people;
} TDATABASE;

#ifndef __PROGTEST__

typedef struct TResult {
    struct TResult * m_Next;
    int m_ID;
    char * m_Name;
} TRESULT;

#endif /* __PROGTEST__ */

void initAll(TDATABASE * db) {
    db->people = vector_person_new();
}

void doneAll(TDATABASE * db) {
    vector_person_free(db->people);
}

int addPerson(TDATABASE * db, int id, const char * name, int id1, int id2) {
    /* TODO */
    return 0;
}

TRESULT * ancestors(TDATABASE * db, int id) {
    /* TODO */
    return NULL;
}

TRESULT * commonAncestors(TDATABASE * db, int id1, int id2) {
    /* TODO */
    return NULL;
}

void freeResult(TRESULT * res) {
    /* TODO */
}

#ifndef __PROGTEST__

#include "./asserts.c"

#endif /* __PROGTEST__ */

