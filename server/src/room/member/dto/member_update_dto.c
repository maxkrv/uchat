#include "room.h"

static t_room_member_update_dto *init_room_member_update_dto() {
    t_room_member_update_dto *dto =
        (t_room_member_update_dto *)malloc(sizeof(t_room_member_update_dto));
    dto->is_admin = false;

    return dto;
}

void mx_room_member_update_dto_free(t_room_member_update_dto *dto) {
    if (!dto) {
        return;
    }

    free(dto);
}

t_room_member_update_dto *mx_parse_room_member_update_dto(struct mg_str body) {
    t_room_member_update_dto *dto = init_room_member_update_dto();
    cJSON *obj = cJSON_ParseWithLength(body.ptr, body.len);

    if (!obj) {
        free(dto);
        return NULL;
    }

    dto->is_admin = mx_cjson_get_bool(obj, "is_admin");

    cJSON_Delete(obj);

    return dto;
}
