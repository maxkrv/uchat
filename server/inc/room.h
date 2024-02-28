#ifndef UCHAT_ROOM
#define UCHAT_ROOM

#define MX_ROOM_TYPE_GROUP "group";
#define MX_ROOM_TYPE_DIRECT "direct";

#include "base.h"
#include "utils.h"
#include "shared.h"
#include "auth.h"

typedef struct {
    char *name;
    char *discription;
    int photo_id;
    char *type;
} t_room_create_dto;

t_room_create_dto *mx_get_room_create_dto(t_string body);

void mx_room_ctrl_get(t_connection *c, t_http_message *m);
void mx_room_ctrl_post(t_connection *c, t_http_message *m);
void mx_room_ctrl_put(t_connection *c, t_http_message *m);
void mx_room_ctrl_delete(t_connection *c, t_http_message *m);

t_room *mx_room_get(int id);
t_room *mx_room_create(t_room_create_dto *dto);
t_room *mx_room_put(int id, t_room_create_dto *dto);
t_room *mx_room_delete(int id);

t_room *mx_room_repo_get(int id);
bool mx_room_repo_create(t_room_create_dto *dto);
bool mx_room_repo_put(int id, t_room_create_dto *dto);
bool mx_room_repo_delete(int id);

cJSON *mx_room_to_cjson(t_room *room);
cJSON *mx_rooms_to_cjson(t_list *rooms);
t_string mx_room_stringify(t_room *room);

cJSON *mx_room_member_to_cjson(t_room_member *room);
cJSON *mx_room_members_to_cjson(t_list *rooms);
t_string mx_room_members_stringify(t_list *members);

void mx_room_route(t_connection *conn, t_http_message *req);
#endif
