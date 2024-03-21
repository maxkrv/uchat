#ifndef UCHAT_H
#define UCHAT_H

#include "base.h"
#include "utils.h"
#include "shared.h"
#include "http.h"
#include "../inc/auth.h"
#include "room.h"
#include "message.h"
#include "db.h"
#include "auth.h"
#include "file.h"

int mx_run_server(t_env_params *env, char *addr);
void mx_http_request_handler(t_connection *c, int ev, void *ev_data);
void mx_free_file(t_file *file); // free file memory- file_repo
void mx_free_member(t_room_member *member);
#endif
