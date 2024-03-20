#include "sdk/sdk.h"

t_user_create_dto *mx_user_create_dto_init(void) {
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

void mx_user_create_dto_free(t_user_create_dto *dto) {
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

t_string mx_user_create_dto_stringify(t_user_create_dto *dto) {
    if (!dto) {
        return NULL;
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddStringToObject(obj, "name", dto->name);
    cJSON_AddStringToObject(obj, "password", dto->password);
    cJSON_AddStringToObject(obj, "tag", dto->tag);
    cJSON_AddStringToObject(obj, "status", dto->status);
    cJSON_AddNumberToObject(obj, "photo_id", dto->photo_id);

    t_string str = cJSON_PrintUnformatted(obj);
    cJSON_Delete(obj);

    return str;
}

t_user_update_dto *mx_user_update_dto_init(void) {
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

t_string mx_user_update_dto_stringify(t_user_update_dto *dto) {
    if (!dto) {
        return NULL;
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddStringToObject(obj, "name", dto->name);
    cJSON_AddStringToObject(obj, "tag", dto->tag);
    cJSON_AddStringToObject(obj, "status", dto->status);
    cJSON_AddNumberToObject(obj, "photo_id", dto->photo_id);

    t_string str = cJSON_PrintUnformatted(obj);
    cJSON_Delete(obj);

    return str;
}
