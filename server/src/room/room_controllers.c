#include "room.h"

void mx_room_ctrl_get(t_connection *c, t_http_message *m) {
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (room_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room id provided");
        return;
    }

    t_room *room = mx_room_get(room_id);

    if (!room) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "Room not found");
        return;
    }

    t_string json_string = mx_room_stringify(room);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_room(room);
}

void mx_room_ctrl_post(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_user_id_from_auth_jwt(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token provided");
        return;
    }
    t_room_create_dto *dto = mx_get_room_create_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room data provided");
        return;
    }
    t_room *room = mx_room_create(dto, user_id);

    mx_delete_room_create_dto(dto);
    if (!room) {
        mx_http_reply_exception(c, m, HTTP_STATUS_INTERNAL_SERVER_ERROR,
                                "Cant create room");
        return;
    }

    t_string json_string = mx_room_stringify(room);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_room(room);
}

void mx_room_ctrl_put(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_user_id_from_auth_jwt(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token provided");
        return;
    }
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (room_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room id provided");
        return;
    }
    t_room_create_dto *dto = mx_get_room_create_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room data provided");
        return;
    }
    if (!mx_is_room_admin(room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        mx_delete_room_create_dto(dto);
        return;
    }
    t_room *room = mx_room_put(room_id, dto);

    mx_delete_room_create_dto(dto);
    if (!room) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Cant update room");
        return;
    }

    t_string json_string = mx_room_stringify(room);
    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_room(room);
}

void mx_room_ctrl_delete(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_user_id_from_auth_jwt(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token provided");
        return;
    }
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (room_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room id provided");
        return;
    }
    if (!mx_is_room_admin(room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        return;
    }

    t_room *room = mx_room_delete(room_id);

    if (!room) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "Room not found");
        return;
    }

    t_string json_string = mx_room_stringify(room);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_room(room);
}
