#include "libmx.h"

char *mx_itoa(int number) {
    if (number == 0) {
        return mx_strdup("0");
    }

    int length = 0;
    int number_copy = number;

    while (number_copy != 0) {
        number_copy /= 10;
        length++;
    }

    char *number_string = mx_strnew(length + (number < 0 ? 1 : 0));
    int index = length + (number < 0 ? 0 : -1);

    if (number < 0) {
        number_string[0] = '-';
        number *= -1;
    }

    while (number != 0) {
        number_string[index--] = (number % 10) + '0';
        number /= 10;
    }

    return number_string;
}
