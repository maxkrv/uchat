#include "server.h"
/* +7 */
t_message *mx_sqlite_bind_columns_to_message(sqlite3_stmt *stmt, int from) {
    t_message *message = NULL;

    if (sqlite3_column_int(stmt, from + 0) > 0) {
        message = mx_message_init();

        message->id = sqlite3_column_int(stmt, from + 0);
        message->author_id = sqlite3_column_int(stmt, from + 1);
        message->room_id = sqlite3_column_int(stmt, from + 2);
        message->reply_id = sqlite3_column_int(stmt, from + 3);
        message->text = mx_sqlite3_get_string(stmt, from + 4);
        message->created_at = sqlite3_column_int(stmt, from + 5);
        message->edited_at = sqlite3_column_int(stmt, from + 6);
    }
    return message;
}
/* +5 */
t_file *mx_sqlite_bind_columns_to_file(sqlite3_stmt *stmt, int from) {
    t_file *file = NULL;

    if (sqlite3_column_int(stmt, from + 0) > 0) {
        file = mx_file_init();

        file->id = sqlite3_column_int(stmt, from + 0);
        file->name = mx_sqlite3_get_string(stmt, from + 1);
        file->url = mx_sqlite3_get_string(stmt, from + 2);
        file->created_at = sqlite3_column_int(stmt, from + 3);
        file->edited_at = sqlite3_column_int(stmt, from + 4);
    }

    return file;
}
/* +7 */
t_room *mx_sqlite_bind_columns_to_room(sqlite3_stmt *stmt, int from) {
    t_room *room = NULL;

    if (sqlite3_column_int(stmt, from + 0) > 0) {
        room = mx_room_init();

        room->id = sqlite3_column_int(stmt, from + 0);
        room->name = mx_sqlite3_get_string(stmt, from + 1);
        room->photo_id = sqlite3_column_int(stmt, from + 2);
        room->type = mx_sqlite3_get_string(stmt, from + 3);
        room->description = mx_sqlite3_get_string(stmt, from + 4);
        room->created_at = sqlite3_column_int(stmt, from + 5);
        room->edited_at = sqlite3_column_int(stmt, from + 6);
    }

    return room;
}
/* +9 */
t_user *mx_sqlite_bind_columns_to_user(sqlite3_stmt *stmt, int from) {
    t_user *user = NULL;

    if (sqlite3_column_int(stmt, from + 0) > 0) {
        user = mx_user_init();

        user->id = sqlite3_column_int(stmt, from + 0);
        user->name = mx_sqlite3_get_string(stmt, from + 1);
        user->tag = mx_sqlite3_get_string(stmt, from + 2);
        user->password_hash = mx_sqlite3_get_string(stmt, from + 3);
        user->photo_id = sqlite3_column_int(stmt, from + 4);
        user->status = mx_sqlite3_get_string(stmt, from + 5);
        user->description = mx_sqlite3_get_string(stmt, from + 6);
        user->created_at = sqlite3_column_int(stmt, from + 7);
        user->edited_at = sqlite3_column_int(stmt, from + 8);
    }

    return user;
}
/* +5 */
t_favorite_room *mx_sqlite_bind_columns_to_favorite(sqlite3_stmt *stmt,
                                                    int from) {
    t_favorite_room *fav = NULL;

    if (sqlite3_column_int(stmt, from + 0) > 0) {
        fav = mx_favorite_room_init();

        fav->id = sqlite3_column_int(stmt, from + 0);
        fav->room_id = sqlite3_column_int(stmt, from + 1);
        fav->user_id = sqlite3_column_int(stmt, from + 2);
        fav->created_at = sqlite3_column_int(stmt, from + 3);
        fav->edited_at = sqlite3_column_int(stmt, from + 4);
    }

    return fav;
}
/* +5 */
t_pined_message *mx_sqlite_bind_columns_to_pine(sqlite3_stmt *stmt, int from) {
    t_pined_message *pine = NULL;

    if (sqlite3_column_int(stmt, from + 0) > 0) {
        pine = mx_room_pined_init();

        pine->id = sqlite3_column_int(stmt, from + 0);
        pine->room_id = sqlite3_column_int(stmt, from + 1);
        pine->message_id = sqlite3_column_int(stmt, from + 2);
        pine->created_at = sqlite3_column_int(stmt, from + 3);
        pine->edited_at = sqlite3_column_int(stmt, from + 4);
    }

    return pine;
}
/* +6 */
t_room_member *mx_sqlite_bind_columns_to_member(sqlite3_stmt *stmt, int from) {
    t_room_member *mem = NULL;

    if (sqlite3_column_int(stmt, from + 0) > 0) {
        mem = mx_room_member_init();

        mem->id = sqlite3_column_int(stmt, from + 0);
        mem->user_id = sqlite3_column_int(stmt, from + 1);
        mem->room_id = sqlite3_column_int(stmt, from + 2);
        mem->is_admin = sqlite3_column_int(stmt, from + 3);
        mem->created_at = sqlite3_column_int(stmt, from + 4);
        mem->edited_at = sqlite3_column_int(stmt, from + 5);
    }

    return mem;
}
/* +5 */
t_read_message *mx_sqlite_bind_columns_to_reader(sqlite3_stmt *stmt,
                                                 int from) {
    t_read_message *mem = NULL;

    if (sqlite3_column_int(stmt, from + 0) > 0) {
        mem = mx_read_message_init();

        mem->id = sqlite3_column_int(stmt, from + 0);
        mem->user_id = sqlite3_column_int(stmt, from + 1);
        mem->message_id = sqlite3_column_int(stmt, from + 2);
        mem->created_at = sqlite3_column_int(stmt, from + 4);
        mem->edited_at = sqlite3_column_int(stmt, from + 5);
    }

    return mem;
}
