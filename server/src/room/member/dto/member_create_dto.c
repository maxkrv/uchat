#include "room.h"

static t_room_member_create_dto *init_room_member_create_dto() {
    t_room_member_create_dto *dto =
        (t_room_member_create_dto *)malloc(sizeof(t_room_member_create_dto));
    dto->room_id = 0;
    dto->user_id = 0;
    dto->is_admin = false;

    return dto;
}

t_room_member_create_dto *
mx_room_member_create_dto_constructor(int room_id, int user_id,
                                      bool is_admin) {
    t_room_member_create_dto *dto =
        (t_room_member_create_dto *)malloc(sizeof(t_room_member_create_dto));
    dto->room_id = room_id;
    dto->user_id = user_id;
    dto->is_admin = is_admin;

    return dto;
}

void mx_room_member_create_dto_free(t_room_member_create_dto *dto) {
    if (!dto) {
        return;
    }

    free(dto);
}

static t_room_member_create_dto *
validate_room_member_create_dto(t_room_member_create_dto *dto) {
    if (dto->room_id <= 0 || dto->user_id <= 0) {
        mx_room_member_create_dto_free(dto);
        return NULL;
    }

    return dto;
}

static t_room_member_create_dto *
parse_room_member_create_dto(struct mg_str body) {
    t_room_member_create_dto *dto = init_room_member_create_dto();
    cJSON *obj = cJSON_ParseWithLength(body.ptr, body.len);

    if (!obj) {
        free(dto);
        return NULL;
    }

    dto->room_id = mx_cjson_get_number(obj, "room_id");
    dto->user_id = mx_cjson_get_number(obj, "user_id");
    dto->is_admin = mx_cjson_get_bool(obj, "is_admin");

    cJSON_Delete(obj);

    return dto;
}

t_room_member_create_dto *mx_room_member_create_dto_get(struct mg_str body) {
    t_room_member_create_dto *dto = parse_room_member_create_dto(body);

    if (!dto) {
        return NULL;
    }

    return validate_room_member_create_dto(dto);
}
