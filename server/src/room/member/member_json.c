#include "server.h"

cJSON *mx_member_to_cjson(t_room_member *member) {
    if (!member) {
        return cJSON_CreateNull();
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", member->id);
    cJSON_AddNumberToObject(obj, "room_id", member->room_id);
    cJSON_AddNumberToObject(obj, "user_id", member->user_id);
    cJSON_AddBoolToObject(obj, "is_admin", member->is_admin);

    cJSON_AddNumberToObject(obj, "created_at", member->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", member->edited_at);

    cJSON_AddItemToObject(obj, "user", mx_user_to_cjson(member->user));
    cJSON_AddItemToObject(obj, "room", mx_room_to_cjson(member->room));

    return obj;
}

t_string mx_member_stringify(t_room_member *member) {
    cJSON *obj = mx_member_to_cjson(member);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}

cJSON *mx_members_to_cjson(t_list *members) {
    cJSON *members_array = cJSON_CreateArray();

    for (t_list *node = members; node; node = node->next) {
        cJSON_AddItemToArray(members_array, mx_member_to_cjson(node->data));
    }

    return members_array;
}

t_string mx_members_stringify(t_list *members) {
    cJSON *obj = mx_members_to_cjson(members);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}
