#include "message.h"

t_read_message *mx_read_message_repo_get(int id) {
    return mx_read_message_init();
}

t_list *mx_read_messages_repo_get(int message_id) {
    t_list *list = NULL;

    mx_push_back(&list, mx_read_message_repo_get(1));

    return list;
}

int mx_message_repo_read(int user_id, int message_id) {
    return 1;
}
