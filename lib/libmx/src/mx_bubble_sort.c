#include "libmx.h"

int mx_bubble_sort(char **arr, int size) {
    int swap = 0;

    for (int i = 0; i < size - 1; ++i) {
        if (mx_strcmp(arr[i], arr[i + 1]) <= 0) {
            continue;
        }
        char *temp = arr[i + 1];

        arr[i + 1] = arr[i];
        arr[i] = temp;
        swap++;
    }

    if (swap != 0) {
        return swap + mx_bubble_sort(arr, size);
    }

    return swap;
}

