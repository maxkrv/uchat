#ifndef UCHAT_H
#define UCHAT_H

#include "base.h"
#include "utils.h"
#include "shared.h"
#include "http.h"
#include "user.h"
#include "room.h"
#include "message.h"
#include "db.h"

int mx_run_server(t_env_params *env, char *addr);
void mx_http_request_handler(t_connection *c, int ev, void *ev_data);

#endif
