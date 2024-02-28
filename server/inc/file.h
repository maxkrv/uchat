#ifndef SERVER_FILE_H
#define SERVER_FILE_H

#include "base.h"
#include "structs.h"

cJSON *mx_file_to_cjson(t_file *file);
t_string mx_file_stringify(t_file *file);
cJSON *mx_files_to_cjson(t_list *file);
t_string mx_files_stringify(t_list *files);

#endif
