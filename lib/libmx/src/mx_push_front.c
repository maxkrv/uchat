#include "libmx.h"

void mx_push_front(t_list **list, void *data) {
    if (*list == NULL
        || list == NULL) {
        *list = mx_create_node(data);
        return;
    }

    t_list *l = mx_create_node(data);

    l->next = *list;
    *list = l;
}

