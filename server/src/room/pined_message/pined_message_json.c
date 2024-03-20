#include "server.h"

cJSON *mx_pined_message_to_cjson(t_pined_message *pined) {
    if (!pined) {
        return cJSON_CreateNull();
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", pined->id);
    cJSON_AddNumberToObject(obj, "room_id", pined->room_id);
    cJSON_AddNumberToObject(obj, "message_id", pined->message_id);

    cJSON_AddNumberToObject(obj, "created_at", pined->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", pined->edited_at);

    cJSON_AddItemToObject(obj, "message", mx_message_to_cjson(pined->message));
    cJSON_AddItemToObject(obj, "room", mx_room_to_cjson(pined->room));

    return obj;
}

t_string mx_pined_message_stringify(t_pined_message *pined) {
    cJSON *obj = mx_pined_message_to_cjson(pined);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}

cJSON *mx_pined_messages_to_cjson(t_list *members) {
    cJSON *members_array = cJSON_CreateArray();

    for (t_list *node = members; node; node = node->next) {
        cJSON_AddItemToArray(members_array,
                             mx_pined_message_to_cjson(node->data));
    }

    return members_array;
}

t_string mx_pined_messages_stringify(t_list *members) {
    cJSON *obj = mx_pined_messages_to_cjson(members);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}
