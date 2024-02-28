#include "message.h"

void mx_message_ctrl_get(t_connection *c, t_http_message *m) {
    int message_id = mx_extract_id_from_query(m->query);

    if (message_id < 0) {
        mg_http_reply(c, HTTP_STATUS_UNPROCESSABLE_ENTITY, MX_EMPTY, MX_EMPTY);
    }

    t_message *message = mx_message_get(message_id);
    t_string json_string = mx_message_stringify(message);

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
}

void mx_message_ctrl_post(t_connection *c, t_http_message *m) {
    t_message_create_dto *dto = mx_get_message_create_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid message data provided");
        return;
    }
    t_message *message = mx_message_create(dto);

    if (!message) {
        mx_http_reply_exception(c, m, HTTP_STATUS_INTERNAL_SERVER_ERROR,
                                "Cant create message");
        return;
    }

    t_string json_string = mx_message_stringify(message);

    free(dto);
    mg_http_reply(c, HTTP_STATUS_CREATED, MX_EMPTY, json_string.ptr);
}

void mx_message_ctrl_put(t_connection *c, t_http_message *m) {
    int message_id = mx_extract_id_from_query(m->query);
    t_message_create_dto *dto = mx_get_message_create_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid message data provided");
        return;
    }

    t_message *message = mx_message_put(message_id, dto);

    if (!message) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Cant update message");
        return;
    }

    t_string json_string = mx_message_stringify(message);

    free(dto);
    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
}

void mx_message_ctrl_delete(t_connection *c, t_http_message *m) {
    int message_id = mx_extract_id_from_query(m->query);

    if (message_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid message id provided");
        return;
    }

    t_message *message = mx_message_delete(message_id);

    if (!message) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Message not found");
        return;
    }

    t_string json_string = mx_message_stringify(message);

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
}
