#include "server.h"

void mx_message_ctrl_get(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }

    int message_id = mx_extract_id_from_query(m->query, "message_id");

    if (message_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid message id");
        return;
    }

    t_message *message = mx_message_get(message_id);

    if (!message) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Message not found");
        return;
    }
    t_string json_string = mx_message_stringify(message);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_message_free(message);
}

void mx_message_ctrl_get_many(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (room_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room id");
        return;
    }
    if (!mx_is_user_member_of(room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        return;
    }
    t_list *messages = mx_message_get_many(room_id);
    t_string json_string = mx_messages_stringify(messages);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON,
                  json_string ? json_string : "[]");
    mx_strdel(&json_string);
    mx_list_free(&messages, (t_func_free)mx_message_free);
}

void mx_message_ctrl_post(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    t_message_create_dto *dto = mx_message_create_dto_get(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid dto provided");
        return;
    }
    if (!mx_is_user_member_of(dto->room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        mx_message_create_dto_free(dto);
        return;
    }

    t_message *message = mx_message_create(user_id, dto);

    mx_message_create_dto_free(dto);
    if (!message) {
        mx_http_reply_exception(c, m, HTTP_STATUS_INTERNAL_SERVER_ERROR,
                                "Cant create message");
        return;
    }

    t_string json_string = mx_message_stringify(message);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_ws_emit("message-created", message->room_id,
               mx_message_to_cjson(message));
    mx_strdel(&json_string);
    mx_message_free(message);
}

void mx_message_ctrl_put(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int message_id = mx_extract_id_from_query(m->query, "message_id");

    if (message_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid message id provided");
        return;
    }

    t_message_create_dto *dto = mx_message_update_dto_get(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid message data provided");
        return;
    }
    if (!mx_is_message_author(user_id, message_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        mx_message_create_dto_free(dto);
        return;
    }

    t_message *message = mx_message_put(message_id, dto);

    mx_message_create_dto_free(dto);
    if (!message) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Cant update message");
        return;
    }

    t_string json_string = mx_message_stringify(message);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_ws_emit("message-updated", message->room_id,
               mx_message_to_cjson(message));
    mx_strdel(&json_string);
    mx_message_free(message);
}

void mx_message_ctrl_delete(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int message_id = mx_extract_id_from_query(m->query, "message_id");

    if (message_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid message id provided");
        return;
    }
    t_message *mess = mx_message_get(message_id);

    if (!mess) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Message not found");
        return;
    }
    if (!mx_is_message_author(user_id, message_id) &&
        !mx_is_room_admin(mess->room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        mx_message_free(mess);
        return;
    }
    mx_message_free(mess);

    t_message *message = mx_message_delete(message_id);

    if (!message) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Message not found");
        return;
    }

    t_string json_string = mx_message_stringify(message);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_ws_emit("message-deleted", message->room_id,
               mx_message_to_cjson(message));
    mx_strdel(&json_string);
    mx_message_free(message);
}
