#include "server.h"

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
    mx_room_free(room);
}

void mx_room_ctrl_post(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    t_room_create_dto *dto = mx_room_create_dto_get(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room data provided");
        return;
    }
    dto->type = mx_strdup(MX_ROOM_TYPE_GROUP);

    t_room *room = mx_room_create(dto, user_id);

    mx_room_create_dto_free(dto);
    if (!room) {
        mx_http_reply_exception(c, m, HTTP_STATUS_INTERNAL_SERVER_ERROR,
                                "Cant create room");
        return;
    }

    t_string json_string = mx_room_stringify(room);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_push_client_room(user_id, room);
    mx_strdel(&json_string);
}

void mx_room_direct_ctrl_post(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    t_string username =
        mx_decode_uri_component(mg_http_var(m->query, mg_str_s("username")));

    if (!username) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid username provided");
        return;
    }

    t_room_create_dto *dto = mx_room_create_dto_get(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room data provided");
        mx_strdel(&username);
        return;
    }
    dto->type = mx_strdup(MX_ROOM_TYPE_DIRECT);
    t_user *user = mx_user_get_by_name(username);

    if (!user) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid username provided");
        mx_room_create_dto_free(dto);
        mx_strdel(&username);
        return;
    }
    t_room *room = mx_room_create_direct(dto, user_id, user->id);

    mx_room_create_dto_free(dto);
    mx_strdel(&username);
    if (!room) {
        mx_http_reply_exception(c, m, HTTP_STATUS_INTERNAL_SERVER_ERROR,
                                "Cant create direct room");
        return;
    }
    int member2_id = mx_members_repo_create(user->id, room->id, true);

    if (member2_id <= 0) {
        mx_room_repo_delete(room->id);
        mx_http_reply_exception(c, m, HTTP_STATUS_INTERNAL_SERVER_ERROR,
                                "Cant add member to direct room");
        return;
    }
    t_room_member *mem = mx_members_repo_get(member2_id);

    t_string json_string = mx_room_stringify(room);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_push_client_room(user_id, room);
    mx_push_client_room(user->id, room);
    mx_ws_emit("member-joined", mem->room_id, mx_member_to_cjson(mem));
    mx_room_member_free(mem);
    mx_strdel(&json_string);
}

void mx_room_ctrl_put(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (room_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room id provided");
        return;
    }
    t_room_create_dto *dto = mx_room_create_dto_get(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid room data provided");
        return;
    }
    if (!mx_is_room_admin(room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN, "No permissions");
        mx_room_create_dto_free(dto);
        return;
    }
    t_room *room = mx_room_put(room_id, dto);

    mx_room_create_dto_free(dto);
    if (!room) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Cant update room");
        return;
    }

    t_string json_string = mx_room_stringify(room);
    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_ws_emit("room-updated", room->id, mx_room_to_cjson(room));
    mx_strdel(&json_string);
    mx_room_free(room);
}

void mx_room_ctrl_delete(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
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
    mx_ws_emit("room-deleted", room->id, mx_room_to_cjson(room));
    mx_delete_clients_room(room->id);
    mx_strdel(&json_string);
    mx_room_free(room);
}
