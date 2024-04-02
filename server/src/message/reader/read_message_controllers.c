#include "server.h"

void mx_message_ctrl_read_message(t_connection *c, t_http_message *m) {
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
    t_read_message *read = mx_read_message(user_id, message_id);

    if (!read) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant read message");
        return;
    }
    t_string json_string = mx_read_message_stringify(read);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_ws_emit("message-was-read", read->message->room_id,
               mx_read_message_to_cjson(read));
    mx_strdel(&json_string);
    mx_read_message_free(read);
}

void mx_message_ctrl_get_reader(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int read_id = mx_extract_id_from_query(m->query, "read_id");

    if (read_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid reader id");
        return;
    }

    t_read_message *read = mx_read_message_get(read_id);

    if (!read) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Reader not found");
        return;
    }
    t_string json_string = mx_read_message_stringify(read);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_read_message_free(read);
}

void mx_message_ctrl_get_readers(t_connection *c, t_http_message *m) {
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

    t_list *reads = mx_read_messages_get(message_id);
    t_string json_string = mx_read_messages_stringify(reads);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON,
                  json_string ? json_string : "[]");
    mx_strdel(&json_string);
    mx_list_free(&reads, (t_func_free)mx_read_message_free);
}
