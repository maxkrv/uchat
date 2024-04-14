#include "user.h"

static t_user_update_dto *init_user_update_dto() {
    t_user_update_dto *dto =
        (t_user_update_dto *)malloc(sizeof(t_user_update_dto));
    dto->name = NULL;
    dto->photo_id = 0;
    dto->tag = NULL;
    dto->status = NULL;
    dto->description = NULL;

    return dto;
}

void mx_user_update_dto_free(t_user_update_dto *dto) {
    if (!dto) {
        return;
    }

    mx_strdel(&dto->name);
    mx_strdel(&dto->tag);
    mx_strdel(&dto->status);
    mx_strdel(&dto->description);

    free(dto);
}

static t_user_update_dto *validate_user_update_dto(t_user_update_dto *dto) {
    if (!dto->name || mx_strlen(dto->name) < 4) {
        mx_user_update_dto_free(dto);
        return NULL;
    }

    return dto;
}

static t_user_update_dto *parse_user_update_dto(struct mg_str body) {
    t_user_update_dto *dto = init_user_update_dto();
    cJSON *obj = cJSON_ParseWithLength(body.ptr, body.len);

    if (!obj) {
        free(dto);
        return NULL;
    }

    dto->name = mx_cjson_get_string(obj, "name");
    dto->photo_id = mx_cjson_get_number(obj, "photo_id");
    dto->tag = mx_cjson_get_string(obj, "tag");
    dto->status = mx_cjson_get_string(obj, "status");
    dto->description = mx_cjson_get_string(obj, "description");

    cJSON_Delete(obj);

    return dto;
}

t_user_update_dto *mx_user_update_dto_get(struct mg_str body) {
    t_user_update_dto *dto = parse_user_update_dto(body);

    if (!dto) {
        return NULL;
    }

    return validate_user_update_dto(dto);
}
