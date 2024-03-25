#ifndef BACKEND_AUTH_H
#define BACKEND_AUTH_H

#define MX_AUTH_JWT_EXP_TIME 2592000

#include "base.h"
#include "utils.h"
#include "shared.h"
#include "jwt.h"
#include "user.h"

typedef struct {
    char *name;
    char *password;
} t_login_dto;

typedef struct {
    char *old_password;
    char *new_password;
} t_change_password_dto;

typedef struct s_user_create_dto t_register_dto;
// auth router
void mx_auth_route(t_connection *conn, t_http_message *req);

// structs constructors and destructors
t_login_dto *mx_login_dto_get(struct mg_str body);
void mx_login_dto_free(t_login_dto *dto);
t_change_password_dto *mx_change_password_dto_get(struct mg_str body);
void mx_change_password_dto_free(t_change_password_dto *dto);

// auth helpers
bool mx_is_valid_password(char *password);
t_string mx_token_stringify(t_jwt_token t);
int mx_auth(t_http_message *req);
char *mx_create_auth_jwt(int user_id);
int mx_verify_auth_jwt(char *jwt);

// controllers
void mx_auth_ctrl_login(t_connection *c, t_http_message *m);
void mx_auth_ctrl_change_password(t_connection *c, t_http_message *m);
void mx_auth_ctrl_register(t_connection *c, t_http_message *m);

// services
t_jwt_token mx_auth_login(t_login_dto *dto);
t_user *mx_auth_register(t_register_dto *dto);
t_user *mx_auth_change_password(int id, t_change_password_dto *dto);

#endif
