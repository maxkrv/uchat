#include "message.h"

//--------------------------------------------------
// Free memory allocated for message struct
//--------------------------------------------------
void mx_free_message_file(t_message_file *file) {
    if (file != NULL) {
        free(file->file_id);
        free(file->message_id);
        free(file->type);
        free(file->created_at);
        free(file->edited_at);
        free(file);
    }
}

//-----------------------------------------------------------
// get message_file by id and get all information for struct    
//-----------------------------------------------------------
t_message_file *mx_message_file_repo_get(int id) {
    sqlite3 *db = mx_get_env()->db_connection;
    t_message_file *file = mx_init_message_file();

    const char *sql = "SELECT mf.id, mf.file_id, mf.message_id, mf.type, mf.created_at, mf.edited_at, "
                      "m.id, m.room_id, m.author_id, m.reply_id, m.word, m.created_at, m.edited_at, "
                      "f.id, f.name, f.url, f.created_at, f.edited_at "
                      "FROM message_file mf "
                      "JOIN message m ON mf.message_id = m.id "
                      "JOIN file f ON mf.file_id = f.id "
                      "WHERE mf.id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            file->id = sqlite3_column_int(stmt, 0);
            file->file_id = sqlite3_column_int(stmt, 1);
            file->message_id = sqlite3_column_int(stmt, 2);
            file->type = strdup((const char *)sqlite3_column_text(stmt, 3));
            file->created_at = sqlite3_column_int(stmt, 4);
            file->edited_at = sqlite3_column_int(stmt, 5);

            // Fill message struct
            file->message = malloc(sizeof(t_message));
            file->message->id = sqlite3_column_int(stmt, 6);
            file->message->room_id = sqlite3_column_int(stmt, 7);
            file->message->author_id = sqlite3_column_int(stmt, 8);
            file->message->reply_id = sqlite3_column_int(stmt, 9);
            file->message->text = strdup((const char *)sqlite3_column_text(stmt, 10));
            file->message->created_at = sqlite3_column_int(stmt, 11);
            file->message->edited_at = sqlite3_column_int(stmt, 12);

            // Fill file struct
            file->file = malloc(sizeof(t_file));
            file->file->id = sqlite3_column_int(stmt, 13);
            file->file->name = strdup((const char *)sqlite3_column_text(stmt, 14));
            file->file->url = strdup((const char *)sqlite3_column_text(stmt, 15));
            file->file->created_at = sqlite3_column_int(stmt, 16);
            file->file->edited_at = sqlite3_column_int(stmt, 17);
        } else {
            mx_free_message_file(file); 
            file = NULL;
        }
        sqlite3_finalize(stmt);
    } else {
        mx_free_message_file(file); 
        file = NULL;
    }

    return file;
}

//----------------------------------------------------------------
// Uisng this struct for storing message id
typedef struct s_message_files {
    int id;
    
} t_message_files;

/* 
----------------------------------------------------------------
get all file messages of specified user 
----------------------------------------------------------------
*/
t_list *mx_message_file_repo_get_all(int user_id) {
    sqlite3 *db = mx_get_env()->db_connection;
    t_list *head = NULL;
    t_list *tail = NULL;
    sqlite3_stmt *stmt;
    //printf("ok");
    const char *sql = "SELECT mf.id FROM message_file mf "
                      "JOIN message m ON mf.message_id = m.id "
                      "WHERE m.author_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return NULL; 
    }

    sqlite3_bind_int(stmt, 1, user_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ok1 \n");
        t_message_file *message_file = malloc(sizeof(t_message_file));
        if (message_file == NULL) {
            printf("not ok");
            sqlite3_finalize(stmt);
            return NULL;
        }

        /* Populate message_file with data from the database */
        message_file->id = sqlite3_column_int(stmt, 0);
        // message_file->message_content = strdup(sqlite3_column_text(stmt, 1));
        // message_file->file_path = strdup(sqlite3_column_text(stmt, 2));
        
        t_list *node = malloc(sizeof(t_list));
        if (node == NULL) {
            free(message_file);
            sqlite3_finalize(stmt);
            return NULL;
        }
        node->data = message_file;
        node->next = NULL;

        if (head == NULL) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    sqlite3_finalize(stmt);
    return head;
}

// -------------------------------------------------------
// add new message file to db , return message file's id
// -------------------------------------------------------
int mx_message_file_repo_create(int file_id, char *type, int message_id) {
    sqlite3 *db = mx_get_env()->db_connection;
    int file_massege_id = -1; 
    
    if (db == NULL) {
        fprintf(stderr, "Invalid db\n");
        return file_massege_id;
    }

    time_t current_time = time(NULL);

    const char *sql = "INSERT INTO message_file (file_id, type, message_id, created_at, edited_at) "
                      "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, file_id);
        sqlite3_bind_text(stmt, 2, type, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, message_id);
        sqlite3_bind_int64(stmt, 4, (sqlite3_int64)current_time);
        sqlite3_bind_int64(stmt, 5, (sqlite3_int64)current_time);
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            file_massege_id = sqlite3_last_insert_rowid(db);
            printf("file messege added successfully!\n");
        } else {
            fprintf(stderr, "Error inserting message: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
    }
    return file_massege_id;
}

// ---------------------------------------------------
// update message file information in database
// ---------------------------------------------------
bool mx_message_file_repo_update(int id, int file_id, char *type) {
    sqlite3 *db = mx_get_env()->db_connection;
    if (db == NULL) {
        fprintf(stderr, "Error with db in mx_message_file_repo_update \n");
        return false;
    }
    const char *sql = "UPDATE message_file SET file_id = ?, type = ?, edited_at = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, file_id);
        sqlite3_bind_text(stmt, 2, type, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, (int)time(NULL)); 
        sqlite3_bind_int(stmt, 4, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error updating message_file: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }
        printf("Message file updated successfully!\n");
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return true;
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
}


// -------------------------------------------
// Delete the message file
// -------------------------------------------
bool mx_message_file_repo_delete(int id) {
    sqlite3 *db = mx_get_env()->db_connection;

    const char *sql = "DELETE FROM message_file WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error deleting Message(file): %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }

        printf("Message(file) with ID %d deleted successfully!\n", id);
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return true;
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
}
