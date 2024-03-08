#include "utils.h"

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
