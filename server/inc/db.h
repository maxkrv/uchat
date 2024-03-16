#ifndef BACKEND_DB_H
#define BACKEND_DB_H

#include "base.h"
#include "utils.h"
#include "shared.h"

#include <dirent.h>

sqlite3 *mx_connect_to_database(const char *db_path);
void mx_handle_sqlite_error(int rc, char *message_error);

void mx_run_migrations(sqlite3 *db, t_env_params *env);
void mx_record_migration(sqlite3 *db, const char *migration_name);
void mx_create_migration_table(sqlite3 *db);
#endif
