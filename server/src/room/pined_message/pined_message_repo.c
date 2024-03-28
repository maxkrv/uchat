#include "server.h"

static t_list *bind_columns_to_pines(sqlite3_stmt *stmt) {
    t_list *favs = NULL;
    t_pined_message *last = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!last || sqlite3_column_int(stmt, 0) != last->id) {
            last = mx_sqlite_bind_columns_to_pine(stmt, 0);
            last->message = mx_sqlite_bind_columns_to_message(stmt, 5);
            last->message->author = mx_sqlite_bind_columns_to_user(stmt, 17);
            if (last->message->author) {
                last->message->author->photo =
                    mx_sqlite_bind_columns_to_file(stmt, 26);
                last->message->author->password_hash = NULL;
            }
            mx_push_back(&favs, last);
        }
        if (sqlite3_column_int(stmt, 12) > 0) {
            mx_push_back(&last->message->files,
                         mx_sqlite_bind_columns_to_file(stmt, 12));
        }
    }

    return favs;
}

t_pined_message *mx_pined_repo_get(int id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT p_m.*, m.*, mf.*, u.*, uf.* "
                      "FROM room_pinned_message p_m "
                      "LEFT JOIN message m ON p_m.message_id = m.id "
                      "LEFT JOIN message_file m_f ON m_f.message_id = m.id "
                      "LEFT JOIN file mf ON m_f.file_id = mf.id "
                      "LEFT JOIN user u ON m.author_id = u.id "
                      "LEFT JOIN file uf ON u.photo_id = uf.id "
                      "WHERE p_m.id = ? ;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, id);

    t_list *pines = bind_columns_to_pines(stmt);
    t_pined_message *pine = pines ? pines->data : NULL;

    mx_clear_list(&pines);
    sqlite3_finalize(stmt);

    return pine;
}
/* Returns [{discriptions + room + room.photo}] */
t_list *mx_pined_repo_get_many(int room_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT p_m.*, m.*, mf.*, u.*, uf.* "
                      "FROM room_pinned_message p_m "
                      "LEFT JOIN message m ON p_m.message_id = m.id "
                      "LEFT JOIN message_file m_f ON m_f.message_id = m.id "
                      "LEFT JOIN file mf ON m_f.file_id = mf.id "
                      "LEFT JOIN user u ON m.author_id = u.id "
                      "LEFT JOIN file uf ON u.photo_id = uf.id "
                      "WHERE p_m.room_id = ? ;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, room_id);

    t_list *pines = bind_columns_to_pines(stmt);

    sqlite3_finalize(stmt);

    return pines;
}

int mx_pined_repo_create(int message_id, int room_id) {
    sqlite3 *db = mx_env_get()->db_connection;
    int pine_id = -1;
    sqlite3_stmt *stmt;

    const char *sql = "INSERT INTO room_pinned_message "
                      "(message_id, room_id) "
                      "VALUES (?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    mx_sqlite3_bind_id(stmt, 1, message_id);
    mx_sqlite3_bind_id(stmt, 2, room_id);

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

bool mx_pined_repo_delete(int id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (id <= 0) {
        return false;
    }

    const char *sql = "DELETE FROM room_pinned_message "
                      "WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }
    mx_sqlite3_bind_id(stmt, 1, id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}
