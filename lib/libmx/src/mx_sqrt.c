#include "libmx.h"

int mx_sqrt(int x) {
    double result = 1;

    if (x <= 0) {
        return 0;
    }

    for (int i = 0; i <= 10000; i++) {
        result -= (result * result - x) / (result * 2);
    }

    if (result - (int) result == 0) {
        return (int) result;
    }

    return 0;
}

