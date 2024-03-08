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

void mx_delete_user_create_dto(t_user_create_dto *dto) {
    if (!dto) {
        return;
    }

    mx_strdel(&dto->name);
    mx_strdel(&dto->password);
    mx_strdel(&dto->tag);
    mx_strdel(&dto->status);
    mx_strdel(&dto->description);

    free(dto);
}

static t_user_create_dto *validate_user_create_dto(t_user_create_dto *dto) {
    if (!dto->name || !dto->password || !dto->tag || !dto->status ||
        dto->photo_id < 0 || mx_strlen(dto->name) < 4 ||
        mx_strlen(dto->password) < 4 || mx_strlen(dto->status) < 4 ||
        mx_strlen(dto->tag) < 4) {
        mx_delete_user_create_dto(dto);
        return NULL;
    }

    return dto;
}

static t_user_create_dto *parse_user_create_dto(struct mg_str body) {
    t_user_create_dto *dto = init_user_create_dto();
    cJSON *obj = cJSON_ParseWithLength(body.ptr, body.len);
    cJSON *key;

    if (!obj) {
        free(dto);
        return NULL;
    }

    dto->name = mx_cjson_get_string(obj, "name");
    dto->password = mx_cjson_get_string(obj, "password");
    dto->photo_id = mx_cjson_get_number(obj, "photo_id");
    dto->tag = mx_cjson_get_string(obj, "tag");
    dto->status = mx_cjson_get_string(obj, "status");

    cJSON_Delete(obj);

    return dto;
}

t_user_create_dto *mx_get_user_create_dto(struct mg_str body) {
    t_user_create_dto *dto = parse_user_create_dto(body);
    if (!dto) {
        return NULL;
    }

    return validate_user_create_dto(dto);
}
