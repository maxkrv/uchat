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

    if (header_obj == NULL) {
        return head;
    }

    head.alg = mx_cjson_get_string(header_obj, "alg");
    head.typ = mx_cjson_get_string(header_obj, "typ");
    head.iat = mx_cjson_get_number(header_obj, "iat");
    head.exp = mx_cjson_get_number(header_obj, "exp");

    cJSON_Delete(header_obj);
    return head;
}
void mx_jwt_head_free(t_jwt_head *head) {
    mx_strdel(&head->alg);
    mx_strdel(&head->typ);
}
