#include "user.h"

void mx_user_route(t_connection *conn, t_http_message *req) {
    static t_crud_router router = {mx_user_ctrl_post,
                                   mx_user_ctrl_get,
                                   mx_user_ctrl_put,
                                   mx_user_ctrl_delete};

    mx_crud_route(&router, conn, req);
}
