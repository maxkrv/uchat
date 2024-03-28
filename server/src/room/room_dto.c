#include "room.h"

t_room_create_dto *mx_room_create_dto_init(void) {
    t_room_create_dto *dto =
        (t_room_create_dto *)malloc(sizeof(t_room_create_dto));
    dto->name = NULL;
    dto->description = NULL;
    dto->photo_id = 0;
    dto->type = NULL;

    return dto;
}

void mx_room_create_dto_free(t_room_create_dto *dto) {
    if (!dto) {
        return;
    }
    mx_strdel(&dto->name);
    mx_strdel(&dto->description);
    mx_strdel(&dto->type);

    free(dto);
}

static t_room_create_dto *validate_room_create_dto(t_room_create_dto *dto) {
    if (!dto->name || mx_strlen(dto->name) < 4) {
        mx_room_create_dto_free(dto);
        return NULL;
    }

    return dto;
}

static t_room_create_dto *parse_room_create_dto(struct mg_str body) {
    t_room_create_dto *dto = mx_room_create_dto_init();
    cJSON *obj = cJSON_ParseWithLength(body.ptr, body.len);

    if (!obj) {
        free(dto);
        return NULL;
    }

    dto->name = mx_cjson_get_string(obj, "name");
    dto->description = mx_cjson_get_string(obj, "description");
    dto->photo_id = mx_cjson_get_number(obj, "photo_id");

    cJSON_Delete(obj);

    return dto;
}

t_room_create_dto *mx_room_create_dto_get(struct mg_str body) {
    t_room_create_dto *dto = parse_room_create_dto(body);

    if (!dto) {
        return NULL;
    }

    return validate_room_create_dto(dto);
}
