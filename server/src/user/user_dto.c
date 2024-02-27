#include "user.h"

static t_user_create_dto *init_user_create_dto() {
    t_user_create_dto *dto =
        (t_user_create_dto *)malloc(sizeof(t_user_create_dto));
    dto->name = NULL;
    dto->password = NULL;
    dto->photo_url = NULL;
    dto->tag = NULL;

    return dto;
}

t_user_create_dto *mx_parse_user_create_dto(t_string body) {
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

    key = cJSON_GetObjectItemCaseSensitive(obj, "photo_url");
    dto->photo_url = cJSON_GetStringValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "tag");
    dto->tag = cJSON_GetStringValue(key);

    cJSON_Delete(obj);
    return dto;
}
