#include "server.h"

static t_list *bind_columns_to_messages(sqlite3_stmt *stmt) {
    t_list *messages = NULL;
    t_message *last = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!last || sqlite3_column_int(stmt, 0) != last->id) {
            last = mx_sqlite_bind_columns_to_message(stmt, 0);
            last->author = mx_sqlite_bind_columns_to_user(stmt, 12);
            if (last->author) {
                last->author->photo = mx_sqlite_bind_columns_to_file(stmt, 21);
                last->author->password_hash = NULL;
                mx_push_back(&messages, last);
            }
        }
        if (sqlite3_column_int(stmt, 7) > 0) {
            mx_push_back(&last->files,
                         mx_sqlite_bind_columns_to_file(stmt, 7));
        }
    }

    return messages;
}

t_message *mx_message_repo_get(int id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT m.*, f.*, u.*, uf.* "
                      "FROM message m "
                      "LEFT JOIN message_file mf ON mf.message_id = m.id "
                      "LEFT JOIN file f ON mf.file_id = f.id "
                      "LEFT JOIN user u ON m.author_id = u.id "
                      "LEFT JOIN file uf ON u.photo_id = uf.id "
                      "WHERE m.id = ? ;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, id);

    t_list *messages = bind_columns_to_messages(stmt);
    t_message *message = messages ? messages->data : NULL;
    mx_clear_list(&messages);
    sqlite3_finalize(stmt);

    return message;
}

t_list *mx_message_repo_get_many(int room_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT m.*, f.*, u.*, uf.* "
                      "FROM message m "
                      "LEFT JOIN message_file mf ON mf.message_id = m.id "
                      "LEFT JOIN file f ON mf.file_id = f.id "
                      "LEFT JOIN user u ON m.author_id = u.id "
                      "LEFT JOIN file uf ON u.photo_id = uf.id "
                      "WHERE m.room_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, room_id);

    t_list *list = bind_columns_to_messages(stmt);

    sqlite3_finalize(stmt);

    return list;
}

int mx_message_repo_create(int author_id, t_message_create_dto *dto) {
    sqlite3 *db = mx_env_get()->db_connection;
    int message_id = -1;
    sqlite3_stmt *stmt;

    if (!dto) {
        return -1;
    }
    const char *sql = "INSERT INTO message "
                      "(room_id, reply_id, data, author_id) "
                      "VALUES (?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    mx_sqlite3_bind_id(stmt, 1, dto->room_id);
    mx_sqlite3_bind_id(stmt, 2, dto->reply_id);
    sqlite3_bind_text(stmt, 3, dto->text, -1, SQLITE_TRANSIENT);
    mx_sqlite3_bind_id(stmt, 4, author_id);

    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        message_id = sqlite3_last_insert_rowid(db);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    } else {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    }
    sqlite3_finalize(stmt);

    return message_id;
}

bool mx_message_repo_put(int id, t_message_create_dto *dto) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (!dto || id <= 0) {
        return false;
    }
    const char *sql = "UPDATE message "
                      "SET reply_id = ?, data = ? "
                      "WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    mx_sqlite3_bind_id(stmt, 1, dto->reply_id);
    sqlite3_bind_text(stmt, 2, dto->text, -1, SQLITE_TRANSIENT);
    mx_sqlite3_bind_id(stmt, 3, id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}

bool mx_message_repo_delete(int id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (id <= 0) {
        return false;
    }

    const char *sql = "DELETE FROM message "
                      "WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }
    mx_sqlite3_bind_id(stmt, 1, id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}
