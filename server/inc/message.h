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
// message router
void mx_message_route(t_connection *conn, t_http_message *req);

/*
/////////////////////////////////////////////
 Message module
 Message { descriptions,
            author,
            message_files}
/////////////////////////////////////////////
*/
// structure constructors and destructors
t_message *mx_message_init(void);
void mx_message_free(t_message *u);
t_message_create_dto *mx_message_create_dto_get(struct mg_str body);
t_message_create_dto *mx_message_update_dto_get(struct mg_str body);
void mx_message_create_dto_free(t_message_create_dto *dto);

// controllers
void mx_message_ctrl_get(t_connection *c, t_http_message *m);
void mx_message_ctrl_get_many(t_connection *c, t_http_message *m);
void mx_message_ctrl_post(t_connection *c, t_http_message *m);
void mx_message_ctrl_put(t_connection *c, t_http_message *m);
void mx_message_ctrl_delete(t_connection *c, t_http_message *m);

// services
t_message *mx_message_get(int id);
t_list *mx_message_get_many(int room_id);
t_message *mx_message_create(int user_id, t_message_create_dto *dto);
t_message *mx_message_put(int id, t_message_create_dto *dto);
t_message *mx_message_delete(int id);

// repositories
t_message *mx_message_repo_get(int id);
t_list *mx_message_repo_get_many(int room_id);
int mx_message_repo_create(int user_id, t_message_create_dto *dto);
bool mx_message_repo_put(int id, t_message_create_dto *dto);
bool mx_message_repo_delete(int id);

// json
cJSON *mx_message_to_cjson(t_message *message);
cJSON *mx_messages_to_cjson(t_list *messages);
t_string mx_message_stringify(t_message *message);
t_string mx_messages_stringify(t_list *messages);

t_message *mx_sqlite_bind_columns_to_message(sqlite3_stmt *stmt, int from);

/*
/////////////////////////////////////////////
 Message.read_messages
 ReadMessage { descriptions,
                user }
/////////////////////////////////////////////
*/
// structure constructors and destructors
t_read_message *mx_read_message_init(void);
void mx_read_message_free(t_read_message *u);

// constrollers
void mx_message_ctrl_get_reader(t_connection *c, t_http_message *m);
void mx_message_ctrl_get_readers(t_connection *c, t_http_message *m);
void mx_message_ctrl_read_message(t_connection *c, t_http_message *m);

// services
t_list *mx_read_messages_get(int message_id);
t_read_message *mx_read_message_get(int id);
t_read_message *mx_read_message(int user_id, int message_id);

// repositories
t_list *mx_read_messages_repo_get(int message_id);
t_read_message *mx_read_message_repo_get(int id);
int mx_message_repo_read(int user_id, int message_id);

// json
cJSON *mx_read_message_to_cjson(t_read_message *message);
cJSON *mx_read_messages_to_cjson(t_list *message);
t_string mx_read_message_stringify(t_read_message *message);
t_string mx_read_messages_stringify(t_list *messages);

t_read_message *mx_sqlite_bind_columns_to_reader(sqlite3_stmt *stmt, int from);

/*
/////////////////////////////////////////////
 Message.message_files
 MessageFiles { descriptions,
                file }
/////////////////////////////////////////////
*/

// repositories
int mx_message_file_repo_create(int message_id, int file_id);
bool mx_message_file_repo_delete(int file_id);
#endif
