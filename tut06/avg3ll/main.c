#ifndef __PROGTEST__
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>
#endif /* __PROGTEST__ */

int round(double a) {
    int sign = a >= 0 ? 1 : -1;
    double f = (a - (int) a) * sign;
    
    if (f >= .5)
        return (a * sign + 1) * sign;
    
    return a;
}

long long avg3 ( long long a, long long b, long long c ) {
    int m = (a % 3 + b % 3 + c % 3) / 3;
    return a / 3 + b / 3 + c / 3 + m;
}

#ifndef __PROGTEST__
int main (int argc, char * argv []) {
    assert ( avg3 ( 1, 2, 3 ) == 2 );
    assert ( avg3 ( -100, 100, 30 ) == 10 );
    assert ( avg3 ( 1, 2, 2 ) == 1 );
    assert ( avg3 ( -1, -2, -2 ) == -1 );
    assert ( avg3 ( LLONG_MAX, LLONG_MAX, LLONG_MAX ) == LLONG_MAX );
    assert ( avg3 ( 9223372036854775800LL, 9223372036854775800LL, -8LL ) == 6148914691236517197LL );
    return 0;
}
#endif /* __PROGTEST__ */

