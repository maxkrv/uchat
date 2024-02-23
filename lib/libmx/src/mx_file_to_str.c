#include "libmx.h"

static int find_file_length(const char *file) {
    short fl = open(file, O_RDONLY);
    short size = 0;
    int length = 0;
    char c;
    size = read(fl, &c, 1);
    while (size > 0) {
        size = read(fl, &c, 1);
        length++;
    }
    close(fl);
    return length;
}

char *mx_file_to_str(const char *file) {
    int fl = open(file, O_RDONLY);
    int size = 0;

    if (fl == -1) {
        close(fl);
        return NULL;
    }
    size = find_file_length(file);
    if (size == 0) {
        return NULL;
    }
    char *result = mx_strnew(size);
    read(fl, result, size);
    close(fl);

    return result;
}
