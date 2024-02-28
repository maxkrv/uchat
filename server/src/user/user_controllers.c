#include "user.h"

void mx_user_ctrl_get(t_connection *c, t_http_message *m) {
    int user_id = mx_extract_id_from_query(m->query);

    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid user id provided");
        return;
    }

    t_user *user = mx_user_get(user_id);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "User not found");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
}

void mx_user_ctrl_post(t_connection *c, t_http_message *m) {
    t_user_create_dto *dto = mx_get_user_create_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid user data provided");
        return;
    }
    t_user *user = mx_user_create(dto);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_INTERNAL_SERVER_ERROR,
                                "Cant create user");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    free(dto);
    mg_http_reply(c, HTTP_STATUS_CREATED, MX_EMPTY, json_string.ptr);
}

void mx_user_ctrl_put(t_connection *c, t_http_message *m) {
    int user_id = mx_parse_jwt_auth_token(m);
    t_user_create_dto *dto = mx_get_user_create_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid user data provided");
        return;
    }

    t_user *user = mx_user_put(user_id, dto);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Cant update user");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    free(dto);
    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
}

void mx_user_ctrl_delete(t_connection *c, t_http_message *m) {
    int user_id = mx_extract_id_from_query(m->query);

    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid user id provided");
        return;
    }

    t_user *user = mx_user_delete(user_id);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "User not found");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
}
