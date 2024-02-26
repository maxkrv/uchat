#include "room.h"

void mx_room_route(t_connection *conn, t_http_message *req) {
    t_crud_router router = {mx_room_ctrl_post, mx_room_ctrl_get,
                            mx_room_ctrl_put, mx_room_ctrl_delete};

    mx_crud_route(&router, conn, req);
}
