#include "message.h"

//--------------------------------------------------
// Free memory allocated for message struct
//--------------------------------------------------
void mx_free_message(t_message *message) {
    if (message != NULL) {
        free(message->text);
        if (message->author != NULL) {
            free(message->author->name);
            free(message->author->tag);
            free(message->author->password_hash);
            free(message->author->status);
            free(message->author->description);
            free(message->author);
        }
        // Free room
        if (message->room != NULL) {
            free(message->room->name);
            free(message->room->description);
            free(message->room);
        }
        // Free reply
        free(message->reply);
        // Free message_files list
        t_list *current_file = message->message_files;
        while (current_file != NULL) {
            t_list *next = current_file->next;
            t_file *file = (t_file *)current_file->data;
            if (file != NULL) {
                free(file->name);
                free(file->url);
                free(file);
            }
            free(current_file);
            current_file = next;
        }
        // Free readed_by list
        t_list *current_user = message->readed_by;
        while (current_user != NULL) {
            t_list *next = current_user->next;
            t_user *user = (t_user *)current_user->data;
            if (user != NULL) {
                free(user);
            }
            free(current_user);
            current_user = next;
        }
        // Free the message itself
        free(message);
    }
}

//------------------------------------------------
// get message by id 
// it is the MODIFIED version of 
// t_message *mx_message_repo_gets(int id)
//------------------------------------------------
t_message *mx_message_repo_gets(int id, bool ignore) {
    sqlite3 *db = mx_get_env()->db_connection;
     t_message *message = mx_init_message();
     
    const char *sql = "SELECT m.id, m.room_id, m.author_id, m.reply_id, m.word, m.created_at, m.edited_at, "
                      "u.id, u.name, u.tag , u.password_hash, "
                      "u.photo_id , u.status , u.description , "
                      "u.created_at , u.edited_at , "
                      "r.id , r.name , "
                      "r.description , r.created_at , r.edited_at , "
                      "f.id, f.type , f.created_at , r.photo_id "
                      "FROM message m "
                      "LEFT JOIN user u ON m.author_id = u.id "
                      "LEFT JOIN room r ON m.room_id = r.id "
                      "LEFT JOIN message_file f ON m.id = f.message_id "
                      "WHERE m.id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            
            // Write data in the user struct
            message->id = sqlite3_column_int(stmt, 0);
            message->room_id = sqlite3_column_int(stmt, 1);
            message->author_id = sqlite3_column_int(stmt, 2);
            message->reply_id = sqlite3_column_int(stmt, 3);
            message->text = strdup((const char *)sqlite3_column_text(stmt, 4));
            message->created_at = sqlite3_column_int(stmt, 5);
            message->edited_at = sqlite3_column_int(stmt, 6);

            message->readed_by = malloc(sizeof(t_list));
            message->author = malloc(sizeof(t_user));
            message->room = malloc(sizeof(t_room));
            message->message_files = malloc(sizeof(t_list));

            // Reply
            if (ignore) {
                printf("ignore\n");
                message->reply = mx_message_repo_gets(sqlite3_column_int(stmt, 3), false);
            }
            //printf("ok 3\n");

            // Populate author information
            message->author->id = sqlite3_column_int(stmt, 7);
            message->author->name = strdup((const char *)sqlite3_column_text(stmt, 8));
            message->author->tag = strdup((const char *)sqlite3_column_text(stmt, 9));
            message->author->password_hash = strdup((const char *)sqlite3_column_text(stmt, 10));
            message->author->photo_id = sqlite3_column_int(stmt, 11);
            message->author->status = strdup((const char *)sqlite3_column_text(stmt, 12));
            message->author->description = strdup((const char *)sqlite3_column_text(stmt, 13));
            message->author->created_at = sqlite3_column_int(stmt, 14);
            message->author->edited_at = sqlite3_column_int(stmt, 15);

            // Populate room information
            message->room->id = sqlite3_column_int(stmt, 16);
            message->room->name = strdup((const char *)sqlite3_column_text(stmt, 17));
            message->room->description = strdup((const char *)sqlite3_column_text(stmt, 18));
            message->room->created_at = sqlite3_column_int(stmt, 19);
            message->room->edited_at = sqlite3_column_int(stmt, 20);
            message->room->photo_id = sqlite3_column_int(stmt, 24);
            
            // Populate file information
            message->message_files->data = sqlite3_column_int(stmt, 21);
            message->message_files->next = NULL;
            
            t_list *current = message->readed_by;
            
            int user_id = sqlite3_column_int(stmt, 7);
                //printf("ok 6\n");
            current->data = user_id;
            current->next = malloc(sizeof(t_list));
            current = current->next;
            
        } else {
            mx_free_message(message);
            message = NULL;
        }
        sqlite3_finalize(stmt);

    } else {
        mx_free_message(message);
        message = NULL;
    }
    return message;
}

