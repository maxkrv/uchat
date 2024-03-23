#include "server.h"

cJSON *mx_file_to_cjson(t_file *file) {
    if (!file) {
        return cJSON_CreateNull();
    }
    cJSON *obj = cJSON_CreateObject();

    cJSON_AddNumberToObject(obj, "id", file->id);
    cJSON_AddStringToObject(obj, "name", file->name);
    cJSON_AddStringToObject(obj, "url", file->url);

    cJSON_AddNumberToObject(obj, "created_at", file->created_at);
    cJSON_AddNumberToObject(obj, "edited_at", file->edited_at);

    return obj;
}

t_string mx_file_stringify(t_file *file) {
    cJSON *obj = mx_file_to_cjson(file);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}

cJSON *mx_files_to_cjson(t_list *file) {
    cJSON *members_array = cJSON_CreateArray();

    for (t_list *node = file; node; node = node->next) {
        cJSON_AddItemToArray(members_array, mx_file_to_cjson(node->data));
    }

    return members_array;
}

t_string mx_files_stringify(t_list *files) {
    cJSON *obj = mx_files_to_cjson(files);
    t_string res = cJSON_PrintUnformatted(obj);

    cJSON_Delete(obj);

    return res;
}
