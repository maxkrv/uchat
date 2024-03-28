#include "server.h"

static t_list *bind_columns_to_stickers(sqlite3_stmt *stmt) {
    t_list *files = NULL;
    t_file *last = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        last = mx_sqlite_bind_columns_to_file(stmt, 0);
        mx_push_back(&files, last);
    }

    return files;
}

bool mx_sticker_repo_delete(int user_id, int file_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (user_id <= 0 || file_id <= 0) {
        return false;
    }

    const char *sql = "DELETE FROM user_sticker "
                      "WHERE user_id = ? AND file_id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }
    mx_sqlite3_bind_id(stmt, 1, user_id);
    mx_sqlite3_bind_id(stmt, 2, file_id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}

int mx_sticker_repo_create(int user_id, int file_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    t_user_id sticker_id = -1;
    sqlite3_stmt *stmt;

    if (user_id <= 0 || file_id <= 0) {
        return -1;
    }
    const char *sql = "INSERT INTO user_sticker "
                      "(user_id, file_id)"
                      "VALUES (?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    mx_sqlite3_bind_id(stmt, 1, user_id);
    mx_sqlite3_bind_id(stmt, 2, file_id);

    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        sticker_id = sqlite3_last_insert_rowid(db);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    } else {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    }
    sqlite3_finalize(stmt);

    return sticker_id;
}

t_list *mx_sticker_repo_get_many(int user_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT f.* "
                      "FROM user_sticker u_s "
                      "LEFT JOIN file f ON u_s.file_id = f.id "
                      "WHERE u_s.user_id = ? ;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, user_id);

    t_list *files = bind_columns_to_stickers(stmt);

    sqlite3_finalize(stmt);

    return files;
}

t_file *mx_sticker_repo_get(int user_id, int file_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT f.* "
                      "FROM user_sticker u_s "
                      "LEFT JOIN file f ON u_s.file_id = f.id "
                      "WHERE u_s.user_id = ? AND u_s.file_id = ? "
                      "LIMIT 1;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, user_id);
    mx_sqlite3_bind_id(stmt, 2, file_id);

    t_list *files = bind_columns_to_stickers(stmt);
    t_file *file = files ? files->data : NULL;

    mx_clear_list(&files);
    sqlite3_finalize(stmt);

    return file;
}
