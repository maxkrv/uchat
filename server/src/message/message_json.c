#include "server.h"

cJSON *mx_message_to_cjson(t_message *message) {
    if (!message) {
        return cJSON_CreateNull();
    }

    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", message->id);
    cJSON_AddNumberToObject(obj, "room_id", message->room_id);
    cJSON_AddNumberToObject(obj, "author_id", message->author_id);
    cJSON_AddNumberToObject(obj, "reply_id", message->reply_id);
    cJSON_AddStringToObject(obj, "text", message->text);
    cJSON_AddNumberToObject(obj, "created_at", message->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", message->edited_at);

    cJSON_AddItemToObject(obj, "readed_by",
                          mx_read_messages_to_cjson(message->readed_by));
    cJSON_AddItemToObject(obj, "author", mx_user_to_cjson(message->author));
    cJSON_AddItemToObject(obj, "room", mx_room_to_cjson(message->room));
    cJSON_AddItemToObject(obj, "reply", mx_message_to_cjson(message->reply));
    cJSON_AddItemToObject(obj, "files", mx_files_to_cjson(message->files));

    return obj;
}

t_string mx_message_stringify(t_message *message) {
    if (!message) {
        return NULL;
    }
    cJSON *obj = mx_message_to_cjson(message);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}

cJSON *mx_messages_to_cjson(t_list *messages) {
    if (!messages) {
        return NULL;
    }
    cJSON *rooms_array = cJSON_CreateArray();

    for (t_list *node = messages; node; node = node->next) {
        cJSON_AddItemToArray(rooms_array, mx_message_to_cjson(node->data));
    }

    return rooms_array;
}

t_string mx_messages_stringify(t_list *messages) {
    if (!messages) {
        return NULL;
    }
    cJSON *obj = mx_messages_to_cjson(messages);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}
