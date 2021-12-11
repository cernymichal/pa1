#ifndef __PROGTEST__

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct TResult {
    struct TResult *m_Next;
    int m_ID;
    char *m_Name;
} TRESULT;

#endif /* __PROGTEST__ */

typedef struct TDatabase {
    /* todo */
} TDATABASE;

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

