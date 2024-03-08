#include "server.h"

void mx_user_ctrl_get(t_connection *c, t_http_message *m) {
    t_user *user;
    int id = mx_extract_id_from_query(m->query, "id");
    t_string tag =
        mx_decode_uri_component(mg_http_var(m->query, mg_str_s("tag")));

    if (id > 0) {
        user = mx_user_get_discription(id);
    } else if (tag) {
        user = mx_user_get_by_tag(tag);
    } else {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid user data provided");
        return;
    }

    mx_strdel(&tag);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "User not found");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_user(user);
}

void mx_user_ctrl_get_me(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);

    if (id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid token provided");
        return;
    }

    t_user *user = mx_user_get(id);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "User not found");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_user(user);
}

void mx_user_ctrl_get_my_rooms(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);

    if (id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid token provided");
        return;
    }

    t_list *rooms = mx_user_get_rooms(id);

    if (!rooms) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Rooms not found");
        return;
    }

    t_string json_string = mx_rooms_stringify(rooms);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_list(&rooms, (t_func_void)mx_delete_room);
}

void mx_user_ctrl_put_me(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);

    if (id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    t_user_update_dto *dto = mx_get_user_update_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid user data provided");
        return;
    }

    t_user *user = mx_user_put(id, dto);

    mx_delete_user_update_dto(dto);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Cant update user");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_user(user);
}

void mx_user_ctrl_delete_me(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);

    if (id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }

    t_user *user = mx_user_delete(id);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "User not found");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_user(user);
}
