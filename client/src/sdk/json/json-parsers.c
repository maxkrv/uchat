#include "sdk/sdk.h"

t_file *mx_file_parse_cjson(cJSON *obj) {
    if (!obj) {
        return NULL;
    }
    t_file *file = mx_file_init();

    file->id = mx_cjson_get_number(obj, "id");
    file->name = mx_cjson_get_string(obj, "name");
    file->url = mx_cjson_get_string(obj, "url");

    file->created_at = mx_cjson_get_number(obj, "created_at");
    file->edited_at = mx_cjson_get_number(obj, "edited_at");

    return file;
}

t_message *mx_message_parse_cjson(cJSON *obj) {
    if (!obj) {
        return NULL;
    }
    t_message *message = mx_message_init();

    message->id = mx_cjson_get_number(obj, "id");
    message->room_id = mx_cjson_get_number(obj, "room_id");
    message->reply_id = mx_cjson_get_number(obj, "reply_id");
    message->author_id = mx_cjson_get_number(obj, "author_id");
    message->text = mx_cjson_get_string(obj, "text");
    message->created_at = mx_cjson_get_number(obj, "created_at");
    message->edited_at = mx_cjson_get_number(obj, "edited_at");

    message->author = mx_user_parse_cjson(cJSON_GetKey(obj, "author"));
    message->readed_by = mx_cjson_parse_array(
        cJSON_GetKey(obj, "readed_by"), (t_func_parser)mx_reader_parse_cjson);
    message->room = mx_room_parse_cjson(cJSON_GetKey(obj, "room"));
    message->reply = mx_message_parse_cjson(cJSON_GetKey(obj, "reply"));
    message->files = mx_cjson_parse_array((cJSON_GetKey(obj, "files")),
                                          (t_func_parser)mx_file_parse_cjson);

    return message;
}

t_read_message *mx_reader_parse_cjson(cJSON *obj) {
    if (!obj) {
        return NULL;
    }
    t_read_message *message = mx_read_message_init();

    message->id = mx_cjson_get_number(obj, "id");
    message->user_id = mx_cjson_get_number(obj, "user_id");
    message->message_id = mx_cjson_get_number(obj, "message_id");

    message->created_at = mx_cjson_get_number(obj, "created_at");
    message->edited_at = mx_cjson_get_number(obj, "edited_at");
    message->user = mx_user_parse_cjson(cJSON_GetKey(obj, "user"));
    message->message = mx_message_parse_cjson(cJSON_GetKey(obj, "message"));

    return message;
}

t_room *mx_room_parse_cjson(cJSON *obj) {
    if (!obj) {
        return NULL;
    }
    t_room *room = mx_room_init();

    room->id = mx_cjson_get_number(obj, "id");
    room->name = mx_cjson_get_string(obj, "name");
    room->type = mx_cjson_get_string(obj, "type");
    room->photo_id = mx_cjson_get_number(obj, "photo_id");
    room->description = mx_cjson_get_string(obj, "description");

    room->created_at = mx_cjson_get_number(obj, "created_at");
    room->edited_at = mx_cjson_get_number(obj, "edited_at");
    room->members = mx_cjson_parse_array(cJSON_GetKey(obj, "members"),
                                         (t_func_parser)mx_member_parse_cjson);
    room->messages = mx_cjson_parse_array(
        cJSON_GetKey(obj, "messages"), (t_func_parser)mx_message_parse_cjson);
    room->pined_messages = mx_cjson_parse_array(
        cJSON_GetKey(obj, "messages"), (t_func_parser)mx_pined_parse_cjson);
    room->photo = mx_file_parse_cjson(cJSON_GetKey(obj, "photo"));

    return room;
}
t_pined_message *mx_pined_parse_cjson(cJSON *obj) {
    if (!obj) {
        return NULL;
    }
    t_pined_message *message = mx_room_pined_init();

    message->id = mx_cjson_get_number(obj, "id");
    message->room_id = mx_cjson_get_number(obj, "room_id");
    message->message_id = mx_cjson_get_number(obj, "message_id");

    message->created_at = mx_cjson_get_number(obj, "created_at");
    message->edited_at = mx_cjson_get_number(obj, "edited_at");
    message->room = mx_room_parse_cjson(cJSON_GetKey(obj, "room"));
    message->message = mx_message_parse_cjson(cJSON_GetKey(obj, "message"));

    return message;
}
t_room_member *mx_member_parse_cjson(cJSON *obj) {
    if (!obj) {
        return NULL;
    }
    t_room_member *member = mx_room_member_init();

    member->id = mx_cjson_get_number(obj, "id");
    member->room_id = mx_cjson_get_number(obj, "room_id");
    member->user_id = mx_cjson_get_number(obj, "user_id");
    member->is_admin = mx_cjson_get_bool(obj, "is_admin");

    member->created_at = mx_cjson_get_number(obj, "created_at");
    member->edited_at = mx_cjson_get_number(obj, "edited_at");

    member->room = mx_room_parse_cjson(cJSON_GetKey(obj, "room"));
    member->user = mx_user_parse_cjson(cJSON_GetKey(obj, "user"));

    return member;
}

t_user *mx_user_parse_cjson(cJSON *obj) {
    if (!obj) {
        return NULL;
    }
    t_user *user = mx_user_init();

    user->id = mx_cjson_get_number(obj, "id");
    user->name = mx_cjson_get_string(obj, "name");
    user->tag = mx_cjson_get_string(obj, "tag");
    user->status = mx_cjson_get_string(obj, "status");
    user->description = mx_cjson_get_string(obj, "description");
    user->photo_id = mx_cjson_get_number(obj, "photo_id");

    user->created_at = mx_cjson_get_number(obj, "created_at");
    user->edited_at = mx_cjson_get_number(obj, "edited_at");

    user->rooms = mx_cjson_parse_array(cJSON_GetKey(obj, "rooms"),
                                       (t_func_parser)mx_room_parse_cjson);
    user->favorites =
        mx_cjson_parse_array(cJSON_GetKey(obj, "favorites"),
                             (t_func_parser)mx_favorite_parse_cjson);
    user->messages = mx_cjson_parse_array(
        cJSON_GetKey(obj, "messages"), (t_func_parser)mx_message_parse_cjson);
    user->photo = mx_file_parse_cjson(cJSON_GetKey(obj, "photo"));

    return user;
}

t_favorite_room *mx_favorite_parse_cjson(cJSON *obj) {
    if (!obj) {
        return NULL;
    }
    t_favorite_room *fav = mx_favorite_room_init();

    fav->id = mx_cjson_get_number(obj, "id");
    fav->room_id = mx_cjson_get_number(obj, "room_id");
    fav->user_id = mx_cjson_get_number(obj, "user_id");

    fav->created_at = mx_cjson_get_number(obj, "created_at");
    fav->edited_at = mx_cjson_get_number(obj, "edited_at");

    fav->room = mx_room_parse_cjson(cJSON_GetKey(obj, "room"));
    fav->user = mx_user_parse_cjson(cJSON_GetKey(obj, "user"));

    return fav;
}
