#include "utils.h"

char *mx_sqlite3_get_string(sqlite3_stmt *stmt, int col) {
    const unsigned char *str = sqlite3_column_text(stmt, col);

    return str ? mx_strdup((const char *)str) : NULL;
}
void mx_sqlite3_bind_id(sqlite3_stmt *stmt, int col, int id) {
    if (id > 0) {
        sqlite3_bind_int(stmt, col, id);
    } else {
        sqlite3_bind_null(stmt, col);
    }
}
