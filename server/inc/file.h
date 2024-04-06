#ifndef SERVER_FILE_H
#define SERVER_FILE_H

#define MX_MAX_FILE_SIZE 10 * 1024 * 1024
#include "base.h"
#include "structs.h"

void mx_file_route(t_connection *c, t_http_message *req);

t_file *mx_file_init(void);
void mx_file_free(t_file *u);

void mx_file_ctrl_get(t_connection *c, t_http_message *m);
void mx_file_ctrl_upload(t_connection *c, t_http_message *m);

t_file *mx_file_service_get(int file_id);
t_file *mx_file_service_upload(char *file, int file_len, char *filename);
t_file *mx_file_service_delete(int file_id);

bool mx_file_delete_uploaded(char *filename);
bool mx_file_upload(const char *file, int file_len, const char *filename);
t_file *mx_file_save_to_db(char *filename, char *url);
t_file *mx_file_delete_from_db(int file_id);

t_file *mx_file_repo_get(int id);
int mx_file_repo_create(char *filename, char *url);
bool mx_file_repo_delete(int file_id);

cJSON *mx_file_to_cjson(t_file *file);
t_string mx_file_stringify(t_file *file);
cJSON *mx_files_to_cjson(t_list *file);
t_string mx_files_stringify(t_list *files);

t_file *mx_sqlite_bind_columns_to_file(sqlite3_stmt *stmt, int from);

#endif
