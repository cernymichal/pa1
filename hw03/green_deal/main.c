#ifndef __PROGTEST__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#endif /* __PROGTEST__ */

int energyConsumption(
    int y1, int m1, int d1, int h1, int i1,
    int y2, int m2, int d2, int h2, int i2,
    long long int *consumption
) {
    return 1;
}
 
#ifndef __PROGTEST__

int main(int argc, char *argv[]) {
    long long int consumption;    
    
    assert(
        energyConsumption(
            2021, 10,  1, 13, 15,
            2021, 10,  1, 18, 45, &consumption
        ) == 1
        && consumption == 67116LL
    );
    
    assert(
        energyConsumption(
            2021, 10,  1, 13, 15,
            2021, 10,  2, 11, 20, &consumption
        ) == 1
        && consumption == 269497LL
    );
    
    assert(
        energyConsumption(
            2021,  1,  1, 13, 15,
            2021, 10,  5, 11, 20, &consumption
        ) == 1
        && consumption == 81106033LL
    );
    
    assert(
        energyConsumption(
            2024,  1,  1, 13, 15,
            2024, 10,  5, 11, 20, &consumption
        ) == 1
        && consumption == 81398919LL
    );
    
    assert(
        energyConsumption(
            1900,  1,  1, 13, 15,
            1900, 10,  5, 11, 20, &consumption
        ) == 1
        && consumption == 81106033LL
    );
    
    assert(
        energyConsumption(
            2021, 10,  1,  0,  0,
            2021, 10,  1, 12,  0, &consumption
        ) == 1
        && consumption == 146443LL
    );
    
    assert(
        energyConsumption(
            2021, 10,  1,  0, 15,
            2021, 10,  1,  0, 25, &consumption
        ) == 1
        && consumption == 2035LL
    );
    
    assert(
        energyConsumption(
            2021, 10,  1, 12,  0,
            2021, 10,  1, 12,  0, &consumption
        ) == 1
        && consumption == 0LL
    );
    
    assert(
        energyConsumption(
            2021, 10,  1, 12,  0,
            2021, 10,  1, 12,  1, &consumption
        ) == 1
        && consumption == 204LL
    );
    
    assert(
        energyConsumption(
            2021, 11,  1, 12,  0,
            2021, 10,  1, 12,  0, &consumption
        ) == 0
    );
    
    assert(
        energyConsumption(
            2021, 10, 32, 12,  0,
            2021, 11, 10, 12,  0, &consumption
        ) == 0
    );
    
    assert(
        energyConsumption(
            2100,  2, 29, 12,  0,
            2100,  2, 29, 12,  0, &consumption
        ) == 0
    );
  
    assert(
        energyConsumption(
            2400,  2, 29, 12,  0,
            2400,  2, 29, 12,  0, &consumption
        ) == 1
        && consumption == 0LL
    );
  
  return 0;
}
 
#endif /* __PROGTEST__ */
