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

t_message *mx_init_message(void);
void mx_delete_message(t_message *u);

t_read_message *mx_init_read_message(void);
void mx_delete_read_message(t_read_message *u);

t_message_file *mx_init_message_file(void);
void mx_delete_message_file(t_message_file *u);

t_message_create_dto *mx_get_message_create_dto(struct mg_str body);
void mx_delete_message_create_dto(t_message_create_dto *dto);

void mx_message_ctrl_get(t_connection *c, t_http_message *m);
void mx_message_ctrl_post(t_connection *c, t_http_message *m);
void mx_message_ctrl_put(t_connection *c, t_http_message *m);
void mx_message_ctrl_delete(t_connection *c, t_http_message *m);

t_message *mx_message_get(int id);
t_message *mx_message_create(t_message_create_dto *dto);
t_message *mx_message_put(int id, t_message_create_dto *dto);
t_message *mx_message_delete(int id);

t_message *mx_message_repo_get(int id);
int mx_message_repo_create(t_message_create_dto *dto);
bool mx_message_repo_put(int id, t_message_create_dto *dto);
bool mx_message_repo_delete(int id);

cJSON *mx_message_to_cjson(t_message *message);
cJSON *mx_messages_to_cjson(t_list *messages);
t_string mx_message_stringify(t_message *message);

cJSON *mx_read_message_to_cjson(t_read_message *message);
cJSON *mx_read_messages_to_cjson(t_list *message);
t_string mx_read_message_stringify(t_read_message *message);

t_message_file *mx_message_file_get(int id);
t_list *mx_message_file_get_all(int user_id);
t_message_file *mx_message_file_delete(int id);
t_message_file *mx_message_file_update(int id, char *type, char *file_id);
t_message_file *mx_message_file_create(int file_id, char *type);

t_message_file *mx_message_file_repo_get(int id);
t_list *mx_message_file_repo_get_all(int user_id);
int mx_message_file_repo_create(int file_id, char *type);
bool mx_message_file_repo_update(int id, int file_id, char *type);
bool mx_message_file_repo_delete(int id);

cJSON *mx_message_file_to_cjson(t_message_file *message_file);
t_string mx_message_file_stringify(t_message_file *message);
cJSON *mx_message_files_to_cjson(t_list *messages);
t_string mx_message_files_stringify(t_list *messages);

void mx_message_route(t_connection *conn, t_http_message *req);
#endif
