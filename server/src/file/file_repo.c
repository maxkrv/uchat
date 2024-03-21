#include "server.h"

//--------------------------------------------------
// Free memory allocated for user struct
//--------------------------------------------------
void mx_free_file(t_file *file) {
    if (file != NULL) {
        free(file->name);
        free(file->url);
        free(file->edited_at);
        free(file->created_at);
        free(file);
    }
}

// -------------------------------------------
// Get file by id
// return t_file structure
// -------------------------------------------
t_file *mx_file_repo_get(int id) {
    sqlite3 *db = mx_get_env()->db_connection;
    t_file *file = mx_init_file();

    const char *sql = "SELECT * FROM file WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("ok 2\n");
            // Write data in the file struct
            file->id = sqlite3_column_int(stmt, 0);
            file->name = strdup((const char *)sqlite3_column_text(stmt, 1));
            file->url = strdup((const char *)sqlite3_column_text(stmt, 2));
            file->created_at = sqlite3_column_int(stmt, 3);
            file->edited_at = sqlite3_column_int(stmt, 4);

        } else {
            mx_free_file(file);
            file = NULL;
        }
        sqlite3_finalize(stmt);

    } else {
        mx_free_file(file);
        file = NULL;
    }
    return file;
}
