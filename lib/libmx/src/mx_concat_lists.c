#include "libmx.h"

void mx_concat_lists(t_list **list1, t_list **list2) {
    if (list1 == NULL || list2 == NULL) return;

    if (*list1 == NULL) {
        *list1 = *list2;
        *list2 = NULL;
        return;
    }

    t_list *current = *list1;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = *list2;
    *list2 = NULL;
}
