#include "sdk/sdk.h"

void *mx_entity_parse_string(t_string str, t_func_parser parse) {
    cJSON *obj = cJSON_Parse(str);

    if (!obj) {
        return NULL;
    }

    void *ent = parse(obj);

    cJSON_Delete(obj);

    return ent;
}

t_list *mx_entities_parse_string(t_string str, t_func_parser parse) {
    cJSON *obj = cJSON_Parse(str);

    if (!obj) {
        return NULL;
    }

    t_list *list = NULL;
    cJSON *arr_value = NULL;

    cJSON_ArrayForEach(arr_value, obj) {
        if (arr_value->type == cJSON_Object) {
            void *ent = parse(arr_value);

            if (ent) {
                mx_push_back(&list, ent);
            }
        }
    }

    cJSON_Delete(obj);

    return list;
}
