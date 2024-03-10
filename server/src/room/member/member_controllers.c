#include "room.h"

void mx_room_ctrl_get_member(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);
    int member_id = mx_extract_id_from_query(m->query, "member_id");

    if (id <= 0 || member_id <= 0) {
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
    t_string json_string = mx_room_member_stringify(member);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_room_member(member);
}
void mx_room_ctrl_get_members(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);
    int room_id = mx_extract_id_from_query(m->query, "room_id");

    if (id <= 0 || room_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token provided");
        return;
    }
    t_list *members = mx_room_get_members(room_id);

    if (!members) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Members does not exist");
        return;
    }
    t_string json_string = mx_room_members_stringify(members);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_list(&members, (t_func_void)mx_delete_room_member);
}

void mx_room_ctrl_add_member(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);
    t_room_member_create_dto *dto = mx_get_room_member_create_dto(m->body);

    if (id <= 0 || !dto) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    dto->is_admin = false;
    if (!mx_is_user_member_of(dto->room_id, id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN,
                                "User has no permissions");
        mx_delete_room_member_create_dto(dto);
        return;
    }
    t_room_member *mem = mx_room_add_member(dto);

    if (!mem) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant add member");
        mx_delete_room_member_create_dto(dto);
        mx_delete_room_member(mem);
        return;
    }

    t_string json_string = mx_room_member_stringify(mem);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_room_member_create_dto(dto);
    mx_delete_room_member(mem);
}

void mx_room_ctrl_update_member(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);
    int member_id = mx_extract_id_from_query(m->query, "member_id");
    t_room_member_update_dto *dto = mx_parse_room_member_update_dto(m->body);

    if (id <= 0 || !dto || member_id <= 0) {
        mx_http_reply_exception(c, m, HTTP_STATUS_UNPROCESSABLE_ENTITY,
                                "Invalid data provided");
        return;
    }
    t_room_member *member = mx_room_get_member(member_id);
    if (!member) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Member not found");
        mx_delete_room_member_update_dto(dto);
        return;
    }
    if (!mx_is_room_admin(member->room_id, id)) {
        mx_http_reply_exception(c, m, HTTP_STATUS_FORBIDDEN,
                                "User has no permissions");
        mx_delete_room_member_update_dto(dto);
        mx_delete_room_member(member);
        return;
    }
    mx_delete_room_member(member);

    t_room_member *mem = mx_room_update_member(member_id, dto);

    mx_delete_room_member_update_dto(dto);

    if (!mem) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant add member");
        return;
    }

    t_string json_string = mx_room_member_stringify(mem);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_room_member(mem);
}

void mx_room_ctrl_delete_member(t_connection *c, t_http_message *m) {
    t_user_id id = mx_user_id_from_auth_jwt(m);
    int member_id = mx_extract_id_from_query(m->query, "member_id");

    if (id <= 0 || member_id <= 0) {
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
        mx_delete_room_member(member);
        return;
    }
    mx_delete_room_member(member);
    t_room_member *mem = mx_room_delete_member(member_id);

    if (!mem) {
        mx_http_reply_exception(c, m, HTTP_STATUS_NOT_FOUND,
                                "Cant add member");
        return;
    }

    t_string json_string = mx_room_member_stringify(mem);

    mg_http_reply(c, HTTP_STATUS_CREATED, MX_HEADERS_JSON, json_string);
    mx_strdel(&json_string);
    mx_delete_room_member(mem);
}
