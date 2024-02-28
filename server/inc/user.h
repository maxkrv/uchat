#ifndef UCHAT_USER
#define UCHAT_USER

#include "base.h"
#include "utils.h"
#include "shared.h"
#include "auth.h"

typedef struct {
    char *name;
    char *tag;
    char *password;
    int photo_id;
    char *status;
    char *description;
} t_user_create_dto;

t_user_create_dto *mx_get_user_create_dto(t_string body);

// controllers
void mx_user_ctrl_get(t_connection *c, t_http_message *m);
void mx_user_ctrl_post(t_connection *c, t_http_message *m);
void mx_user_ctrl_put(t_connection *c, t_http_message *m);
void mx_user_ctrl_delete(t_connection *c, t_http_message *m);

// service
t_user *mx_user_get(int id);
t_user *mx_user_create(t_user_create_dto *dto);
t_user *mx_user_put(int id, t_user_create_dto *dto);
t_user *mx_user_delete(int id);

t_user *mx_user_repo_get(int id);
bool mx_user_repo_create(t_user_create_dto *dto);
bool mx_user_repo_put(int id, t_user_create_dto *dto);
bool mx_user_repo_delete(int id);

void mx_user_route(t_connection *conn, t_http_message *req);

// helpers
cJSON *mx_user_to_cjson(t_user *user);
t_string mx_user_stringify(t_user *user);
cJSON *mx_users_to_cjson(t_list *users);

cJSON *mx_favorite_room_to_cjson(t_favorite_room *fav);
t_string mx_favorite_room_stringify(t_favorite_room *fav);
cJSON *mx_favorite_rooms_to_cjson(t_list *members);
t_string mx_favorite_rooms_stringify(t_list *members);

cJSON *mx_room_pined_message_to_cjson(t_room_pined_message *pined);
t_string mx_room_pined_message_stringify(t_room_pined_message *pined);
cJSON *mx_room_pined_messages_to_cjson(t_list *members);
t_string mx_room_pined_messages_stringify(t_list *members);

#endif
