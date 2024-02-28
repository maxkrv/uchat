#include "user.h"

static t_user_create_dto *init_user_create_dto() {
    t_user_create_dto *dto =
        (t_user_create_dto *)malloc(sizeof(t_user_create_dto));
    dto->name = NULL;
    dto->password = NULL;
    dto->photo_id = 0;
    dto->tag = NULL;
    dto->status = NULL;
    dto->description = NULL;

    return dto;
}
static t_user_create_dto *validate_user_create_dto(t_user_create_dto *dto) {
    if (!dto->name || !dto->password || !dto->tag || !dto->status ||
        dto->photo_id < 0 || mx_strlen(dto->name) < 4 ||
        mx_strlen(dto->password) < 4 || mx_strlen(dto->status) < 4 ||
        mx_strlen(dto->tag) < 4) {
        free(dto);
        return NULL;
    }

    return dto;
}

static t_user_create_dto *parse_user_create_dto(t_string body) {
    t_user_create_dto *dto = init_user_create_dto();
    cJSON *obj = cJSON_ParseWithLength(body.ptr, body.len);
    cJSON *key;

    if (!obj) {
        free(dto);
        return NULL;
    }

    key = cJSON_GetObjectItemCaseSensitive(obj, "name");
    dto->name = cJSON_GetStringValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "password");
    dto->password = cJSON_GetStringValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "photo_id");
    dto->photo_id = cJSON_GetNumberValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "tag");
    dto->tag = cJSON_GetStringValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "description");
    dto->description = cJSON_GetStringValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "status");
    dto->status = cJSON_GetStringValue(key);

    cJSON_Delete(obj);

    return dto;
}

t_user_create_dto *mx_get_user_create_dto(t_string body) {
    t_user_create_dto *dto = parse_user_create_dto(body);

    if (!dto) {
        return NULL;
    }

    return validate_user_create_dto(dto);
}
