#include "server.h"

cJSON *mx_favorite_to_cjson(t_favorite_room *fav) {
    if (!fav) {
        return cJSON_CreateNull();
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", fav->id);
    cJSON_AddNumberToObject(obj, "room_id", fav->room_id);
    cJSON_AddNumberToObject(obj, "user_id", fav->user_id);

    cJSON_AddNumberToObject(obj, "created_at", fav->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", fav->edited_at);

    cJSON_AddItemToObject(obj, "user", mx_user_to_cjson(fav->user));
    cJSON_AddItemToObject(obj, "room", mx_room_to_cjson(fav->room));

    return obj;
}

t_string mx_favorite_stringify(t_favorite_room *fav) {
    cJSON *obj = mx_favorite_to_cjson(fav);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}

cJSON *mx_favorites_to_cjson(t_list *members) {
    cJSON *members_array = cJSON_CreateArray();

    for (t_list *node = members; node; node = node->next) {
        cJSON_AddItemToArray(members_array, mx_favorite_to_cjson(node->data));
    }

    return members_array;
}

t_string mx_favorites_stringify(t_list *members) {
    cJSON *obj = mx_favorites_to_cjson(members);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}
