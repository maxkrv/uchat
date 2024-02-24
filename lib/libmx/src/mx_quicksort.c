#include "libmx.h"

static void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

static int partition(char **arr, int left, int right, int *swap_count) {
    int pivot_len = mx_strlen(arr[(left + right) / 2]);

    while (left <= right) {
        while (mx_strlen(arr[left]) < pivot_len) left++;
        while (mx_strlen(arr[right]) > pivot_len) right--;

        if (left <= right) {
            if (mx_strlen(arr[left]) != mx_strlen(arr[right])) {
                swap(&arr[left], &arr[right]);
                (*swap_count)++;
            }
            left++;
            right--;
        }
    }

    return left;
}

int mx_quicksort(char **arr, int left, int right) {
    if (!arr) return -1;

    int swap_count = 0;

    if (left < right) {
        int pivotIndex = partition(arr, left, right, &swap_count);

        swap_count += mx_quicksort(arr, left, pivotIndex - 1);
        swap_count += mx_quicksort(arr, pivotIndex, right);
    }

    return swap_count;
}
