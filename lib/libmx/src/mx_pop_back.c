#include "libmx.h"

void mx_pop_back(t_list **head) {
    if (head == NULL || *head == NULL) {
        return;
    }

    t_list **current = head;
    while ((*current)->next != NULL) {
        current = &((*current)->next);
    }

    free(*current);
    *current = NULL;
}
