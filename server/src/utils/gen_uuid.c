#include "utils.h"

char *mx_gen_uuid(void) {
    char v[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    t_string str = mx_strnew(36);

    for (int i = 0; i < 36; ++i) {
        str[i] = v[rand() % 16];
    }
    str[8] = '-';
    str[13] = '-';
    str[18] = '-';
    str[23] = '-';

    return str;
}
