#include "server.h"

void mx_user_ctrl_get(t_connection *c, t_http_message *m) {
    t_user *user;
    int user_id = mx_extract_id_from_query(m->query, "user_id");
    t_string tag =
        mx_decode_uri_component(mg_http_var(m->query, mg_str_s("tag")));
    t_string name =
        mx_decode_uri_component(mg_http_var(m->query, mg_str_s("name")));
    if (user_id > 0) {
        user = mx_user_get(user_id);
    } else if (tag) {
        user = mx_user_get_by_tag(tag);
    } else if (name) {
        user = mx_user_get_by_name(name);
    } else {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid user data provided");
        return;
    }

    mx_strdel(&tag);
    mx_strdel(&name);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "User not found");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_user_free(user);
}

void mx_user_ctrl_get_me(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }

    t_user *user = mx_user_get(user_id);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "User not found");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_user_free(user);
}

void mx_user_ctrl_get_my_rooms(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }

    t_list *rooms = mx_user_get_rooms(user_id);
    t_string json_string = mx_rooms_stringify(rooms);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON,
                  json_string ? json_string : "[]");
    mx_strdel(&json_string);
    mx_list_free(&rooms, (t_func_free)mx_room_free);
}

void mx_user_ctrl_put_me(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    t_user_update_dto *dto = mx_user_update_dto_get(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid user data provided");
        return;
    }

    t_user *user = mx_user_put(user_id, dto);

    mx_user_update_dto_free(dto);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Cant update user");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_user_free(user);
}

void mx_user_ctrl_delete_me(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }

    t_user *user = mx_user_delete(user_id);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "User not found");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_user_free(user);
}
