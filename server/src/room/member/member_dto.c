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

void mx_delete_room_member_create_dto(t_room_member_create_dto *dto) {
    if (!dto) {
        return;
    }

    free(dto);
}

static t_room_member_create_dto *
validate_room_member_create_dto(t_room_member_create_dto *dto) {
    if (dto->room_id <= 0 || dto->user_id <= 0) {
        free(dto);
        return NULL;
    }

    return dto;
}

static t_room_member_create_dto *
parse_room_member_create_dto(struct mg_str body) {
    t_room_member_create_dto *dto = init_room_member_create_dto();
    cJSON *obj = cJSON_ParseWithLength(body.ptr, body.len);
    cJSON *key;

    if (!obj) {
        free(dto);
        return NULL;
    }

    key = cJSON_GetObjectItemCaseSensitive(obj, "room_id");
    dto->room_id = cJSON_GetNumberValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "user_id");
    dto->user_id = cJSON_GetNumberValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "is_admin");
    if (cJSON_IsBool(key)) {
        dto->is_admin = key->valueint;
    }
    cJSON_Delete(obj);

    return dto;
}

t_room_member_create_dto *mx_get_room_member_create_dto(struct mg_str body) {
    t_room_member_create_dto *dto = parse_room_member_create_dto(body);

    if (!dto) {
        return NULL;
    }

    return validate_room_member_create_dto(dto);
}
