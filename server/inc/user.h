#ifndef UCHAT_USER
#define UCHAT_USER

#include "base.h"
#include "utils.h"
#include "shared.h"

typedef struct {
    int id;
    char *name;
    char *tag;
    char *password_hash;
    char *photo_url;
    char *status;
    char *description;
    int created_at;
    int edited_at;

    t_list *messages;
    t_list *rooms;
    t_list *read_messages;
} t_user;

typedef struct {
    char *name;
    char *tag;
    char *password;
    char *photo_url;
} t_user_create_dto;

typedef struct {
    char *name;
    char *tag;
    char *password;
    char *photo_url;
} t_user_put_dto;

//controllers
void mx_user_ctrl_get(t_connection *c, t_http_message *m);

void mx_user_ctrl_post(t_connection *c, t_http_message *m);

void mx_user_ctrl_put(t_connection *c, t_http_message *m);

void mx_user_ctrl_delete(t_connection *c, t_http_message *m);

//service
t_user *mx_user_get(int id);

t_user *mx_user_create(t_user_create_dto *dto);

t_user *mx_user_put(int id, t_user_put_dto *dto);

t_user *mx_user_delete(int id);

void mx_user_route(t_connection *conn, t_http_message *req);

#endif
