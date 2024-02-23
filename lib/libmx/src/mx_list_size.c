#include "libmx.h"

int mx_list_size(t_list *list) {
    int len = 0;

    for (t_list *l = list; l != NULL; l = l->next) {
        len++;
    }

    return len;
}

