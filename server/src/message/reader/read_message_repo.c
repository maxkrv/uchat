#include "server.h"

static t_list *bind_columns_to_readers(sqlite3_stmt *stmt) {
    t_list *readers = NULL;
    t_read_message *last = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!last || sqlite3_column_int(stmt, 0) != last->id) {
            last = mx_sqlite_bind_columns_to_reader(stmt, 0);
            last->user = mx_sqlite_bind_columns_to_user(stmt, 5);

            if (last->user) {
                last->user->photo = mx_sqlite_bind_columns_to_file(stmt, 14);
                last->user->password_hash = NULL;
            }
            mx_push_back(&readers, last);
        }
    }

    return readers;
}

t_read_message *mx_read_message_repo_get(int id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT m_r.*, u.*, uf.* "
                      "FROM message_reader m_r "
                      "LEFT JOIN user u ON m_r.reader_id = u.id "
                      "LEFT JOIN file uf ON u.photo_id = uf.id "
                      "WHERE m_r.id = ? ;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, id);

    t_list *reads = bind_columns_to_readers(stmt);
    t_read_message *reader = reads ? reads->data : NULL;

    mx_clear_list(&reads);
    sqlite3_finalize(stmt);

    return reader;
}

t_list *mx_read_messages_repo_get(int message_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT m_r.*, u.*, uf.* "
                      "FROM message_reader m_r "
                      "LEFT JOIN user u ON m_r.reader_id = u.id "
                      "LEFT JOIN file uf ON u.photo_id = uf.id "
                      "WHERE m_r.message_id = ? ;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, message_id);

    t_list *readers = bind_columns_to_readers(stmt);
    sqlite3_finalize(stmt);

    return readers;
}

int mx_message_repo_read(int user_id, int message_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    int pine_id = -1;
    sqlite3_stmt *stmt;

    const char *sql = "INSERT INTO message_reader "
                      "(reader_id, message_id) "
                      "VALUES (?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    mx_sqlite3_bind_id(stmt, 1, user_id);
    mx_sqlite3_bind_id(stmt, 2, message_id);

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
