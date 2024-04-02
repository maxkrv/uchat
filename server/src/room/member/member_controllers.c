#include "server.h"

void mx_room_ctrl_get_member(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int member_id = mx_extract_id_from_query(m->query, "member_id");

    if (member_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    t_room_member *member = mx_room_get_member(member_id);

    if (!member) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Member does not exist");
        return;
    }
    t_string json_string = mx_member_stringify(member);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_room_member_free(member);
}
void mx_room_ctrl_get_members(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
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
    t_list *members = mx_room_get_members(room_id);
    t_string json_string = mx_members_stringify(members);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON,
                  json_string ? json_string : "[]");
    mx_strdel(&json_string);
    mx_list_free(&members, (t_func_free)mx_room_member_free);
}

void mx_room_ctrl_add_member(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    t_room_member_create_dto *dto = mx_room_member_create_dto_get(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    if (!mx_is_user_member_of(dto->room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN,
                                "User has no permissions");
        mx_room_member_create_dto_free(dto);
        return;
    }
    t_room_member *mem = mx_room_add_member(dto);

    mx_room_member_create_dto_free(dto);
    if (!mem) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant add member");
        return;
    }

    t_string json_string = mx_member_stringify(mem);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_push_client_room(mem->user_id, mx_room_get(mem->room_id));
    mx_ws_emit("member-joined", mem->room_id, mx_member_to_cjson(mem));
    mx_strdel(&json_string);
    mx_room_member_free(mem);
}

void mx_room_ctrl_update_member(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int member_id = mx_extract_id_from_query(m->query, "member_id");

    if (member_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid member id provided");
        return;
    }
    t_room_member *member = mx_room_get_member(member_id);
    if (!member) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Member not found");
        return;
    }
    if (!mx_is_room_admin(member->room_id, user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN,
                                "User has no permissions");
        mx_room_member_free(member);
        return;
    }
    t_room_member_update_dto *dto = mx_parse_room_member_update_dto(m->body);

    if (!dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    mx_room_member_free(member);

    t_room_member *mem = mx_room_update_member(member_id, dto);

    mx_room_member_update_dto_free(dto);

    if (!mem) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant update member");
        return;
    }

    t_string json_string = mx_member_stringify(mem);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_ws_emit("member-updated", mem->room_id, mx_member_to_cjson(mem));
    mx_strdel(&json_string);
    mx_room_member_free(mem);
}

void mx_room_ctrl_delete_member(t_connection *c, t_http_message *m) {
    t_user_id user_id = mx_auth(m);

    if (user_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token");
        return;
    }
    int member_id = mx_extract_id_from_query(m->query, "member_id");

    if (member_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    t_room_member *member = mx_room_get_member(member_id);

    if (!member) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Member does not exist");
        return;
    }
    if (!mx_is_room_admin(member->room_id, member->user_id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN,
                                "User is not a member of room");
        mx_room_member_free(member);
        return;
    }
    mx_room_member_free(member);
    t_room_member *mem = mx_room_delete_member(member_id);

    if (!mem) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant delete member");
        return;
    }

    t_string json_string = mx_member_stringify(mem);

    mg_http_reply(c, HTTP_STATUS_OK, MX_HEADERS_JSON, json_string);
    mx_ws_emit("member-left", mem->room_id, mx_member_to_cjson(mem));
    mx_delete_client_room(mem->user_id, mem->room_id);
    mx_strdel(&json_string);
    mx_room_member_free(mem);
}
