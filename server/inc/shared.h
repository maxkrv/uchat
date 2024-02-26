#ifndef BACKEND_SHARED_H
#define BACKEND_SHARED_H

#include "base.h"
#include "utils.h"
#include "structs.h"

typedef void t_handler(t_connection *c, t_http_message *m);

typedef struct s_crud_router {
    t_handler *create;
    t_handler *read;
    t_handler *update;
    t_handler *delete;
} t_crud_router;

void mx_crud_route(t_crud_router *router, t_connection *conn,
                   t_http_message *req);
int mx_extract_id_from_query(t_string query);

#endif
