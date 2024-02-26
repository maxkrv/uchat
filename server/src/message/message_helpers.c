// t_user to json
// req parse and validation
#include "server.h"

cJSON *mx_message_to_cjson(t_message *message) {
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", message->id);
    cJSON_AddNumberToObject(obj, "room_id", message->room_id);
    cJSON_AddNumberToObject(obj, "author_id", message->author_id);
    cJSON_AddNumberToObject(obj, "reply_id", message->reply_id);
    cJSON_AddStringToObject(obj, "test", message->text);
    cJSON_AddBoolToObject(obj, "discription", message->is_edited);
    cJSON_AddNumberToObject(obj, "created_at", message->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", message->edited_at);

    cJSON_AddItemToObject(obj, "read_messages",
                          mx_read_messages_to_cjson(message->read_messages));
    cJSON_AddItemToObject(obj, "author", mx_user_to_cjson(message->author));
    cJSON_AddItemToObject(obj, "room", mx_room_to_cjson(message->room));

    return obj;
}

t_string mx_message_stringify(t_message *message) {
    cJSON *obj = mx_message_to_cjson(message);

    return mg_str(cJSON_PrintUnformatted(obj));
}

cJSON *mx_messages_to_cjson(t_list *messages) {
    cJSON *rooms_array = cJSON_CreateArray();

    for (t_list *node = messages; node; node = node->next) {
        cJSON_AddItemToArray(rooms_array, mx_message_to_cjson(node->data));
    }

    return rooms_array;
}
