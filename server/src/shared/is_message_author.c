#include "server.h"
bool mx_is_message_author(int user_id, int message_id) {
    t_message *message = mx_message_repo_get(message_id);
    bool res = message && message->author_id == user_id;

    mx_message_free(message);

    return res;
}
