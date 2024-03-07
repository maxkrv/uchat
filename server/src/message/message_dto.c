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

void mx_delete_message_create_dto(t_message_create_dto *dto) {
    if (!dto) {
        return;
    }
    mx_strdel(&dto->text);
    mx_delete_list(&dto->file_ids, free);

    free(dto);
}

static t_message_create_dto *
validate_message_create_dto(t_message_create_dto *dto) {
    if (!dto->room_id || !dto->text) {
        mx_delete_message_create_dto(dto);
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

    key = cJSON_GetObjectItemCaseSensitive(obj, "reply_id");
    dto->reply_id = cJSON_GetNumberValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "room_id");
    dto->room_id = cJSON_GetNumberValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "text");
    dto->text = cJSON_GetStringValue(key);

    key = cJSON_GetObjectItemCaseSensitive(obj, "file_ids");
    cJSON *arr_value = NULL;

    cJSON_ArrayForEach(arr_value, key) {
        if (cJSON_IsNumber(arr_value)) {
            mx_push_back(&dto->file_ids, &arr_value->valuedouble);
        }
    }
    cJSON_Delete(obj);

    return dto;
}

t_message_create_dto *mx_get_message_create_dto(struct mg_str body) {
    t_message_create_dto *dto = parse_message_create_dto(body);

    if (!dto) {
        return NULL;
    }

    return validate_message_create_dto(dto);
}
