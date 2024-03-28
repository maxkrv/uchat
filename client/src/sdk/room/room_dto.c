#include "sdk/sdk.h"

t_room_create_dto *mx_room_create_dto_init(void) {
    t_room_create_dto *dto =
        (t_room_create_dto *)malloc(sizeof(t_room_create_dto));
    dto->name = NULL;
    dto->description = NULL;
    dto->photo_id = 0;

    return dto;
}

void mx_room_create_dto_free(t_room_create_dto *dto) {
    if (!dto) {
        return;
    }
    mx_strdel(&dto->name);
    mx_strdel(&dto->description);

    free(dto);
}
t_string mx_room_create_dto_stringify(t_room_create_dto *dto) {
    if (!dto) {
        return NULL;
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddStringToObject(obj, "name", dto->name);
    cJSON_AddStringToObject(obj, "description", dto->description);
    cJSON_AddNumberToObject(obj, "photo_id", dto->photo_id);

    t_string str = cJSON_PrintUnformatted(obj);
    cJSON_Delete(obj);

    return str;
}

t_string mx_room_member_create_stringify(int room_id, int user_id,
                                         bool is_admin) {
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "room_id", room_id);
    cJSON_AddNumberToObject(obj, "user_id", user_id);
    cJSON_AddBoolToObject(obj, "is_admin", is_admin);

    t_string str = cJSON_PrintUnformatted(obj);
    cJSON_Delete(obj);

    return str;
}

t_string mx_room_member_update_stringify(bool is_admin) {
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddBoolToObject(obj, "is_admin", is_admin);

    t_string str = cJSON_PrintUnformatted(obj);
    cJSON_Delete(obj);

    return str;
}
