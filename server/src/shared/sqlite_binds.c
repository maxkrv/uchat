#include "server.h"

t_message *mx_sqlite_bind_columns_to_message(sqlite3_stmt *stmt, int from) {
    t_message *message = mx_message_init();

    message->id = sqlite3_column_int(stmt, from + 0);
    message->author_id = sqlite3_column_int(stmt, from + 1);
    message->room_id = sqlite3_column_int(stmt, from + 2);
    message->reply_id = sqlite3_column_int(stmt, from + 3);
    message->text = mx_sqlite3_get_string(stmt, from + 4);
    message->created_at = sqlite3_column_int(stmt, from + 5);
    message->edited_at = sqlite3_column_int(stmt, from + 6);

    return message;
}

t_file *mx_sqlite_bind_columns_to_file(sqlite3_stmt *stmt, int from) {
    t_file *file = mx_file_init();

    file->id = sqlite3_column_int(stmt, from + 0);
    file->name = mx_sqlite3_get_string(stmt, from + 1);
    file->url = mx_sqlite3_get_string(stmt, from + 2);
    file->created_at = sqlite3_column_int(stmt, from + 3);
    file->edited_at = sqlite3_column_int(stmt, from + 4);

    return file;
}

t_room *mx_sqlite_bind_columns_to_room(sqlite3_stmt *stmt, int from) {
    t_room *room = mx_room_init();

    room->id = sqlite3_column_int(stmt, from + 0);
    room->name = mx_sqlite3_get_string(stmt, from + 1);
    room->photo_id = sqlite3_column_int(stmt, from + 2);
    room->type = mx_sqlite3_get_string(stmt, from + 3);
    room->description = mx_sqlite3_get_string(stmt, from + 4);
    room->created_at = sqlite3_column_int(stmt, from + 5);
    room->edited_at = sqlite3_column_int(stmt, from + 6);

    return room;
}

t_user *mx_sqlite_bind_columns_to_user(sqlite3_stmt *stmt, int from) {
    t_user *user = mx_user_init();

    user->id = sqlite3_column_int(stmt, from + 0);
    user->name = mx_sqlite3_get_string(stmt, from + 1);
    user->tag = mx_sqlite3_get_string(stmt, from + 2);
    user->password_hash = mx_sqlite3_get_string(stmt, from + 3);
    user->photo_id = sqlite3_column_int(stmt, from + 4);
    user->status = mx_sqlite3_get_string(stmt, from + 5);
    user->description = mx_sqlite3_get_string(stmt, from + 6);
    user->created_at = sqlite3_column_int(stmt, from + 7);
    user->edited_at = sqlite3_column_int(stmt, from + 8);

    return user;
}
