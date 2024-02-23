#include "libmx.h"

int mx_binary_search(char **arr, int size, const char *s, int *count) {
    int first_index = 0;
    int last_index = size - 1;
    int mid_index;

    while (first_index <= last_index) {
        mid_index = (first_index + last_index) / 2;
        (*count)++;

        int cmp_result = mx_strcmp(arr[mid_index], s);

        if (cmp_result > 0) {
            last_index = mid_index - 1;
        } else if (cmp_result < 0) {
            first_index = mid_index + 1;
        } else {
            return mid_index;
        }
    }

    *count = 0;
    return -1;
}
