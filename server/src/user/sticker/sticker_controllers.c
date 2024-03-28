#include "server.h"

void mx_user_ctrl_get_stickers(t_connection *c, t_http_message *m) {
    int user_id = mx_auth(m);
    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    t_list *stickers = mx_stickers_get(user_id);
    t_string json_string = mx_files_stringify(stickers);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON,
                  json_string ? json_string : "[]");
    mx_strdel(&json_string);
    mx_list_free(&stickers, (t_func_free)mx_file_free);
}

void mx_user_ctrl_post_sticker(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int file_id = mx_extract_id_from_query(m->query, "file_id");
    if (file_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid user data provided");
        return;
    }
    t_file *sticker = mx_sticker_add(user_id, file_id);

    if (!sticker) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant add sticker");
        return;
    }
    t_string json_string = mx_file_stringify(sticker);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_file_free(sticker);
}

void mx_user_ctrl_delete_sticker(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int file_id = mx_extract_id_from_query(m->query, "file_id");
    if (file_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid user data provided");
        return;
    }
    t_file *sticker = mx_sticker_delete(user_id, file_id);

    if (!sticker) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant delete sticker");
        return;
    }
    t_string json_string = mx_file_stringify(sticker);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_file_free(sticker);
}
