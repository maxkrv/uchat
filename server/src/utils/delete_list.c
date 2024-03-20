
#include "utils.h"

void mx_list_free(t_list **list, t_func_free func) {
    t_list *tmp = *list;

    while (tmp) {
        func(tmp->data);
        tmp = tmp->next;
    }
    mx_clear_list(list);
}
