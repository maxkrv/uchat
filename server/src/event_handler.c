#include "server.h"

static void http_event_handler(t_connection *conn, int event, void *data) {
    if (event == MG_EV_HTTP_MSG) {
        t_http_message *req = data;

        mx_http_msg_route(conn, req);
        // Log request
        MG_INFO(("%.*s %.*s %lu -> %.*s %lu", req->method.len, req->method.ptr,
                 req->uri.len, req->uri.ptr, req->body.len, 3,
                 conn->send.buf + 9, conn->send.len));
        return;
    }
}

static void ws_create_client(t_connection *conn, t_http_message *req) {
    t_ws_server *server = mx_ws_server_get();
    t_user_id user_id = mx_auth(req);

    if (user_id <= 0) {
        mx_http_reply_exception(conn, req, HTTP_STATUS_UNAUTHORIZED,
                                "Invalid token provided");
        return;
    }
    t_list *rooms = mx_room_repo_get_many(user_id);
    t_client *client = mx_create_client(conn, user_id, rooms);

    mx_push_back((t_list **)&server->clients, client);
}

static void ws_close_client(t_connection *conn) {
    t_ws_server *server = mx_ws_server_get();
    t_client *client = NULL;

    for (t_clients_list *clients = server->clients; clients;
         clients = clients->next) {
        t_client *c = clients->data;

        if (c->connection == conn) {
            client = c;
            break;
        }
    }
    mx_delete_node((t_list **)&server->clients, (t_list *)client,
                   (t_func_free)mx_delete_client);
}

static void ws_event_handler(t_connection *conn, int event, void *data) {
    if (event == MG_EV_WS_OPEN) {
        t_http_message *req = data;
        ws_create_client(conn, req);
        MG_INFO(("## WS new client created ##"));
        return;
    }
    if (event == MG_EV_CLOSE) {
        if (conn->is_websocket) {
            ws_close_client(conn);
            MG_INFO(("## WS client closed ##"));
        }
        return;
    }
    if (event == MG_EV_WS_MSG) {
        struct mg_ws_message *msg = data;
        MG_INFO(("## WS new message from client ##"));
        struct mg_str resp = mg_str("Sorry I dont listen any ws messages");
        mg_ws_send(conn, resp.ptr, resp.len, WEBSOCKET_OP_TEXT);
    }
}

void mx_event_handler(t_connection *conn, int event, void *data) {
    if (event == MG_EV_READ) {
        if (conn->recv.len > MX_MAX_FILE_SIZE) {
            MG_ERROR(("Msg too large: %d", conn->recv.len));
            conn->is_draining = true; // close this connection
        }
        return;
    }
    http_event_handler(conn, event, data);
    ws_event_handler(conn, event, data);
}
