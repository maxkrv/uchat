#include "message.h"

static t_message_create_dto *init_message_create_dto() {
    t_message_create_dto *dto =
        (t_message_create_dto *)malloc(sizeof(t_message_create_dto));
    dto->reply_id = 0;
    dto->file_ids = NULL;
    dto->text = NULL;
    dto->room_id = 0;

    return dto;
}

void mx_message_create_dto_free(t_message_create_dto *dto) {
    if (!dto) {
        return;
    }
    mx_strdel(&dto->text);
    mx_list_free(&dto->file_ids, free);

    free(dto);
}

static t_message_create_dto *
validate_message_create_dto(t_message_create_dto *dto) {
    if (!dto->room_id || !(dto->text || dto->file_ids)) {
        mx_message_create_dto_free(dto);
        return NULL;
    }

    return dto;
}

static t_message_create_dto *parse_message_create_dto(struct mg_str body) {
    t_message_create_dto *dto = init_message_create_dto();
    cJSON *obj = cJSON_ParseWithLength(body.ptr, body.len);
    cJSON *key;

    if (!obj) {
        free(dto);
        return NULL;
    }

    dto->reply_id = mx_cjson_get_number(obj, "reply_id");
    dto->room_id = mx_cjson_get_number(obj, "room_id");
    dto->text = mx_cjson_get_string(obj, "text");

    key = cJSON_GetObjectItemCaseSensitive(obj, "file_ids");
    cJSON *arr_value = NULL;

    cJSON_ArrayForEach(arr_value, key) {
        if (cJSON_IsNumber(arr_value)) {
            mx_push_back(&dto->file_ids, mx_itoa(arr_value->valueint));
        }
    }
    cJSON_Delete(obj);

    return dto;
}

t_message_create_dto *mx_message_create_dto_get(struct mg_str body) {
    t_message_create_dto *dto = parse_message_create_dto(body);

    if (!dto) {
        return NULL;
    }

    return validate_message_create_dto(dto);
}

t_message_create_dto *mx_message_update_dto_get(struct mg_str body) {
    t_message_create_dto *dto = parse_message_create_dto(body);

    if (!dto) {
        return NULL;
    }
    if (!(dto->text || dto->file_ids)) {
        mx_message_create_dto_free(dto);
        return NULL;
    }
    return dto;
}
