#include "client.h"

t_ws_message *mx_ws_message_init(void) {
    t_ws_message *message = (t_ws_message *)malloc(sizeof(t_ws_message));

    message->event = NULL;
    message->payload = NULL;

    return message;
}

void mx_ws_message_free(t_ws_message *message, t_func_free payload_free) {
    if (!message) {
        return;
    }
    if (message->event) {
        mx_strdel(&message->event);
    }
    if (payload_free && message->payload) {
        payload_free(message->payload);
    }
    free(message);
}

t_ws_message *mx_ws_message_parse(t_string data, t_func_parser parse_payload) {
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        return NULL;
    }
    t_ws_message *message = mx_ws_message_init();

    if (parse_payload) {
        message->payload = parse_payload(cJSON_GetKey(json, "payload"));
    }

    message->event = mx_cjson_get_string(json, "event");

    cJSON_Delete(json);

    return message;
}

t_string mx_ws_message_parse_event(t_string data) {
    if (!data) {
        return NULL;
    }

    cJSON *json = cJSON_Parse(data);

    if (!json) {
        return NULL;
    }
    t_string event = mx_cjson_get_string(json, "event");

    cJSON_Delete(json);
    return event;
}
