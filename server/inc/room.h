#ifndef UCHAT_ROOM
#define UCHAT_ROOM

#define MX_ROOM_TYPE_GROUP "group"
#define MX_ROOM_TYPE_DIRECT "direct"
#define MX_ROOM_TYPE_NOTES "notes"

#include "base.h"
#include "utils.h"
#include "shared.h"
#include "auth.h"

typedef struct {
    char *name;
    char *description;
    int photo_id;
    char *type;
} t_room_create_dto;

typedef struct {
    int room_id;
    int user_id;
    bool is_admin;
} t_room_member_create_dto;
typedef struct {
    bool is_admin;
} t_room_member_update_dto;
// room router
void mx_room_route(t_connection *conn, t_http_message *req);

/*
/////////////////////////////////////////////
 Room module
 Room { descriptions,
            photo}
/////////////////////////////////////////////
*/
// structure constructors and destructors
t_room *mx_room_init(void);
void mx_room_free(t_room *u);
t_room_create_dto *mx_room_create_dto_get(struct mg_str body);
t_room_create_dto *mx_room_create_dto_init(void);
void mx_room_create_dto_free(t_room_create_dto *d);

// constrollers
void mx_room_ctrl_get(t_connection *c, t_http_message *m);
void mx_room_ctrl_post(t_connection *c, t_http_message *m);
void mx_room_direct_ctrl_post(t_connection *c, t_http_message *m);
void mx_room_ctrl_put(t_connection *c, t_http_message *m);
void mx_room_ctrl_delete(t_connection *c, t_http_message *m);

// services
t_room *mx_room_get(int id);
t_room *mx_room_create(t_room_create_dto *dto, int user_id);
t_room *mx_room_put(int id, t_room_create_dto *dto);
t_room *mx_room_delete(int id);
t_room *mx_room_create_direct(t_room_create_dto *dto, t_user_id first_id,
                              t_user_id second_id);

// repositories
t_room *mx_room_repo_get(int id);
t_list *mx_room_repo_get_many(int user_id);
bool mx_room_repo_direct_exist(t_user_id first_id, t_user_id second_id);
int mx_members_repo_count(int room_id);
int mx_room_repo_create(t_room_create_dto *dto);
bool mx_room_repo_put(int id, t_room_create_dto *dto);
bool mx_room_repo_delete(int id);

// json
cJSON *mx_room_to_cjson(t_room *room);
cJSON *mx_rooms_to_cjson(t_list *rooms);
t_string mx_room_stringify(t_room *room);
t_string mx_rooms_stringify(t_list *rooms);

t_room *mx_sqlite_bind_columns_to_room(sqlite3_stmt *stmt, int from);

/*
/////////////////////////////////////////////
 Room.pined_messages
 PinedMessage { descriptions,
            message,
            message_files}
/////////////////////////////////////////////
*/
// structure constructors and destructors
t_pined_message *mx_room_pined_init(void);
void mx_room_pined_free(t_pined_message *m);

// controllers
void mx_room_ctrl_pine_message(t_connection *c, t_http_message *m);
void mx_room_ctrl_get_pined(t_connection *c, t_http_message *m);
void mx_room_ctrl_unpine(t_connection *c, t_http_message *m);

// services
t_list *mx_room_get_pined_messages(int room_id);
t_pined_message *mx_room_pine_message(int message_id, int room_id);
t_pined_message *mx_room_unpine(int id);

// repositories
t_pined_message *mx_pined_repo_get(int id);
t_list *mx_pined_repo_get_many(int room_id);
int mx_pined_repo_create(int message_id, int room_id);
bool mx_pined_repo_delete(int id);

// json
cJSON *mx_pined_message_to_cjson(t_pined_message *pined);
t_string mx_pined_message_stringify(t_pined_message *pined);
cJSON *mx_pined_messages_to_cjson(t_list *members);
t_string mx_pined_messages_stringify(t_list *members);

t_pined_message *mx_sqlite_bind_columns_to_pine(sqlite3_stmt *stmt, int from);

/*
/////////////////////////////////////////////
 Room.members.
 Member { descriptions,
            user,
            user.photo}
/////////////////////////////////////////////
*/
// structure constructors and destructors
t_room_member *mx_room_member_init(void);
void mx_room_member_free(t_room_member *m);
t_room_member_create_dto *
mx_room_member_create_dto_constructor(int room_id, int user_id, bool is_admin);
t_room_member_create_dto *mx_room_member_create_dto_get(struct mg_str body);
void mx_room_member_create_dto_free(t_room_member_create_dto *dto);
t_room_member_update_dto *mx_parse_room_member_update_dto(struct mg_str body);
void mx_room_member_update_dto_free(t_room_member_update_dto *dto);

// controllers
void mx_room_ctrl_get_member(t_connection *c, t_http_message *m);
void mx_room_ctrl_get_members(t_connection *c, t_http_message *m);
void mx_room_ctrl_add_member(t_connection *c, t_http_message *m);
void mx_room_ctrl_update_member(t_connection *c, t_http_message *m);
void mx_room_ctrl_delete_member(t_connection *c, t_http_message *m);

// services
t_list *mx_room_get_members(int room_id);
t_room_member *mx_room_get_member(int id);
t_room_member *mx_room_add_member(t_room_member_create_dto *dto);
t_room_member *mx_room_update_member(int id, t_room_member_update_dto *dto);
t_room_member *mx_room_delete_member(int id);

// repositories
t_room_member *mx_members_repo_get(int id);
t_room_member *mx_members_repo_get_by(int user_id, int room_id);
int mx_members_repo_create(int user_id, int room_id, bool is_admin);
bool mx_members_repo_update(int id, bool is_admin);
t_list *mx_members_repo_get_many(int room_id);
bool mx_members_repo_delete(int id);

// json
cJSON *mx_member_to_cjson(t_room_member *room);
t_string mx_member_stringify(t_room_member *member);
cJSON *mx_members_to_cjson(t_list *rooms);
t_string mx_members_stringify(t_list *members);

t_room_member *mx_sqlite_bind_columns_to_member(sqlite3_stmt *stmt, int from);
#endif
