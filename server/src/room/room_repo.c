#include "server.h"

static t_list *bind_columns_to_rooms(sqlite3_stmt *stmt) {
    t_list *rooms = NULL;
    t_room *last = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!last || sqlite3_column_int(stmt, 0) != last->id) {
            last = mx_sqlite_bind_columns_to_room(stmt, 0);
            last->photo = mx_sqlite_bind_columns_to_file(stmt, 7);
            mx_push_back(&rooms, last);
        }
    }

    return rooms;
}

t_room *mx_room_repo_get(int id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT r.*, f.* "
                      "FROM room r "
                      "LEFT JOIN file f ON r.photo_id = f.id "
                      "WHERE r.id = ? ;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return NULL;
    }
    t_room *room = mx_sqlite_bind_columns_to_room(stmt, 0);

    if (room) {
        room->photo = mx_sqlite_bind_columns_to_file(stmt, 7);
        sqlite3_finalize(stmt);
    }

    return room;
}

bool mx_room_repo_direct_exist(t_user_id first_id, t_user_id second_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT r.*, u.* "
                      "FROM room r "
                      "LEFT JOIN room_member r_m ON r_m.room_id = r.id "
                      "LEFT JOIN user u ON r_m.user_id = u.id "
                      "WHERE r.type =  'direct' "
                      "AND EXISTS( "
                      "SELECT 1 "
                      "FROM room_member rm_inner "
                      "WHERE rm_inner.room_id = r.id "
                      "AND rm_inner.user_id IN(?, ?)"
                      "GROUP BY rm_inner.room_id "
                      "HAVING COUNT( "
                      "DISTINCT rm_inner.user_id) = 2) LIMIT 2;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }

    mx_sqlite3_bind_id(stmt, 1, first_id);
    mx_sqlite3_bind_id(stmt, 2, second_id);

    int count = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        count++;
    }
    sqlite3_finalize(stmt);

    return count == 2;
}

t_list *mx_room_repo_get_many(int user_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT r.*, rf.* "
                      "FROM room_member r_m "
                      "LEFT JOIN room r ON r_m.room_id = r.id "
                      "LEFT JOIN file rf ON r.photo_id = rf.id "
                      "WHERE r_m.user_id = ? ;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, user_id);

    t_list *rooms = bind_columns_to_rooms(stmt);

    sqlite3_finalize(stmt);
    return rooms;
}

int mx_room_repo_create(t_room_create_dto *dto) {
    sqlite3 *db = mx_env_get()->db_connection;
    int room_id = -1;
    sqlite3_stmt *stmt;

    if (!dto) {
        return -1;
    }
    const char *sql = "INSERT INTO room "
                      "(name, description, photo_id, type) "
                      "VALUES (?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    sqlite3_bind_text(stmt, 1, dto->name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, dto->description, -1, SQLITE_TRANSIENT);
    mx_sqlite3_bind_id(stmt, 3, dto->photo_id);
    sqlite3_bind_text(stmt, 4, dto->type, -1, SQLITE_TRANSIENT);

    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        room_id = sqlite3_last_insert_rowid(db);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    } else {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    }
    sqlite3_finalize(stmt);

    return room_id;
}

bool mx_room_repo_put(int id, t_room_create_dto *dto) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (!dto || id <= 0) {
        return false;
    }
    const char *sql = "UPDATE room "
                      "SET name = ?, photo_id = ?, description = ? "
                      "WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    sqlite3_bind_text(stmt, 1, dto->name, -1, SQLITE_TRANSIENT);
    mx_sqlite3_bind_id(stmt, 2, dto->photo_id);
    sqlite3_bind_text(stmt, 3, dto->description, -1, SQLITE_TRANSIENT);
    mx_sqlite3_bind_id(stmt, 4, id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}

bool mx_room_repo_delete(int id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (id <= 0) {
        return false;
    }

    const char *sql = "DELETE FROM room "
                      "WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }
    mx_sqlite3_bind_id(stmt, 1, id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}
