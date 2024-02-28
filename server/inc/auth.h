#ifndef BACKEND_AUTH_H
#define BACKEND_AUTH_H

#include "base.h"
#include "utils.h"
#include "shared.h"

int mx_parse_jwt_auth_token(t_http_message *req);

#endif
