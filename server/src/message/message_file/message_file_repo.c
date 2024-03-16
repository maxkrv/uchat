#include "server.h"

t_list *mx_message_file_repo_get_all(int user_id) {
    t_list *list = NULL;

    mx_push_back(&list, mx_file_repo_get(1));

    return list;
}

int mx_message_file_repo_create(int file_id, char *type) {
    return 1;
}

bool mx_message_file_repo_delete(int id) {
    return true;
}
