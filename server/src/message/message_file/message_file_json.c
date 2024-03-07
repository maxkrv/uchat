#include "server.h"

cJSON *mx_message_file_to_cjson(t_message_file *message_file) {
    if (!message_file) {
        return cJSON_CreateNull();
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", message_file->id);
    cJSON_AddNumberToObject(obj, "file_id", message_file->file_id);
    cJSON_AddNumberToObject(obj, "message_id", message_file->message_id);
    cJSON_AddStringToObject(obj, "type", message_file->type);

    cJSON_AddNumberToObject(obj, "created_at", message_file->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", message_file->edited_at);

    cJSON_AddItemToObject(obj, "file", mx_file_to_cjson(message_file->file));
    cJSON_AddItemToObject(obj, "message",
                          mx_message_to_cjson(message_file->message));

    return obj;
}

t_string mx_message_file_stringify(t_message_file *message) {
    cJSON *obj = mx_message_file_to_cjson(message);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}

cJSON *mx_message_files_to_cjson(t_list *messages) {
    cJSON *members_array = cJSON_CreateArray();

    for (t_list *node = messages; node; node = node->next) {
        cJSON_AddItemToArray(members_array,
                             mx_message_file_to_cjson(node->data));
    }

    return members_array;
}

t_string mx_message_files_stringify(t_list *messages) {
    cJSON *obj = mx_message_files_to_cjson(messages);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}
