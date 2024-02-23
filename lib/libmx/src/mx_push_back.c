#include "libmx.h"

void mx_push_back(t_list **list, void *data) {
    if (*list == NULL
        || list == NULL) {
        *list = mx_create_node(data);
        return;
    }

    t_list *l = *list;

    while (l->next != NULL) {
        l = l->next;
    }

    l->next = mx_create_node(data);
}
