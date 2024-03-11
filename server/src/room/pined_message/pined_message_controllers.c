#include "room.h"

void mx_room_ctrl_pine_message(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_user_id_from_auth_jwt(m);
    int message_id = mx_extract_id_from_query(m->query, "message_id");
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (user_id <= 0 || message_id <= 0 || room_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    if (!mx_is_user_member_of(room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        return;
    }
    t_list *pines = mx_room_pine_message(message_id, room_id);

    if (!pines) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant pine message");
        return;
    }

    t_string json_string = mx_pined_messages_stringify(pines);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_list(&pines, (t_func_void)mx_delete_room_pined);
}

void mx_room_ctrl_get_pined(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_user_id_from_auth_jwt(m);
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (user_id <= 0 || room_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token provided");
        return;
    }
    if (!mx_is_user_member_of(room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        return;
    }
    t_list *pines = mx_room_get_pined_messages(user_id);

    if (!pines) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant get pined messages");
        return;
    }

    t_string json_string = mx_pined_messages_stringify(pines);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_list(&pines, (t_func_void)mx_delete_room_pined);
}

void mx_room_ctrl_unpine(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_user_id_from_auth_jwt(m);
    int pined_id = mx_extract_id_from_query(m->query, "pined_id");

    if (user_id < 0 || pined_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    t_room_pined_message *message = mx_pined_repo_get(pined_id);

    if (!mx_is_user_member_of(message->room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        mx_delete_room_pined(message);
        return;
    }
    mx_delete_room_pined(message);

    t_list *pines = mx_room_unpine(pined_id);

    if (!pines) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "No pined messages found");
        return;
    }

    t_string json_string = mx_pined_messages_stringify(pines);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_list(&pines, (t_func_void)mx_delete_room_pined);
}
