#include "message.h"

t_list *mx_read_messages_get(int message_id) {
    return mx_read_messages_get(message_id);
}

t_read_message *mx_read_message_get(int id) {
    return mx_read_message_get(id);
}

t_read_message *mx_read_message(int user_id, int message_id) {
    int read_id = mx_message_repo_read(user_id, message_id);

    if (read_id <= 0) {
        return NULL;
    }

    return mx_read_message_get(read_id);
}
