#include "server.h"

cJSON *mx_user_to_cjson(t_user *user) {
    if (!user) {
        return cJSON_CreateNull();
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", user->id);
    cJSON_AddStringToObject(obj, "name", user->name);
    cJSON_AddStringToObject(obj, "tag", user->tag);
    cJSON_AddStringToObject(obj, "password_hash", user->password_hash);
    cJSON_AddNumberToObject(obj, "photo_id", user->photo_id);
    cJSON_AddStringToObject(obj, "status", user->status);
    cJSON_AddStringToObject(obj, "description", user->description);
    cJSON_AddNumberToObject(obj, "created_at", user->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", user->edited_at);

    cJSON_AddItemToObject(obj, "rooms", mx_rooms_to_cjson(user->rooms));
    cJSON_AddItemToObject(obj, "favorites",
                          mx_favorites_to_cjson(user->favorites));
    cJSON_AddItemToObject(obj, "messages",
                          mx_messages_to_cjson(user->messages));
    cJSON_AddItemToObject(obj, "photo", mx_file_to_cjson(user->photo));

    return obj;
}

t_string mx_user_stringify(t_user *user) {
    cJSON *obj = mx_user_to_cjson(user);

    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}

cJSON *mx_users_to_cjson(t_list *users) {
    cJSON *users_array = cJSON_CreateArray();

    for (t_list *node = users; node; node = node->next) {
        cJSON_AddItemToArray(users_array, mx_users_to_cjson(node->data));
    }

    return users_array;
}
