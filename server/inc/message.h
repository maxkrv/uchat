#ifndef UCHAT_MESSAGE
#define UCHAT_MESSAGE

#define MX_MX_MESSAGE_FILE_TYPE_UNKNOWN "unknown";
#define MX_MESSAGE_FILE_TYPE_TEXT "text";
#define MX_MESSAGE_FILE_TYPE_IMAGE "image";
#define MX_MESSAGE_FILE_TYPE_AUDIO "audio";
#define MX_MESSAGE_FILE_TYPE_VIDEO "video";
#define MX_MESSAGE_FILE_TYPE_FILE "file";

#include "base.h"
#include "utils.h"
#include "shared.h"
#include "auth.h"

typedef struct {
    int room_id;
    int reply_id;
    t_list *file_ids;
    char *text;
} t_message_create_dto;

t_message_create_dto *mx_get_message_create_dto(t_string body);

void mx_message_ctrl_get(t_connection *c, t_http_message *m);
void mx_message_ctrl_post(t_connection *c, t_http_message *m);
void mx_message_ctrl_put(t_connection *c, t_http_message *m);
void mx_message_ctrl_delete(t_connection *c, t_http_message *m);

t_message *mx_message_get(int id);
t_message *mx_message_create(t_message_create_dto *dto);
t_message *mx_message_put(int id, t_message_create_dto *dto);
t_message *mx_message_delete(int id);

t_message *mx_message_repo_get(int id);
bool mx_message_repo_create(t_message_create_dto *dto);
bool mx_message_repo_put(int id, t_message_create_dto *dto);
bool mx_message_repo_delete(int id);

cJSON *mx_message_to_cjson(t_message *message);
cJSON *mx_messages_to_cjson(t_list *messages);
t_string mx_message_stringify(t_message *message);

cJSON *mx_read_message_to_cjson(t_read_message *message);
cJSON *mx_read_messages_to_cjson(t_list *message);
t_string mx_read_message_stringify(t_read_message *message);

cJSON *mx_message_file_to_cjson(t_message_file *message_file);
t_string mx_message_file_stringify(t_message_file *message);
cJSON *mx_message_files_to_cjson(t_list *messages);
t_string mx_message_files_stringify(t_list *messages);

void mx_message_route(t_connection *conn, t_http_message *req);
#endif
