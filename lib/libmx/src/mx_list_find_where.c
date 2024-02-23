#include "libmx.h"

t_list *mx_list_find_where(t_list *list, void *data, t_comp_func is_equal) {
    t_list *current = list;

    while (current != NULL) {
        if (is_equal(current->data, data)) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}
