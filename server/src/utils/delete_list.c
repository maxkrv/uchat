
#include "utils.h"

void mx_delete_list(t_list **list, t_func_void func) {
    t_list *tmp = *list;

    while (tmp) {
        func(tmp->data);
        tmp = tmp->next;
    }
    mx_clear_list(list);
}