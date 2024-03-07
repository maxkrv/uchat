#include "auth.h"

static t_change_password_dto *init_change_password_dto() {
    t_change_password_dto *dto =
        (t_change_password_dto *)malloc(sizeof(t_change_password_dto));
    dto->old_password = NULL;
    dto->new_password = NULL;

    return dto;
}

void mx_delete_change_password_dto(t_change_password_dto *dto) {
    if (!dto) {
        return;
    }
    mx_strdel(&dto->new_password);
    mx_strdel(&dto->old_password);

    free(dto);
}

static t_change_password_dto *
validate_change_password_dto(t_change_password_dto *dto) {
    if (!dto->new_password || !dto->old_password ||
        !mx_is_valid_password(dto->old_password) ||
        !mx_is_valid_password(dto->new_password)) {

        mx_delete_change_password_dto(dto);
        return NULL;
    }

    return dto;
}

static t_change_password_dto *parse_change_password_dto(struct mg_str body) {
    t_change_password_dto *dto = init_change_password_dto();
    cJSON *obj = cJSON_ParseWithLength(body.ptr, body.len);
    cJSON *key;

    if (!obj) {
        free(dto);
        return NULL;
    }

    key = cJSON_GetObjectItemCaseSensitive(obj, "old_password");
    dto->old_password = cJSON_GetStringValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "new_password");
    dto->new_password = cJSON_GetStringValue(key);

    cJSON_Delete(obj);

    return dto;
}

t_change_password_dto *mx_get_change_password_dto(struct mg_str body) {
    t_change_password_dto *dto = parse_change_password_dto(body);

    if (!dto) {
        return NULL;
    }

    return validate_change_password_dto(dto);
}
