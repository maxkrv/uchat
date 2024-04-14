#include "client.h"

static struct mg_connection *ws_connect(const char *url, const char *token);

static struct mg_mgr *ws_client_mgr = NULL;
static struct mg_connection *ws_conn = NULL;
static t_string ws_url = NULL;

gboolean websocket_server_push_events(gpointer user_data) {
    if (ws_client_mgr) {
        mg_mgr_poll(ws_client_mgr, 0);
    }
    (void)user_data;

    return TRUE;
}

static t_string ws_message_parse_event(t_string data) {
    if (!data) {
        return NULL;
    }

    cJSON *json = cJSON_Parse(data);

    if (!json) {
        return NULL;
    }
    t_string event = mx_cjson_get_string(json, "event");

    cJSON_Delete(json);
    return event;
}

gboolean ws_reconect(gpointer user_data) {
    if (!ws_client_mgr || !ws_url || !mx_sdk_env_get()->jwt_token) {
        return G_SOURCE_CONTINUE;
    }

    (void)user_data;
    ws_connect(ws_url, mx_sdk_env_get()->jwt_token);

    return G_SOURCE_REMOVE;
}

static void websocket_event_handler(struct mg_connection *conn, int event_type,
                                    void *event_data) {
    if (event_type == MG_EV_WS_OPEN) {
        MG_INFO(("WebSocket connection opened"));
        ws_conn = conn;
    } else if (event_type == MG_EV_CLOSE) {
        if (conn == ws_conn) {
            MG_INFO(("WebSocket connection closed"));
        }
        ws_conn = NULL;
        g_timeout_add_seconds(1, ws_reconect, NULL);
    } else if (event_type == MG_EV_WS_MSG) {
        struct mg_ws_message *wm = (struct mg_ws_message *)event_data;

        if (wm->data.ptr == NULL) {
            return;
        }
        t_string data = mx_strndup(wm->data.ptr, wm->data.len);
        t_string event = ws_message_parse_event(data);

        if (!event || !data) {
            return;
        }
        mx_ws_event_handler(conn, event, data);

        mx_strdel(&event);
        mx_strdel(&data);
    }
}

static struct mg_connection *ws_connect(const char *url, const char *token) {
    struct mg_connection *conn =
        mg_ws_connect(ws_client_mgr, url, websocket_event_handler, NULL,
                      "Authorization: Bearer %s\r\n", token);
    if (!conn || conn->is_closing || conn->is_draining) {
        MG_INFO(("Cannot connect to %s", url));
        return NULL;
    }
    return conn;
}

void ws_client_init(const char *url) {
    ws_client_mgr = (struct mg_mgr *)malloc(sizeof(struct mg_mgr));
    mg_mgr_init(ws_client_mgr);
    ws_url = mx_strdup(url);

    g_timeout_add_seconds(1, websocket_server_push_events, NULL);
    g_timeout_add_seconds(1, ws_reconect, NULL);
}

void ws_client_free(void) {
    mg_mgr_free(ws_client_mgr);
    mx_strdel(&ws_url);

    ws_client_mgr = NULL;
    ws_url = NULL;
    ws_conn = NULL;
}
