#include "server.h"

void mx_room_ctrl_pine_message(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);
    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int message_id = mx_extract_id_from_query(m->query, "message_id");
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (message_id <= 0 || room_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    if (!mx_is_user_member_of(room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        return;
    }
    t_pined_message *pine = mx_room_pine_message(message_id, room_id);

    if (!pine) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant pine message");
        return;
    }

    t_string json_string = mx_pined_message_stringify(pine);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_ws_emit("message-pinned", pine->room_id,
               mx_pined_message_to_cjson(pine));
    mx_strdel(&json_string);
    mx_room_pined_free(pine);
}

void mx_room_ctrl_get_pined(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);
    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (room_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid data provided");
        return;
    }
    if (!mx_is_user_member_of(room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        return;
    }
    t_list *pines = mx_room_get_pined_messages(room_id); // <<<<< misha proebalsya
    t_string json_string = mx_pined_messages_stringify(pines);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON,
                  json_string ? json_string : "[]");
    mx_strdel(&json_string);
    mx_list_free(&pines, (t_func_free)mx_room_pined_free);
}

void mx_room_ctrl_unpine(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);
    if (user_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int pined_id = mx_extract_id_from_query(m->query, "pined_id");

    if (pined_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    t_pined_message *message = mx_pined_repo_get(pined_id);
    if (!message) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Message not found");
        return;
    }
    if (!mx_is_user_member_of(message->room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        mx_room_pined_free(message);
        return;
    }
    mx_room_pined_free(message);

    t_pined_message *pine = mx_room_unpine(pined_id);

    if (!pine) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant unpine message");
        return;
    }

    t_string json_string = mx_pined_message_stringify(pine);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_ws_emit("message-unpinned", pine->room_id,
               mx_pined_message_to_cjson(pine));
    mx_strdel(&json_string);
    mx_room_pined_free(pine);
}
