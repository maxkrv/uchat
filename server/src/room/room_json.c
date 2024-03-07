// t_user to json
// req parse and validation
#include "server.h"

cJSON *mx_room_to_cjson(t_room *room) {
    if (!room) {
        return cJSON_CreateNull();
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", room->id);
    cJSON_AddStringToObject(obj, "name", room->name);
    cJSON_AddStringToObject(obj, "type", room->type);
    cJSON_AddNumberToObject(obj, "photo_id", room->photo_id);
    cJSON_AddStringToObject(obj, "description", room->description);
    cJSON_AddNumberToObject(obj, "created_at", room->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", room->edited_at);

    cJSON_AddItemToObject(obj, "members", mx_members_to_cjson(room->members));
    cJSON_AddItemToObject(obj, "messages",
                          mx_messages_to_cjson(room->messages));
    cJSON_AddItemToObject(obj, "pined_messages",
                          mx_pined_messages_to_cjson(room->pined_messages));
    cJSON_AddItemToObject(obj, "photo", mx_file_to_cjson(room->photo));

    return obj;
}

t_string mx_room_stringify(t_room *room) {
    cJSON *obj = mx_room_to_cjson(room);

    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}

cJSON *mx_rooms_to_cjson(t_list *rooms) {
    cJSON *rooms_array = cJSON_CreateArray();

    for (t_list *node = rooms; node; node = node->next) {
        cJSON_AddItemToArray(rooms_array, mx_room_to_cjson(node->data));
    }

    return rooms_array;
}

t_string mx_rooms_stringify(t_list *rooms) {
    cJSON *obj = mx_rooms_to_cjson(rooms);

    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}
