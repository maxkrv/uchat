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
    t_list *list = parse(obj);

    cJSON_Delete(obj);

    return list;
}
