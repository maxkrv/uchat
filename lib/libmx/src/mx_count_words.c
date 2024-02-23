#include "libmx.h"

int mx_count_words(const char *str, char c) {
    if (str == NULL) {
        return -1;
    }

    if (!mx_strcmp(str, "")) {
        return 0;
    }

    int words = 0;
    bool in_word = false;

    while (*str != '\0') {
        if (*str == c) {
            in_word = false;
        } else {
            if (!in_word) {
                words++;
                in_word = true;
            }
        }
        str++;
    }

    return words;
}
