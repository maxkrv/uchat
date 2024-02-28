#include "room.h"

static t_room_create_dto *init_room_create_dto() {
    t_room_create_dto *dto =
        (t_room_create_dto *)malloc(sizeof(t_room_create_dto));
    dto->name = NULL;
    dto->discription = NULL;
    dto->photo_id = 0;
    dto->type = NULL;

    return dto;
}
static t_room_create_dto *validate_room_create_dto(t_room_create_dto *dto) {
    if (!dto->name || !dto->type || dto->photo_id < 0 ||
        mx_strlen(dto->name) < 4 || mx_strlen(dto->type) < 4) {
        free(dto);
        return NULL;
    }

    return dto;
}

static t_room_create_dto *parse_room_create_dto(t_string body) {
    t_room_create_dto *dto = init_room_create_dto();
    cJSON *obj = cJSON_ParseWithLength(body.ptr, body.len);
    cJSON *key;

    if (!obj) {
        free(dto);
        return NULL;
    }

    key = cJSON_GetObjectItemCaseSensitive(obj, "name");
    dto->name = cJSON_GetStringValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "discription");
    dto->discription = cJSON_GetStringValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "photo_url");
    dto->photo_id = cJSON_GetNumberValue(key);

    cJSON_Delete(obj);

    return dto;
}

t_room_create_dto *mx_get_room_create_dto(t_string body) {
    t_room_create_dto *dto = parse_room_create_dto(body);

    if (!dto) {
        return NULL;
    }

    return validate_room_create_dto(dto);
}
