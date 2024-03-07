#ifndef SERVER_FILE_H
#define SERVER_FILE_H

#include "base.h"
#include "structs.h"

t_file *mx_init_file(void);
void mx_delete_file(t_file *u);

t_file *mx_file_repo_get(int id);

cJSON *mx_file_to_cjson(t_file *file);
t_string mx_file_stringify(t_file *file);
cJSON *mx_files_to_cjson(t_list *file);
t_string mx_files_stringify(t_list *files);

#endif
