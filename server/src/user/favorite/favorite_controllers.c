#include "user.h"
#include "auth.h"

void mx_user_ctrl_add_favorite(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (id <= 0 || room_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }

    t_list *favs = mx_user_add_favorite(id, room_id);

    if (!favs) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant add favorite");
        return;
    }

    t_string json_string = mx_favorites_stringify(favs);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS, json_string);
    mx_strdel(&json_string);
    mx_delete_list(&favs, (t_func_void)mx_delete_favorite_room);
}

void mx_user_ctrl_get_favorites(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);

    if (id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token provided");
        return;
    }

    t_list *favs = mx_user_get_favorites(id);

    if (!favs) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant get favorites");
        return;
    }

    t_string json_string = mx_favorites_stringify(favs);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS, json_string);
    mx_strdel(&json_string);
    mx_delete_list(&favs, (t_func_void)mx_delete_favorite_room);
}

void mx_user_ctrl_delete_favorite(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);
    int favorite_id = mx_extract_id_from_query(m->query, "favorite_id");

    if (id < 0 || favorite_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }

    t_list *favs = mx_user_delete_favorite(favorite_id);

    if (!favs) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "User not found");
        return;
    }

    t_string json_string = mx_favorites_stringify(favs);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS, json_string);
    mx_strdel(&json_string);
    mx_delete_list(&favs, (t_func_void)mx_delete_favorite_room);
}
