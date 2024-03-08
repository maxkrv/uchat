#ifndef BACKEND_UTILS_H
#define BACKEND_UTILS_H

#include "base.h"

typedef void (*t_func_void)(void *);
t_string mx_decode_uri_component(struct mg_str str);
char *mx_gen_uuid(void);
void mx_delete_list(t_list **list, t_func_void func);
bool mx_is_user_member_of(int room_id, int user_id);
bool mx_is_room_admin(int room_id, int user_id);
bool mx_is_message_author(int user_id, int message_id);

char *mx_cjson_get_string(cJSON *json, const char *key);
int mx_cjson_get_number(cJSON *json, const char *key);
bool mx_cjson_get_bool(cJSON *json, const char *key);

#endif
