#include "server.h"

cJSON *mx_read_message_to_cjson(t_read_message *message) {
    if (!message) {
        return cJSON_CreateNull();
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", message->id);
    cJSON_AddNumberToObject(obj, "user_id", message->user_id);
    cJSON_AddNumberToObject(obj, "message_id", message->message_id);

    cJSON_AddNumberToObject(obj, "created_at", message->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", message->edited_at);

    cJSON_AddItemToObject(obj, "user", mx_user_to_cjson(message->user));
    cJSON_AddItemToObject(obj, "message",
                          mx_message_to_cjson(message->message));

    return obj;
}

t_string mx_read_message_stringify(t_read_message *message) {
    cJSON *obj = mx_read_message_to_cjson(message);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}

cJSON *mx_read_messages_to_cjson(t_list *messages) {
    cJSON *members_array = cJSON_CreateArray();

    for (t_list *node = messages; node; node = node->next) {
        cJSON_AddItemToArray(members_array,
                             mx_read_message_to_cjson(node->data));
    }

    return members_array;
}

t_string mx_read_messages_stringify(t_list *messages) {
    cJSON *obj = mx_read_messages_to_cjson(messages);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}
