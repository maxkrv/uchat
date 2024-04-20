#include "client.h"

static void handle_message_created(t_string data) {
    t_ws_message *message =
        mx_ws_message_parse(data, (t_func_parser)mx_message_parse_cjson);
    t_message *payload = message->payload;
    // Handle message event
    MG_INFO(("Message created: %d", payload->id));

    render_messages(payload->room_id);

    mx_ws_message_free(message, (t_func_free)mx_message_free);
}

static void handle_message_updated(t_string data) {
    t_ws_message *message =
        mx_ws_message_parse(data, (t_func_parser)mx_message_parse_cjson);
    t_message *payload = message->payload;
    // Handle message event
    MG_INFO(("Message updated: %d", payload->id));

    render_messages(payload->room_id);

    mx_ws_message_free(message, (t_func_free)mx_message_free);
}

static void handle_message_deleted(t_string data) {
    t_ws_message *message =
        mx_ws_message_parse(data, (t_func_parser)mx_message_parse_cjson);
    t_message *payload = message->payload;
    // Handle message event
    MG_INFO(("Message deleted: %d", payload->id));

    render_messages(payload->room_id);

    mx_ws_message_free(message, (t_func_free)mx_message_free);
}

static void handle_member_joined(t_string data) {
    t_ws_message *message =
        mx_ws_message_parse(data, (t_func_parser)mx_member_parse_cjson);
    t_room_member *payload = message->payload;
    // Handle message event
    MG_INFO(("Member joined: %d", payload->id));

    render_rooms();
    render_room_members(payload->room_id);
}

static void handle_member_left(t_string data) {
    t_ws_message *message =
        mx_ws_message_parse(data, (t_func_parser)mx_member_parse_cjson);
    t_room_member *payload = message->payload;
    // Handle message event
    MG_INFO(("Member left: %d", payload->id));

    render_room_members(payload->room_id);

    mx_ws_message_free(message, (t_func_free)mx_room_member_free);
}

static void handle_member_updated(t_string data) {
    t_ws_message *message =
        mx_ws_message_parse(data, (t_func_parser)mx_user_parse_cjson);
    t_user *payload = message->payload;
    // Handle message event
    MG_INFO(("Member updated: %d", payload->id));

    mx_ws_message_free(message, (t_func_free)mx_user_free);
}

static void handle_message_pinned(t_string data) {
    t_ws_message *message =
        mx_ws_message_parse(data, (t_func_parser)mx_pined_parse_cjson);
    t_pined_message *payload = message->payload;
    // Handle message event
    MG_INFO(("Message pinned: %d", payload->id));

    show_pin_message(payload->room_id);

    mx_ws_message_free(message, (t_func_free)mx_room_pined_free);
}

static void handle_message_unpinned(t_string data) {
    t_ws_message *message =
        mx_ws_message_parse(data, (t_func_parser)mx_pined_parse_cjson);
    t_pined_message *payload = message->payload;
    // Handle message event
    MG_INFO(("Message unpinned: %d", payload->id));

    mx_ws_message_free(message, (t_func_free)mx_room_pined_free);
}

static void handle_room_updated(t_string data) {
    t_ws_message *message =
        mx_ws_message_parse(data, (t_func_parser)mx_room_parse_cjson);
    t_room *payload = message->payload;
    // Handle message event
    MG_INFO(("Room updated: %d", payload->id));

    render_rooms();

    mx_ws_message_free(message, (t_func_free)mx_room_free);
}

static void handle_room_deleted(t_string data) {
    t_ws_message *message =
        mx_ws_message_parse(data, (t_func_parser)mx_room_parse_cjson);
    t_room *payload = message->payload;
    // Handle message event
    MG_INFO(("Room deleted: %d", payload->id));

    render_rooms();

    mx_ws_message_free(message, (t_func_free)mx_room_free);
}

static void handle_message_was_readed(t_string data) {
    t_ws_message *message =
        mx_ws_message_parse(data, (t_func_parser)mx_reader_parse_cjson);
    t_read_message *payload = message->payload;
    // Handle message event
    MG_INFO(("Message was readed: %d", payload->message_id));

    mx_ws_message_free(message, (t_func_free)mx_read_message_free);
}

/**
 * Compares two strings for equality.
 */
bool mx_streq(const char *s1, const char *s2) {
    return strcmp(s1, s2) == 0;
}

/**
 * Handles WebSocket events.
 *
 * This function is responsible for handling WebSocket events received from the
 * server. It takes in a connection object, the event name, and the event data
 * as parameters.
 *
 * @param conn The WebSocket connection object.
 * @param event The name of the WebSocket event.
 * @param data The data recived from server(it includes event). Use
 * mx_ws_message_parse to parse it.
 */
void mx_ws_event_handler(struct mg_connection *conn, t_string event,
                         t_string data) {
    if (mx_streq(event, "message-created")) {
        handle_message_created(data);
    } else if (mx_streq(event, "message-updated")) {
        handle_message_updated(data);
    } else if (mx_streq(event, "message-deleted")) {
        handle_message_deleted(data);
    } else if (mx_streq(event, "member-joined")) {
        handle_member_joined(data);
    } else if (mx_streq(event, "member-left")) {
        handle_member_left(data);
    } else if (mx_streq(event, "member-updated")) {
        handle_member_updated(data);
    } else if (mx_streq(event, "message-pinned")) {
        handle_message_pinned(data);
    } else if (mx_streq(event, "message-unpinned")) {
        handle_message_unpinned(data);
    } else if (mx_streq(event, "room-updated")) {
        handle_room_updated(data);
    } else if (mx_streq(event, "room-deleted")) {
        handle_room_deleted(data);
    } else if (mx_streq(event, "message-was-read")) {
        handle_message_was_readed(data);
    }
    (void)conn;
}
