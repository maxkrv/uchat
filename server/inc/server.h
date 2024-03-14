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

int mx_run_server(t_env_params *env, t_string addr);

void mx_http_request_handler(t_connection *c, int ev, void *ev_data);

t_env_params *mx_get_env(void);
void mx_set_env(t_env_params *e);
t_env_params *mx_create_env();
void mx_init_env(t_env_params *env, int argc, char *argv[]);
void mx_close_env(t_env_params *env);

void mx_usage(t_string program_name);
#endif
