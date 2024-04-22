#include "db.h"

// ---------------------------------------------------------
// this function implements the connection to the database
// ---------------------------------------------------------
sqlite3 *mx_connect_to_database(const char *dbname) {
    sqlite3 *db;

    if (sqlite3_open(dbname, &db) != SQLITE_OK) {
        MG_ERROR(("Can`t open database: %s", sqlite3_errmsg(db)));
        MG_ERROR(("DB path: %s", dbname));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
        return NULL;
    }
    printf("Connection to db established\n");
    // MG_INFO(("DB path: %s", dbname));

    return db;
}

// -------------------------
// handle error
// -------------------------
void mx_handle_sqlite_error(int rc, char *message_error) {
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", message_error);
        sqlite3_free(message_error);
    }
}
