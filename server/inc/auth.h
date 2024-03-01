#ifndef BACKEND_AUTH_H
#define BACKEND_AUTH_H

#define MX_AUTH_JWT_EXP_TIME 3600

#include "base.h"
#include "utils.h"
#include "shared.h"
#include "jwt.h"

int mx_user_id_from_auth_jwt(t_http_message *req);
char *mx_create_auth_jwt(int user_id);
int mx_verify_auth_jwt(char *jwt);

#endif
