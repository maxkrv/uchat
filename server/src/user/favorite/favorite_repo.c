#include "server.h"

static t_list *bind_columns_to_favorite(sqlite3_stmt *stmt) {
    t_list *favs = NULL;
    t_favorite_room *last = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!last || sqlite3_column_int(stmt, 0) != last->id) {
            last = mx_sqlite_bind_columns_to_favorite(stmt, 0);
            mx_push_back(&favs, last);
        }
        last->room = mx_sqlite_bind_columns_to_room(stmt, 5);

        if (last->room) {
            last->room->photo = mx_sqlite_bind_columns_to_file(stmt, 12);
        }
    }

    return favs;
}

t_favorite_room *mx_favorites_repo_get(int room_id, int user_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT f_r.*, r.*, rf.* "
                      "FROM favourite_room f_r "
                      "LEFT JOIN room r ON f_r.room_id = r.id "
                      "LEFT JOIN file rf ON r.photo_id = rf.id "
                      "WHERE f_r.room_id = ? AND f_r.user_id = ? ;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, room_id);
    mx_sqlite3_bind_id(stmt, 2, user_id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return NULL;
    }
    t_favorite_room *fav = mx_sqlite_bind_columns_to_favorite(stmt, 0);
    fav->room = mx_sqlite_bind_columns_to_room(stmt, 5);
    if (fav->room) {
        fav->room->photo = mx_sqlite_bind_columns_to_file(stmt, 12);
    }

    sqlite3_finalize(stmt);

    return fav;
}
/* Returns [{discriptions + room + room.photo}] */
t_list *mx_favorites_repo_get_many(int user_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT f_r.*, r.*, rf.* "
                      "FROM favourite_room f_r "
                      "LEFT JOIN room r ON f_r.room_id = r.id "
                      "LEFT JOIN file rf ON r.photo_id = rf.id "
                      "WHERE f_r.user_id = ? ;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, user_id);

    t_list *favs = bind_columns_to_favorite(stmt);

    sqlite3_finalize(stmt);

    return favs;
}

int mx_favorites_repo_create(int user_id, int room_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    int fav_id = -1;
    sqlite3_stmt *stmt;

    const char *sql = "INSERT INTO favourite_room "
                      "(user_id, room_id) "
                      "VALUES (?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    mx_sqlite3_bind_id(stmt, 1, user_id);
    mx_sqlite3_bind_id(stmt, 2, room_id);

    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        fav_id = sqlite3_last_insert_rowid(db);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    } else {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    }
    sqlite3_finalize(stmt);

    return fav_id;
}

bool mx_favorites_repo_delete(int room_id, int user_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (room_id <= 0 || user_id <= 0) {
        return false;
    }

    const char *sql = "DELETE FROM favourite_room "
                      "WHERE room_id = ? AND user_id = ? ;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }
    mx_sqlite3_bind_id(stmt, 1, room_id);
    mx_sqlite3_bind_id(stmt, 2, user_id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}
