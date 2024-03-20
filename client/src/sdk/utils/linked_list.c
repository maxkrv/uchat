#include "sdk/utils.h"

void mx_delete_node(t_list **list, t_list *node, t_func_free clean_data) {
    if (!list || !*list || !node) {
        return;
    }
    if (*list == node) {
        if (clean_data) {
            clean_data(node->data);
        }
        mx_pop_front(list);
        return;
    }
    t_list *tmp = *list;

    while (tmp) {
        if (tmp->next == node) {
            tmp->next = node->next;
            if (clean_data) {
                clean_data(node->data);
            }
            free(node);
            return;
        }
        tmp = tmp->next;
    }
}

void mx_list_free(t_list **list, t_func_free func) {
    t_list *tmp = *list;

    while (tmp) {
        func(tmp->data);
        tmp = tmp->next;
    }
    mx_clear_list(list);
}
