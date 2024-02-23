#include "libmx.h"

void mx_clear_list(t_list **list) {
    if (*list == NULL
        || list == NULL) {
        return;
    }

    while (*list != NULL) {
        t_list *temp = (*list)->next;
        free(*list);
        *list = temp;
    }

    list = NULL;
}
