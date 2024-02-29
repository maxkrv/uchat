#include "jwt.h"

char *mx_jwt_head_stringify(t_jwt_head *head) {
    cJSON *header_obj = cJSON_CreateObject();

    cJSON_AddItemToObject(header_obj, "alg", cJSON_CreateString(head->alg));
    cJSON_AddItemToObject(header_obj, "typ", cJSON_CreateString(head->typ));
    cJSON_AddItemToObject(header_obj, "exp", cJSON_CreateNumber(head->exp));
    cJSON_AddItemToObject(header_obj, "iat", cJSON_CreateNumber(head->iat));

    char *header_json = cJSON_PrintUnformatted(header_obj);
    cJSON_Delete(header_obj);

    return header_json;
}

t_jwt_head mx_jwt_head_parse(char *header_json) {
    t_jwt_head head = {NULL, NULL, -1, -1};
    cJSON *header_obj = cJSON_Parse(header_json);
    cJSON *key;

    if (header_obj == NULL) {
        return head; // Failed to parse JSON
    }

    key = cJSON_GetObjectItemCaseSensitive(header_obj, "alg");
    head.alg = cJSON_GetStringValue(key);

    key = cJSON_GetObjectItemCaseSensitive(header_obj, "typ");
    head.typ = cJSON_GetStringValue(key);

    key = cJSON_GetObjectItemCaseSensitive(header_obj, "iat");
    head.iat = cJSON_GetNumberValue(key);

    key = cJSON_GetObjectItemCaseSensitive(header_obj, "exp");
    head.exp = cJSON_GetNumberValue(key);

    cJSON_Delete(header_obj);
    return head;
}
