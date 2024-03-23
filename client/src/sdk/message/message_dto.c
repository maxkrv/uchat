#include "sdk/sdk.h"

t_message_create_dto *mx_message_create_dto_init(void) {
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

t_string mx_message_create_dto_stringify(t_message_create_dto *dto) {
    if (!dto) {
        return NULL;
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddStringToObject(obj, "text", dto->text);
    cJSON_AddNumberToObject(obj, "room_id", dto->room_id);
    cJSON_AddNumberToObject(obj, "reply_id", dto->reply_id);

    cJSON *file_ids = cJSON_CreateArray();

    for (t_list *list = dto->file_ids; list; list = list->next) {
        cJSON_AddItemToArray(file_ids,
                             cJSON_CreateNumber(mx_atoi(list->data)));
    }

    cJSON_AddItemToObject(obj, "file_ids", file_ids);

    t_string str = cJSON_PrintUnformatted(obj);
    cJSON_Delete(obj);

    return str;
}
