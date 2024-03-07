#include "message.h"

t_message *mx_message_repo_get(int id) {
    return mx_init_message();
}

t_list *mx_message_repo_get_many(int room_id) {
    t_list *list;

    mx_push_back(&list, mx_message_repo_get(1));

    return list;
}

int mx_message_repo_create(t_message_create_dto *dto) {
    return 1;
}

bool mx_message_repo_put(int id, t_message_create_dto *dto) {
    return true;
}

bool mx_message_repo_delete(int id) {
    return true;
}
