#ifndef BACKEND_UTILS_H
#define BACKEND_UTILS_H

#include "base.h"

typedef void (*t_func_free)(void *);

void mx_list_free(t_list **list, t_func_free func);
void mx_delete_node(t_list **list, t_list *node, t_func_free clean_data);

void mx_list_free(t_list **list, t_func_free func);
void mx_delete_node(t_list **list, t_list *node, t_func_free clean_data);

bool mx_create_path_if_not_exist(char *path);
t_string mx_decode_uri_component(struct mg_str str);
char *mx_gen_uuid(void);
bool mx_is_user_member_of(int room_id, int user_id);
bool mx_is_room_admin(int room_id, int user_id);
char *mx_path_join(const char *path, const char *name);
bool mx_is_message_author(int user_id, int message_id);

char *mx_cjson_get_string(cJSON *json, const char *key);
int mx_cjson_get_number(cJSON *json, const char *key);
bool mx_cjson_get_bool(cJSON *json, const char *key);

char *mx_sqlite3_get_string(sqlite3_stmt *stmt, int col);
void mx_sqlite3_bind_id(sqlite3_stmt *stmt, int col, int id);
#endif
