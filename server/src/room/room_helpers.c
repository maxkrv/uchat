//t_user to json
//req parse and validation
#include "server.h"

cJSON *mx_room_to_cjson(t_room *room) {
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", room->id);
    cJSON_AddNumberToObject(obj, "created_at", room->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", room->edited_at);

//    cJSON_AddArrayToObject(mx_rooms_to_cjson(user->rooms));
//    cJSON_AddArrayToObject(mx_messages_to_cjson(user->messages));
//    cJSON_AddArrayToObject(mx_read_messages_to_cjson(user->read_messages));

    return obj;
}

t_string mx_room_stringify(t_room *room) {
    cJSON *obj = mx_room_to_cjson(room);

    return mg_str(cJSON_PrintUnformatted(obj));
}

cJSON *mx_rooms_to_cjson(t_list *rooms) {
    cJSON *rooms_array = cJSON_CreateArray();

    for (t_list *node = users; node; node = node->next) {
        cJSON_AddItemToArray(users_array, mx_room_to_cjson(node->data));
    }

    return rooms_array;
}
