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
#include "auth.h"
#include "file.h"
#include "ws.h"

int mx_run_server(t_env_params *env, t_string addr);

void mx_http_msg_route(t_connection *conn, t_http_message *req);
void mx_event_handler(t_connection *conn, int event, void *data);

t_env_params *mx_env_get(void);
void mx_env_set(t_env_params *e);
t_env_params *mx_create_env(void);
void mx_env_init(t_env_params *env, int argc, char *argv[]);
void mx_env_close(t_env_params *env);

void mx_usage(t_string program_name);
#endif
