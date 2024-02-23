#include "libmx.h"

void mx_pop_front(t_list **head) {
    if (head == NULL || *head == NULL) {
        return;
    }

    t_list *tmp = *head;
    *head = (*head)->next;
    free(tmp);
}
