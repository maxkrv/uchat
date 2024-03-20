#include "sdk/json.h"

char *mx_cjson_get_string(cJSON *json, const char *key) {
    cJSON *item = cJSON_GetObjectItemCaseSensitive(json, key);
    char *value = cJSON_GetStringValue(item);

    if (!value) {
        return NULL;
    }

    return mx_strdup(value);
}

int mx_cjson_get_number(cJSON *json, const char *key) {
    cJSON *item = cJSON_GetObjectItemCaseSensitive(json, key);
    int value = cJSON_GetNumberValue(item);

    return value;
}

bool mx_cjson_get_bool(cJSON *json, const char *key) {
    cJSON *item = cJSON_GetObjectItemCaseSensitive(json, key);

    if (cJSON_IsBool(item)) {
        return item->valueint;
    }

    return 0;
}

t_list *mx_cjson_parse_array(cJSON *entities, t_func_parser parse) {
    t_list *list = NULL;
    cJSON *arr_value = NULL;

    cJSON_ArrayForEach(arr_value, entities) {
        if (arr_value->type == cJSON_Object) {
            mx_push_back(&list, parse(arr_value));
        }
    }

    return list;
}
