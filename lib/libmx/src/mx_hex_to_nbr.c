#include "libmx.h"

static int get_len(const char *hex) {
    int i = 0;

    while (hex[i] != '\0') {
        i++;
    }

    return i;
}

unsigned long mx_hex_to_nbr(const char *hex) {
    unsigned long temp = 1;
    unsigned long res = 0;
    int len = get_len(hex);

    for (int i = len - 1; i >= 0; --i) {
        char ch = hex[i];

        if (mx_isalpha(ch)) {
            res += (ch - (mx_islower(ch) ? 87 : 55)) * temp;
        } else if (mx_isdigit(ch)) {
            res += (ch - 48) * temp;
        } else if (mx_isupper(ch)) {
            return 0;
        }

        temp *= 16;
    }

    return res;
}
