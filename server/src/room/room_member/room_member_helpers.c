#include "server.h"

cJSON *mx_room_member_to_cjson(t_room_member *member) {
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", member->id);
    cJSON_AddNumberToObject(obj, "room_id", member->room_id);
    cJSON_AddNumberToObject(obj, "user_id", member->user_id);

    cJSON_AddNumberToObject(obj, "created_at", member->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", member->edited_at);

    cJSON_AddItemToObject(obj, "user", mx_user_to_cjson(member->user));
    cJSON_AddItemToObject(obj, "room", mx_room_to_cjson(member->room));

    return obj;
}

t_string mx_room_member_stringify(t_room_member *member) {
    cJSON *obj = mx_room_member_to_cjson(member);

    return mg_str(cJSON_PrintUnformatted(obj));
}

cJSON *mx_room_members_to_cjson(t_list *members) {
    cJSON *members_array = cJSON_CreateArray();

    for (t_list *node = members; node; node = node->next) {
        cJSON_AddItemToArray(members_array,
                             mx_room_member_to_cjson(node->data));
    }

    return members_array;
}

t_string mx_room_members_stringify(t_list *members) {
    cJSON *obj = mx_room_members_to_cjson(members);

    return mg_str(cJSON_PrintUnformatted(obj));
}
