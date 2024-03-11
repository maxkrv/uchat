#include "auth.h"

void mx_auth_ctrl_login(t_connection *c, t_http_message *m) {
    t_login_dto *dto = mx_get_login_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    t_jwt_token token = mx_auth_login(dto);

    mx_delete_login_dto(dto);

    if (!token) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Cant login user");
        return;
    }

    t_string json_string = mx_token_stringify(token);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&token);
    mx_strdel(&json_string);
}

void mx_auth_ctrl_change_password(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_user_id_from_auth_jwt(m);

    if (!user_id) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token provided");
        return;
    }
    t_change_password_dto *dto = mx_get_change_password_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    t_user *user = mx_auth_change_password(user_id, dto);

    mx_delete_change_password_dto(dto);
    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Cant change password");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_delete_user(user);
    mx_strdel(&json_string);
}

void mx_auth_ctrl_register(t_connection *c, t_http_message *m) {
    t_register_dto *dto = mx_get_user_create_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    t_user *user = mx_auth_register(dto);

    mx_delete_user_create_dto(dto);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Cant register user");
        return;
    }

    t_string json_string = mx_user_stringify(user);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_delete_user(user);
    mx_strdel(&json_string);
}