// ----------------------------------------------------------------
// Get all messages by room id; return t_list of all messages
// ----------------------------------------------------------------
t_list *mx_message_repo_get_many(int room_id) {
    sqlite3 *db = mx_get_env()->db_connection;
    t_list *list = NULL;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT m.id FROM message m WHERE m.room_id = ?";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int message_id = sqlite3_column_int(stmt, 0);
            //printf("%d:\n", message_id);
            mx_push_back(&list, mx_message_repo_gets(message_id, true));
        }
        
        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "Error preparing SQL statement for message: %s\n", sqlite3_errmsg(db));
    }
    return list;
}

t_message *mx_message_repo_get(int id) {

}


// ---------------------------------------
// add new message to db , return message's id
// ---------------------------------------
int mx_message_repo_create(t_message_create_dto *dto) {
    sqlite3 *db = mx_get_env()->db_connection;
    t_user_id message_id = -1; 
    if (db == NULL || dto == NULL) {
        fprintf(stderr, "Invalid arguments\n");
        return message_id;
    }
    time_t current_time = time(NULL);

    const char *sql = "INSERT INTO message (author_id, room_id, reply_id, word, created_at, edited_at) "
                      "VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, dto->author_id);
        sqlite3_bind_int(stmt, 2, dto->room_id);
        sqlite3_bind_int(stmt, 3, dto->reply_id);
        sqlite3_bind_text(stmt, 4, dto->text, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, (int)current_time);
        sqlite3_bind_int(stmt, 6, (int)current_time);
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            message_id = sqlite3_last_insert_rowid(db);
            printf("Message added successfully!\n");
        } else {
            fprintf(stderr, "Error inserting message: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    
    } else {
        fprintf(stderr, "Error preparing SQL statement for message: %s\n", sqlite3_errmsg(db));
    }
    return message_id;
}

// ---------------------------------------------------
// update message information in database
// ---------------------------------------------------
bool mx_message_repo_put(int id, t_message_create_dto *dto) {
    sqlite3 *db = mx_get_env()->db_connection;
    if (db == NULL || dto == NULL) {
        fprintf(stderr, "Invalid arguments\n");
        return false;
    }
    const char *sql = "UPDATE message SET room_id = ?, reply_id = ?, word = ?, edited_at = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, dto->room_id);
        sqlite3_bind_int(stmt, 2, dto->reply_id);
        sqlite3_bind_text(stmt, 3, dto->text, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, (int)time(NULL)); 
        sqlite3_bind_int(stmt, 5, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error updating message: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }
        printf("Message updated successfully!\n");
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return true;
    } else {
        fprintf(stderr, "Error preparing SQL statement for message: %s\n", sqlite3_errmsg(db));
        return false;
    }
}

// -------------------------------------------
// Delete the message file
// -------------------------------------------
bool mx_message_repo_delete(int id) {
    sqlite3 *db = mx_get_env()->db_connection;

    const char *sql = "DELETE FROM message WHERE id = ?;";
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
