#include "server.h"

t_string mx_ws_message_stringify(const char *event, cJSON *payload) {
    cJSON *message_obj = cJSON_CreateObject();

    cJSON_AddItemToObject(message_obj, "event", cJSON_CreateString(event));
    cJSON_AddItemToObject(message_obj, "payload", payload);

    t_string message = cJSON_PrintUnformatted(message_obj);
    cJSON_Delete(message_obj);

    return message;
}
