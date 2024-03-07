#include "message.h"

void mx_message_ctrl_read(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);

    if (id <= 0) {
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
    t_read_message *read = mx_read_message(id, message_id);

    if (!read) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Message not found");
        return;
    }
    t_string json_string = mx_read_message_stringify(read);

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string);
    mx_strdel(&json_string);
    mx_delete_read_message(read);
}

void mx_message_ctrl_get_read(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);

    if (id <= 0) {
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

    t_read_message *read = mx_read_message_get(id);

    if (!read) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Message not found");
        return;
    }
    t_string json_string = mx_read_message_stringify(read);

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string);
    mx_strdel(&json_string);
    mx_delete_read_message(read);
}

void mx_message_ctrl_get_reads(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);

    if (id <= 0) {
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

    if (!reads) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Message not found");
        return;
    }
    t_string json_string = mx_read_messages_stringify(reads);

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string);
    mx_strdel(&json_string);
    mx_delete_list(&reads, (t_func_void)mx_delete_read_message);
}
