#include "libmx.h"

t_list *mx_sort_list(t_list *lst, bool (*cmp)(void *a, void *b)) {
    if (lst && cmp) {
        for (t_list *temp1 = lst; temp1; temp1 = temp1->next) {
	        for (t_list *temp2 = lst; temp2->next; temp2 = temp2->next) {
		        if (cmp(temp2->data, temp2->next->data)) {
			        void *temp = temp2->data;
                    temp2->data = temp2->next->data;
                    temp2->next->data = temp;
                }
            }
        }
    }
    return lst;
}

