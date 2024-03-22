#include "server.h"

static t_list *bind_columns_to_rooms(sqlite3_stmt *stmt) {
    t_list *rooms = NULL;
    t_room *last = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!last || sqlite3_column_int(stmt, 0) != last->id) {
            last = mx_sqlite_bind_columns_to_room(stmt, 0);
            mx_push_back(&rooms, last);
        }
        if (sqlite3_column_int(stmt, 7) > 0) {
            last->photo = mx_sqlite_bind_columns_to_file(stmt, 7);
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

    sqlite3_bind_int(stmt, 1, id);

    t_list *rooms = bind_columns_to_rooms(stmt);
    t_room *room = rooms ? rooms->data : NULL;
    mx_clear_list(&rooms);
    sqlite3_finalize(stmt);

    return room;
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
                      "SET name = ?, photo_id = ?, type = ?, description = ? "
                      "WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    sqlite3_bind_text(stmt, 1, dto->name, -1, SQLITE_TRANSIENT);
    mx_sqlite3_bind_id(stmt, 2, dto->photo_id);
    sqlite3_bind_text(stmt, 3, dto->type, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, dto->description, -1, SQLITE_TRANSIENT);
    mx_sqlite3_bind_id(stmt, 5, id);

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
    sqlite3_bind_int(stmt, 2, id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}
