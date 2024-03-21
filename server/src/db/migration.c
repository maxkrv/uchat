#include "db.h"

void mx_create_migration_table(sqlite3 *db) {
    char *message_error;
    char *sql = "CREATE TABLE IF NOT EXISTS migration ("
                "id              INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name            TEXT NOT NULL UNIQUE,"
                "was_run         INT NOT NULL DEFAULT 1, "
                "created_at      INT DEFAULT(strftime('%s', 'now')) "
                ");";

    mx_handle_sqlite_error(sqlite3_exec(db, sql, NULL, 0, &message_error),
                           message_error);
}

//-------------------------------------------
// do all commands from the .sql file
//-------------------------------------------
void mx_execute_sql_from_file(sqlite3 *db, const char *filename) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        fprintf(stderr, "Error: Failed to open file '%s'\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *sql = mx_strnew(file_size);

    fread(sql, 1, file_size, file);
    fclose(file);

    if (sqlite3_exec(db, sql, NULL, 0, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error: Failed to execute SQL commands in file '%s'\n",
                filename);
    }
    mx_strdel(&sql);
}

//-------------------------------------------
// check the migration in the database
//-------------------------------------------
static bool is_migration_recorded(sqlite3 *db, const char *filepath) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM migration WHERE name = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare SQL statement: %s\n",
                sqlite3_errmsg(db));
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, filepath, -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        if (sqlite3_column_int(stmt, 0) > 0) {
            sqlite3_finalize(stmt);
            return true;
        }
    }

    sqlite3_finalize(stmt);
    return false;
}

// -----------------------------------------------------------
// Function to record information about applied migrations
// -----------------------------------------------------------
void mx_record_migration(sqlite3 *db, const char *migration_name) {
    char *sql = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *insert_sql = "INSERT INTO migration (name) VALUES (?)";

    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error: Failed to prepare SQL statement for "
                        "recording migration\n");
        return;
    }

    if (sqlite3_bind_text(stmt, 1, migration_name, strlen(migration_name),
                          SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "Error: Failed to bind migration name parameter\n");
        sqlite3_finalize(stmt);
        return;
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error: Failed to execute SQL statement for migration "
                        "recording\n");
    } else {
        printf("Migration recorded successfully: %s\n", migration_name);
    }
    sqlite3_finalize(stmt);
}

//-------------------------------------------
// Function to run migrations
//-------------------------------------------
void mx_run_migrations(sqlite3 *db, t_env_params *env) {
    char *migration_dir = mx_path_join(env->root_dir, "db/migrations");
    DIR *dir = opendir(migration_dir);
    struct dirent *entry;

    mx_create_migration_table(env->db_connection);

    if (!dir) {
        printf("Error: Failed to open migration directory.\n");
        mx_strdel(&migration_dir);
        return;
    }

    while ((entry = readdir(dir))) {
        if (strstr(entry->d_name, ".sql") != NULL) {
            char *filepath = mx_path_join(migration_dir, entry->d_name);

            // Check if migration is already in the database
            if (is_migration_recorded(db, entry->d_name)) {
                printf("Migration from file '%s' is already executed.\n",
                       entry->d_name);
                continue;
            }

            mx_execute_sql_from_file(db, filepath);
            mx_record_migration(db, entry->d_name);

            mx_strdel(&filepath);
        }
    }
    mx_strdel(&migration_dir);
    closedir(dir);
}
