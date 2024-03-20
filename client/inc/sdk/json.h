#ifndef SDK_JSON_H
#define SDK_JSON_H

#include "sdk/base.h"
#include "sdk/utils.h"
#include "sdk/structs.h"
#include "sdk/dto.h"

char *mx_cjson_get_string(cJSON *json, const char *key);
int mx_cjson_get_number(cJSON *json, const char *key);
bool mx_cjson_get_bool(cJSON *json, const char *key);
t_list *mx_cjson_parse_array(cJSON *entities, t_func_parser parse);

void *mx_entity_parse_string(t_string str, t_func_parser parse);
t_list *mx_entities_parse_string(t_string str, t_func_parser parse);

t_file *mx_file_parse_cjson(cJSON *obj);
t_message *mx_message_parse_cjson(cJSON *obj);
t_read_message *mx_reader_parse_cjson(cJSON *message);
t_room *mx_room_parse_cjson(cJSON *room);
t_pined_message *mx_pined_parse_cjson(cJSON *pined);
t_room_member *mx_member_parse_cjson(cJSON *mem);
t_user *mx_user_parse_cjson(cJSON *user);
t_favorite_room *mx_favorite_parse_cjson(cJSON *fav);
#endif
