#include "../inc/db.h"

// ---------------------------------------------------------
// this function implements the connection to the database
// ---------------------------------------------------------
sqlite3 *connect_to_database(const char *dbname) {
    sqlite3 *db;
    int rc;
    // Open database connection
    rc = sqlite3_open(dbname, &db);

    if (rc != SQLITE_OK) {
        MG_ERROR(("Can`t open database: %s\n", sqlite3_errmsg(db)));
        sqlite3_close(db);
        return NULL;
    }
    MG_INFO(("Database connection established"));

    return db;
}

// -------------------------
// handle error
// -------------------------
void mx_handle_sqlite_error(int rc, char *message_error, char *name) {
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", message_error);
        sqlite3_free(message_error);
        printf("SQL error: %s\n", message_error);
    } else {
        printf(name, " table created successfully\n");
    }
}
