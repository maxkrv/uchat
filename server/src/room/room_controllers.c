#include "room.h"

void mx_room_ctrl_get(t_connection *c, t_http_message *m) {
    int room_id = mx_extract_id_from_query(m->query);

    if (room_id < 0) {
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

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
}

void mx_room_ctrl_post(t_connection *c, t_http_message *m) {
    t_room_create_dto *dto = mx_get_room_create_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room data provided");
        return;
    }
    t_room *room = mx_room_create(dto);

    if (!room) {
        mx_http_reply_exception(c, m, HTTP_STATUS_INTERNAL_SERVER_ERROR,
                                "Cant create room");
        return;
    }

    t_string json_string = mx_room_stringify(room);

    free(dto);
    mg_http_reply(c, HTTP_STATUS_CREATED, MX_EMPTY, json_string.ptr);
}

void mx_room_ctrl_put(t_connection *c, t_http_message *m) {
    int room_id = mx_extract_id_from_query(m->query);
    t_room_create_dto *dto = mx_get_room_create_dto(m->body);

    if (room_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room id provided");
        return;
    }
    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room data provided");
        return;
    }

    t_room *room = mx_room_put(room_id, dto);

    if (!room) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Cant update room");
        return;
    }

    t_string json_string = mx_room_stringify(room);

    free(dto);
    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
}

void mx_room_ctrl_delete(t_connection *c, t_http_message *m) {
    int room_id = mx_extract_id_from_query(m->query);

    if (room_id < 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room id provided");
        return;
    }

    t_room *room = mx_room_delete(room_id);

    if (!room) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND, "Room not found");
        return;
    }

    t_string json_string = mx_room_stringify(room);

    mg_http_reply(c, HTTP_STATUS_OK, MX_EMPTY, json_string.ptr);
}
