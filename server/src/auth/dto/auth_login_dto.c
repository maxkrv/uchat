#include "auth.h"

bool mx_is_valid_password(char *password) {
    int length = strlen(password);
    bool has_lower = false;
    bool has_upper = false;
    bool has_digit = false;

    if (length < 8) {
        return false;
    }

    for (int i = 0; i < length; i++) {
        if (password[i] >= 'A' && password[i] <= 'Z') {
            has_upper = true;
            continue;
        }
        if (password[i] >= 'a' && password[i] <= 'z') {
            has_lower = true;
            continue;
        }
        if (password[i] >= '0' && password[i] <= '9') {
            has_digit = true;
            continue;
        }
    }

    return has_lower && has_digit && has_upper;
}

static t_login_dto *init_login_dto() {
    t_login_dto *dto = (t_login_dto *)malloc(sizeof(t_login_dto));
    dto->name = NULL;
    dto->password = NULL;

    return dto;
}
void mx_login_dto_free(t_login_dto *dto) {
    if (!dto) {
        return;
    }
    mx_strdel(&dto->name);
    mx_strdel(&dto->password);

    free(dto);
}

static t_login_dto *validate_login_dto(t_login_dto *dto) {
    if (!dto->name || !dto->password || mx_strlen(dto->name) < 4 ||
        !mx_is_valid_password(dto->password)) {

        mx_login_dto_free(dto);
        return NULL;
    }

    return dto;
}

static t_login_dto *parse_login_dto(struct mg_str body) {
    t_login_dto *dto = init_login_dto();
    cJSON *obj = cJSON_ParseWithLength(body.ptr, body.len);

    if (!obj) {
        free(dto);
        return NULL;
    }

    dto->name = mx_cjson_get_string(obj, "name");
    dto->password = mx_cjson_get_string(obj, "password");

    cJSON_Delete(obj);

    return dto;
}

t_login_dto *mx_login_dto_get(struct mg_str body) {
    t_login_dto *dto = parse_login_dto(body);

    if (!dto) {
        return NULL;
    }

    return validate_login_dto(dto);
}
