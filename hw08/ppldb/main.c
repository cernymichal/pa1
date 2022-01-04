#ifndef __PROGTEST__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#endif /* __PROGTEST__ */

// ============================== Person

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

int person_compare(Person ** lhs, Person ** rhs)
{
    Person * l = *lhs;
    Person * r = *rhs;

    if (l->id < r->id)
        return -1;
    else if (l->id > r->id)
        return 1;

    return 0;
}

// /============================= Person

// ============================== VectorPerson

#define VECTOR_INITIAL_SIZE 10
#define VECTOR_SIZE_FACTOR 2

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

    Person * key_p = &key;

    Person ** result = (Person **) bsearch(
        &key_p,
        vector->data,
        vector->n,
        sizeof(*vector->data),
        (int (*)(void const *, void const *)) person_compare
    );

    return result ? *result : NULL;
}

// /============================= VectorPerson

// ============================== TResult

#ifndef __PROGTEST__

typedef struct TResult {
    struct TResult * m_Next;
    int m_ID;
    char * m_Name;
} TRESULT;

#endif /* __PROGTEST__ */

TRESULT * tresult_new(int id, char * name, TRESULT * next) {
    TRESULT * tresult = (TRESULT *) malloc(sizeof(*tresult));

    tresult->m_Next = next;
    tresult->m_ID = id;
    tresult->m_Name = name;

    return tresult;
}

void freeResult(TRESULT * res) {
    while (res) {
        TRESULT * temp = res;
        res = res->m_Next;
        free(temp);
    }
}

TRESULT * tresult_merge_unique(TRESULT * a, TRESULT * b) {
    if (!a)
        return b;

    if (!b)
        return a;

    TRESULT ** current_a = &a;
    for (; *current_a; current_a = &(*current_a)->m_Next) {
        for (TRESULT ** current_b = &b; *current_b; current_b = &(*current_b)->m_Next) {
            if ((*current_a)->m_ID != (*current_b)->m_ID)
                continue;

            TRESULT * temp = *current_b;
            *current_b = (*current_b)->m_Next;
            free(temp);
            break;
        }
    }

    *current_a = b;

    return a;
}

TRESULT * tresult_intersection(TRESULT * a, TRESULT * b) {
    TRESULT * result = NULL;
    for (TRESULT * current_a = a; current_a; current_a = current_a->m_Next) {
        for (TRESULT * current_b = b; current_b; current_b = current_b->m_Next) {
            if (current_a->m_ID != current_b->m_ID)
                continue;

            result = tresult_new(current_a->m_ID, current_a->m_Name, result);
            break;
        }
    }

    return result;
}

// /============================= TResult

// ============================== TDatabase

typedef struct TDatabase {
    VectorPerson * people;
} TDATABASE;

void initAll(TDATABASE * db) {
    db->people = vector_person_new();
}

void doneAll(TDATABASE * db) {
    vector_person_free(db->people);
}

int addPerson(TDATABASE * db, int id, const char * name, int id1, int id2) {
    if (id == 0 || vector_person_bsearch(db->people, id))
        return 0;

    if (id1 == id2 && id1 != 0)
        return 0;

    Person * ancestor1 = id1 != 0 ? vector_person_bsearch(db->people, id1) : NULL;

    if (id1 != 0 && !ancestor1)
        return 0;

    Person * ancestor2 = id2 != 0 ? vector_person_bsearch(db->people, id2) : NULL;

    if (id2 != 0 && !ancestor2)
        return 0;

    Person * person = person_new(id, (char *) name, ancestor1, ancestor2);
    vector_person_push(db->people, person);

    return 1;
}

TRESULT * ancestors_rec(TDATABASE * db, Person * person) {
    if (!person)
        return NULL;

    TRESULT * result = NULL;

    if (person->ancestor1) {
        result = tresult_merge_unique(
            result,
            tresult_new(person->ancestor1->id, person->ancestor1->name, NULL)
        );
        result = tresult_merge_unique(
            result,
            ancestors_rec(db, person->ancestor1)
        );
    }

    if (person->ancestor2) {
        result = tresult_merge_unique(
            result,
            tresult_new(person->ancestor2->id, person->ancestor2->name, NULL)
        );
        result = tresult_merge_unique(
            result,
            ancestors_rec(db, person->ancestor2)
        );
    }

    return result;
}

TRESULT * ancestors(TDATABASE * db, int id) {
    Person * person = vector_person_bsearch(db->people, id);
    return ancestors_rec(db, person);
}

TRESULT * commonAncestors(TDATABASE * db, int id1, int id2) {
    TRESULT * ancestors1 = ancestors(db, id1);
    TRESULT * ancestors2 = ancestors(db, id2);

    TRESULT * result = tresult_intersection(ancestors1, ancestors2);

    freeResult(ancestors1);
    freeResult(ancestors2);

    return result;
}

// /============================= TDatabase

#ifndef __PROGTEST__

#include "./asserts.c"

#endif /* __PROGTEST__ */

