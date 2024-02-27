// t_user to json
// req parse and validation
#include "server.h"

cJSON *mx_room_to_cjson(t_room *room) {
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", room->id);
    cJSON_AddBoolToObject(obj, "is_direct_room", room->is_direct_room);
    cJSON_AddStringToObject(obj, "name", room->name);
    cJSON_AddStringToObject(obj, "discription", room->discription);
    cJSON_AddNumberToObject(obj, "created_at", room->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", room->edited_at);

    cJSON_AddItemToObject(obj, "members",
                          mx_room_members_to_cjson(room->members));
    cJSON_AddItemToObject(obj, "messages",
                          mx_messages_to_cjson(room->messages));

    return obj;
}

t_string mx_room_stringify(t_room *room) {
    cJSON *obj = mx_room_to_cjson(room);

    t_string res = mg_str(cJSON_PrintUnformatted(obj));

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
