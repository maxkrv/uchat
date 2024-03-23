#include "server.h"

static t_list *bind_columns_to_users(sqlite3_stmt *stmt) {
    t_list *users = NULL;
    t_user *last = NULL;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!last) {
            last = mx_sqlite_bind_columns_to_user(stmt, 0);
            mx_push_back(&users, last);
        }
        last->photo = mx_sqlite_bind_columns_to_file(stmt, 9);
    }

    return users;
}

t_user *mx_user_repo_get(int id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT u.*, f.* "
                      "FROM user u "
                      "LEFT JOIN file f ON u.photo_id = f.id "
                      "WHERE u.id = ? ;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    mx_sqlite3_bind_id(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return NULL;
    }
    t_user *user = mx_sqlite_bind_columns_to_user(stmt, 0);

    if (user) {
        user->photo = mx_sqlite_bind_columns_to_file(stmt, 9);
    }
    sqlite3_finalize(stmt);

    return user;
}

t_user *mx_user_repo_get_by_tag(char *tag) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    // Create SQL query using Join
    const char *sql = "SELECT u.*, f.* "
                      "FROM user u "
                      "LEFT JOIN file f ON u.photo_id = f.id "
                      "WHERE u.tag = ? ;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }
    sqlite3_bind_text(stmt, 1, tag, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return NULL;
    }
    t_user *user = mx_sqlite_bind_columns_to_user(stmt, 0);

    if (user) {
        user->photo = mx_sqlite_bind_columns_to_file(stmt, 9);
    }
    sqlite3_finalize(stmt);

    return user;
}

t_user *mx_user_repo_get_by_name(char *name) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;
    // Create SQL query using Join
    const char *sql = "SELECT u.*, f.* "
                      "FROM user u "
                      "LEFT JOIN file f ON u.photo_id = f.id "
                      "WHERE u.name = ? ;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return NULL;
    }
    t_user *user = mx_sqlite_bind_columns_to_user(stmt, 0);

    if (user) {
        user->photo = mx_sqlite_bind_columns_to_file(stmt, 9);
    }
    sqlite3_finalize(stmt);

    return user;
}

t_user_id mx_user_repo_create(t_user_create_dto *dto) {
    sqlite3 *db = mx_env_get()->db_connection;
    t_user_id user_id = -1;
    sqlite3_stmt *stmt;

    if (!dto) {
        return -1;
    }
    const char *sql =
        "INSERT INTO user "
        "(name, tag, password_hash, status, photo_id, description) "
        "VALUES (?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    sqlite3_bind_text(stmt, 1, dto->name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, dto->tag, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, dto->password, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, dto->status, -1, SQLITE_TRANSIENT);
    mx_sqlite3_bind_id(stmt, 5, dto->photo_id);
    sqlite3_bind_text(stmt, 6, dto->description, -1, SQLITE_TRANSIENT);

    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        user_id = sqlite3_last_insert_rowid(db);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    } else {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    }
    sqlite3_finalize(stmt);

    return user_id;
}

bool mx_user_repo_put(int id, t_user_update_dto *dto) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (!dto || id <= 0) {
        return false;
    }

    const char *sql = "UPDATE user "
                      "SET name = ?, tag = ?, photo_id = ?, "
                      "status = ?, description = ? "
                      "WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    sqlite3_bind_text(stmt, 1, dto->name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, dto->tag, -1, SQLITE_TRANSIENT);
    mx_sqlite3_bind_id(stmt, 3, dto->photo_id);
    sqlite3_bind_text(stmt, 4, dto->status, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, dto->description, -1, SQLITE_TRANSIENT);
    mx_sqlite3_bind_id(stmt, 6, id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}

bool mx_user_repo_change_password(int id, char *password) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (!password || id <= 0) {
        return false;
    }

    const char *sql = "UPDATE user "
                      "SET password_hash = ? "
                      "WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }
    sqlite3_bind_text(stmt, 1, password, -1, SQLITE_TRANSIENT);
    mx_sqlite3_bind_id(stmt, 2, id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}

bool mx_user_repo_delete(int id) {
    sqlite3 *db = mx_env_get()->db_connection;
    sqlite3_stmt *stmt;

    if (id <= 0) {
        return false;
    }

    const char *sql = "DELETE FROM user "
                      "WHERE id = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }
    mx_sqlite3_bind_id(stmt, 1, id);

    bool res = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return res;
}
