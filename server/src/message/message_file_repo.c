#include "server.h"

int mx_message_file_repo_create(int message_id, int file_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    int message_file_id = -1;
    sqlite3_stmt *stmt;

    const char *sql = "INSERT INTO message_file "
                      "(message_id, file_id) "
                      "VALUES (?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    mx_sqlite3_bind_id(stmt, 1, message_id);
    mx_sqlite3_bind_id(stmt, 2, file_id);

    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        message_file_id = sqlite3_last_insert_rowid(db);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    } else {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    }
    sqlite3_finalize(stmt);

    return message_file_id;
}

bool mx_message_file_repo_delete(int file_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (file_id <= 0) {
        return false;
    }

    const char *sql = "DELETE FROM message_file "
                      "WHERE file_id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }
    mx_sqlite3_bind_id(stmt, 1, file_id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}
