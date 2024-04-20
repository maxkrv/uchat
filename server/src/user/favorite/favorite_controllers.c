#include "user.h"
#include "auth.h"

void mx_user_ctrl_add_favorite(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (room_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }

    t_favorite_room *favs = mx_user_add_favorite(user_id, room_id);

    if (!favs) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant add favorite");
        return;
    }

    t_string json_string = mx_favorite_stringify(favs);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_favorite_room_free(favs);
}

void mx_user_ctrl_get_favorites(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }

    t_list *favs = mx_user_get_favorites(user_id);
    t_string json_string = mx_favorites_stringify(favs);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON,
                  json_string ? json_string : "[]");
    mx_strdel(&json_string);
    mx_list_free(&favs, (t_func_free)mx_favorite_room_free);
}

void mx_user_ctrl_delete_favorite(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }

    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (room_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }

    t_favorite_room *favs = mx_user_delete_favorite(room_id, user_id);

    if (!favs) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "User not found");
        return;
    }

    t_string json_string = mx_favorite_stringify(favs);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_favorite_room_free(favs);
}
