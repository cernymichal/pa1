#ifndef __PROGTEST__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#endif /* __PROGTEST__ */

#define MIN_YEAR 1600

int digit_consumption_3[] = {4, 5, 3};
int digit_consumption_4[] = {4, 5, 2, 3};
int digit_consumption_6[] = {4, 5, 2, 3, 3, 3};
int digit_consumption_10[] = {4, 5, 2, 3, 3, 1, 5, 4, 1, 2};

// sums of digit_consumption_n
#define CONSUMPTION_DIGIT_3 12
#define CONSUMPTION_DIGIT_4 14
#define CONSUMPTION_DIGIT_6 20
#define CONSUMPTION_DIGIT_10 30

#define CONSUMPTION_MINUTE CONSUMPTION_DIGIT_6 + 6 * CONSUMPTION_DIGIT_10
#define CONSUMPTION_HOUR CONSUMPTION_MINUTE + 60 * CONSUMPTION_MINUTE
#define CONSUMPTION_DAY CONSUMPTION_DIGIT_3 + 2 * CONSUMPTION_DIGIT_10 + CONSUMPTION_DIGIT_4 + 24 * CONSUMPTION_HOUR

void swap_vars(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int is_leap_year(int year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0) {
                if (year % 4000 == 0)
                    return 0;

                return 1;
            }
            return 0;
        }
        return 1;
    }
    return 0;
}

int days_in_month(int month, int leap_year) {
    if (month == 2) {
        if (leap_year)
            return 29;

        return 28;
    }

    if (month > 7)
        month++;

    if (month % 2 == 0)
        return 30;

    return 31;
}

// in minutes from MIN_YEAR
/*
long date_to_timestamp(int year, int month, int day, int hour, int minute) {
    long timestamp = 0;

    for (int y = MIN_YEAR; y < year; y++)
        timestamp += (leap_year ? 366 : 365) * 24 * 60;

    for (int m = 1; m < month; m++)
        timestamp += days_in_month(month, leap_year) * 24 * 60;

    timestamp += (day - 1) * 24 * 60;
    timestamp += hour * 60;
    timestamp += minute;

    return timestamp;
}
*/

int valid_date(int year, int month, int day, int hour, int minute) {
    if (
        year < MIN_YEAR
        || month < 1 || month > 12
        || day < 1 || day > days_in_month(month, is_leap_year(year))
        || hour < 1 || hour > 23
        || minute < 1 || minute > 59
    )
        return 0;

    return 1;
}

long long consumption_to_end_of_hour(int minute) {
    long long consumption = 0;

    for (; minute % 10 != 0; minute++)
        consumption += digit_consumption_10[minute % 10] + CONSUMPTION_MINUTE;
    
    for (; minute < 60; minute += 10)
        consumption += digit_consumption_6[minute / 10] + CONSUMPTION_DIGIT_10 + 10 * CONSUMPTION_MINUTE;

    return consumption;
}

long long consumption_to_end_of_day(int hour, int minute) {
    long long consumption = consumption_to_end_of_hour(minute);
    
    if (hour == 23 || hour % 10 == 9)
        consumption += digit_consumption_3[hour / 10];
    
    consumption += digit_consumption_10[hour];

    hour++;

    for (; hour < 24; hour++) {
        if (hour == 23 || hour % 10 == 9)
            consumption += digit_consumption_3[hour / 10];

        consumption += digit_consumption_10[hour] + CONSUMPTION_HOUR;
    }
    
    return consumption;
}

long long consumption_to_end_of_month(int leap_year, int month, int day, int hour, int minute) {
    long long consumption = consumption_to_end_of_day(hour, minute);

    day++;
    
    int days = days_in_month(month, leap_year);
    for(; day <= days; day++)
        consumption += CONSUMPTION_DAY;

    return consumption;
}

long long consumption_to_end_of_year(int year, int month, int day, int hour, int minute) {
    int leap_year = is_leap_year(year);

    long long consumption = consumption_to_end_of_month(leap_year, month, day, hour, minute);
    
    month++;

    for(; month <= 12; month++)
        consumption += days_in_month(month, leap_year) * CONSUMPTION_DAY;

    return consumption;
}

int energyConsumption(
    int y1, int m1, int d1, int h1, int i1,
    int y2, int m2, int d2, int h2, int i2,
    long long int *consumption
) {
    if (!valid_date(y1, m1, d1, h1, i1) || !valid_date(y2, m2, d2, h2, i2))
        return 0;
    
    // ensure date_1 <= date_2
    if (
        y1 > y2
        || (y1 == y2 && m1 > m2)
        || (y1 == y2 && m1 == m2 && d1 > d2)
        || (y1 == y2 && m1 == m2 && d1 == d2 && h1 > h2)
        || (y1 == y2 && m1 == m2 && d1 == d2 && h1 == h2 && i1 > i2)
    ) {
        swap_vars(&y1, &y2);
        swap_vars(&m1, &m2);
        swap_vars(&d1, &d2);
        swap_vars(&h1, &h2);
        swap_vars(&i1, &i2);
    }
    
    *consumption = consumption_to_end_of_year(y1, m1, d1, h1, i1);
    y1++;

    for(; y1 < y2; y1++)
        *consumption += (is_leap_year(y1) ? 366 : 365) * CONSUMPTION_DAY;

    *consumption -= consumption_to_end_of_year(y2, m2, d2, h2, i2);

    return 1;
}
 
#ifndef __PROGTEST__

void cal_asserts() {
    assert(!is_leap_year(1901));
    assert(!is_leap_year(1902));
    assert(!is_leap_year(1903));
    assert(!is_leap_year(1905));
    assert(is_leap_year(1904));
    assert(is_leap_year(1908));
    assert(is_leap_year(1996));
    assert(is_leap_year(2004));
    assert(!is_leap_year(1700));
    assert(!is_leap_year(1800));
    assert(!is_leap_year(1900));
    assert(!is_leap_year(2100));
    assert(is_leap_year(1600));
    assert(is_leap_year(2000));
    assert(is_leap_year(2400));
    assert(is_leap_year(3600));
    assert(is_leap_year(4400));
    assert(!is_leap_year(4000));
    assert(!is_leap_year(8000));

    assert(days_in_month(1, 0) == 31);
    assert(days_in_month(2, 0) == 28);
    assert(days_in_month(2, 1) == 29);
    assert(days_in_month(3, 0) == 31);
    assert(days_in_month(4, 0) == 30);
    assert(days_in_month(5, 0) == 31);
    assert(days_in_month(6, 0) == 30);
    assert(days_in_month(7, 0) == 31);
    assert(days_in_month(8, 0) == 31);
    assert(days_in_month(9, 0) == 30);
    assert(days_in_month(10, 0) == 31);
    assert(days_in_month(11, 0) == 30);
    assert(days_in_month(12, 0) == 31);
}

void consumption_asserts() {
    long long consumption;

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
}

void from_stdin() {
    int y1, m1, d1, h1, i1, y2, m2, d2, h2, i2;

    scanf(" %d %d %d %d %d", &y1, &m1, &d1, &h1, &i1);
    scanf(" %d %d %d %d %d", &y2, &m2, &d2, &h2, &i2);

    long long consumption = 0;
    int result = energyConsumption(y1, m1, d1, h1, i1, y2, m2, d2, h2, i2, &consumption);

    printf("%d %lld\n", result, consumption);
}

int main(int argc, char *argv[]) {
    // cal_asserts();
    // consumption_asserts();
    from_stdin();

    return 0;
}
 
#endif /* __PROGTEST__ */
