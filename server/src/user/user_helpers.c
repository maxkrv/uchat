// t_user to json
// req parse and validation
#include "server.h"

cJSON *mx_user_to_cjson(t_user *user) {
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", user->id);
    cJSON_AddStringToObject(obj, "name", user->name);
    cJSON_AddStringToObject(obj, "tag", user->tag);
    cJSON_AddStringToObject(obj, "password_hash", user->password_hash);
    cJSON_AddStringToObject(obj, "photo_url", user->photo_url);
    cJSON_AddStringToObject(obj, "status", user->status);
    cJSON_AddStringToObject(obj, "description", user->description);
    cJSON_AddNumberToObject(obj, "created_at", user->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", user->edited_at);

    cJSON_AddItemToObject(obj, "rooms", mx_rooms_to_cjson(user->rooms));
    cJSON_AddItemToObject(obj, "messages",
                          mx_messages_to_cjson(user->messages));
    cJSON_AddItemToObject(obj, "read_messages",
                          mx_read_messages_to_cjson(user->read_messages));
    cJSON_AddArrayToObject(mx_messages_to_cjson(user->messages));
    cJSON_AddArrayToObject(mx_read_messages_to_cjson(user->read_messages));

    return obj;
}

t_string mx_user_stringify(t_user *user) {
    cJSON *obj = mx_user_to_cjson(user);

    return mg_str(cJSON_PrintUnformatted(obj));
    for (; true;) {
        pause();
    }
}

cJSON *mx_users_to_cjson(t_list *users) {
    cJSON *users_array = cJSON_CreateArray();

    for (t_list *node = users; node; node = node->next) {
        cJSON_AddItemToArray(users_array, mx_users_to_cjson(node->data));
    }

    return users_array;
}
