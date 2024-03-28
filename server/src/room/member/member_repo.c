#include "server.h"

static t_list *bind_columns_to_members(sqlite3_stmt *stmt) {
    t_list *favs = NULL;
    t_room_member *last = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!last || sqlite3_column_int(stmt, 0) != last->id) {
            last = mx_sqlite_bind_columns_to_member(stmt, 0);
            last->user = mx_sqlite_bind_columns_to_user(stmt, 6);

            if (last->user) {
                last->user->photo = mx_sqlite_bind_columns_to_file(stmt, 15);
                last->user->password_hash = NULL;
            }
            mx_push_back(&favs, last);
        }
    }

    return favs;
}

t_room_member *mx_members_repo_get(int id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT r_m.*, u.*, uf.* "
                      "FROM room_member r_m "
                      "LEFT JOIN user u ON r_m.user_id = u.id "
                      "LEFT JOIN file uf ON u.photo_id = uf.id "
                      "WHERE r_m.id = ? ;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, id);

    t_list *members = bind_columns_to_members(stmt);
    t_room_member *mem = members ? members->data : NULL;

    mx_clear_list(&members);
    sqlite3_finalize(stmt);

    return mem;
}

int mx_members_repo_count(int room_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(r_m.id) "
                      "FROM room_member r_m "
                      "WHERE r_m.room_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }

    mx_sqlite3_bind_id(stmt, 1, room_id);

    int count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    return count;
}

t_room_member *mx_members_repo_get_by(int user_id, int room_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT r_m.*, u.*, uf.* "
                      "FROM room_member r_m "
                      "LEFT JOIN user u ON r_m.user_id = u.id "
                      "LEFT JOIN file uf ON u.photo_id = uf.id "
                      "WHERE r_m.user_id = ? AND r_m.room_id = ? "
                      "LIMIT 1;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, user_id);
    mx_sqlite3_bind_id(stmt, 2, room_id);

    t_list *members = bind_columns_to_members(stmt);
    t_room_member *mem = members ? members->data : NULL;

    mx_clear_list(&members);
    sqlite3_finalize(stmt);

    return mem;
}

int mx_members_repo_create(int user_id, int room_id, bool is_admin) {
    sqlite3 *db = mx_env_get()->db_connection;
    int pine_id = -1;
    sqlite3_stmt *stmt;

    const char *sql = "INSERT INTO room_member "
                      "(user_id, room_id, is_admin) "
                      "VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    mx_sqlite3_bind_id(stmt, 1, user_id);
    mx_sqlite3_bind_id(stmt, 2, room_id);
    sqlite3_bind_int(stmt, 3, is_admin);

    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        pine_id = sqlite3_last_insert_rowid(db);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    } else {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    }
    sqlite3_finalize(stmt);

    return pine_id;
}

bool mx_members_repo_update(int id, bool is_admin) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (id <= 0) {
        return false;
    }
    const char *sql = "UPDATE room_member "
                      "SET is_admin = ? "
                      "WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    sqlite3_bind_int(stmt, 1, is_admin);
    mx_sqlite3_bind_id(stmt, 2, id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}

t_list *mx_members_repo_get_many(int room_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT r_m.*, u.*, uf.* "
                      "FROM room_member r_m "
                      "LEFT JOIN user u ON r_m.user_id = u.id "
                      "LEFT JOIN file uf ON u.photo_id = uf.id "
                      "WHERE r_m.room_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, room_id);

    t_list *members = bind_columns_to_members(stmt);

    sqlite3_finalize(stmt);

    return members;
}

bool mx_members_repo_delete(int id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (id <= 0) {
        return false;
    }

    const char *sql = "DELETE FROM room_member "
                      "WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }
    mx_sqlite3_bind_id(stmt, 1, id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}
