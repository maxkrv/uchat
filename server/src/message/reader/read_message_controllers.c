#include "server.h"

static t_list *bind_columns_to_messages(sqlite3_stmt *stmt) {
    t_list *messages = NULL;
    t_message *last = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!last || sqlite3_column_int(stmt, 0) != last->id) {
            last = mx_sqlite_bind_columns_to_message(stmt, 0);
            last->author = mx_sqlite_bind_columns_to_user(stmt, 12);
            last->author->photo = mx_sqlite_bind_columns_to_file(stmt, 21);
            mx_push_back(&messages, last);
        }
        if (sqlite3_column_int(stmt, 7) > 0) {
            mx_push_back(&last->files,
                         mx_sqlite_bind_columns_to_file(stmt, 7));
        }
    }

    return messages;
}

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
                                "Message not found");
        return;
    }
    t_string json_string = mx_read_message_stringify(read);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_ws_emit("message-was-readed", read->message->room_id,
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

    int message_id = mx_extract_id_from_query(m->query, "message_id");

    if (message_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid message id");
        return;
    }

    t_read_message *read = mx_read_message_get(user_id);

    if (!read) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Message not found");
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

    if (!reads) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Message not found");
        return;
    }
    t_string json_string = mx_read_messages_stringify(reads);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_list_free(&reads, (t_func_free)mx_read_message_free);
}
