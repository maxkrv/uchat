#include "libmx.h"

long mx_atoi(const char *str) {
    long result = 0;
    int sign = 1;
    int i = 0;

    // Skip leading white spaces
    while (mx_isspace(str[i])) {
        i++;
    }

    // Handle optional sign
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Process digits
    while (mx_isdigit(str[i])) {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return sign * result;
}
