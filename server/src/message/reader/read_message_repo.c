#include "message.h"

//--------------------------------------------------
// Free memory allocated for read message struct
//--------------------------------------------------
void mx_free_read_message(t_read_message *message) {
    if (message != NULL) {
        if (message->user != NULL) {
            free(message->user->name);
            free(message->user->tag);
            free(message->user->password_hash);
            free(message->user->status);
            free(message->user->description);

            if (message->user->photo != NULL) {
                free(message->user->photo->name);
                free(message->user->photo->url);
                free(message->user->photo);
            }
            free(message->user);
        }
        free(message);
    }
}


/* 
----------------------------------------------------------------
return message_readerinformation using message_reader_id 
----------------------------------------------------------------
*/
t_read_message *mx_read_message_repo_get(int id) {
    t_read_message *message = mx_init_read_message();
    sqlite3 *db = mx_get_env()->db_connection;

    const char *sql = "SELECT m.id, m.reader_id, m.message_id, m.created_at, m.edited_at, "
                      "u.id, u.name, u.tag, u.password_hash, u.photo_id, u.status, u.description, "
                      "u.created_at AS user_created_at, u.edited_at AS user_edited_at, "
                      "f.id AS photo_id, f.name AS photo_name, f.url AS photo_url, f.created_at AS photo_created_at "
                      "FROM message_reader m "
                      "LEFT JOIN user u ON m.reader_id = u.id "
                      "LEFT JOIN file f ON u.photo_id = f.id "
                      "WHERE m.id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            message->id = sqlite3_column_int(stmt, 0);
            message->user_id = sqlite3_column_int(stmt, 1);
            message->message_id = sqlite3_column_int(stmt, 2);
            message->created_at = sqlite3_column_int(stmt, 3);
            message->edited_at = sqlite3_column_int(stmt, 4);

            message->user = malloc(sizeof(t_user));
            message->user->id = sqlite3_column_int(stmt, 5);
            message->user->name = strdup((const char *)sqlite3_column_text(stmt, 6));
            message->user->tag = strdup((const char *)sqlite3_column_text(stmt, 7));
            message->user->password_hash = strdup((const char *)sqlite3_column_text(stmt, 8));
            message->user->photo_id = sqlite3_column_int(stmt, 9);
            message->user->status = strdup((const char *)sqlite3_column_text(stmt, 10));
            message->user->description = strdup((const char *)sqlite3_column_text(stmt, 11));
            message->user->created_at = sqlite3_column_int(stmt, 12);
            message->user->edited_at = sqlite3_column_int(stmt, 13);

            message->user->photo = malloc(sizeof(t_file));
            message->user->photo->id = sqlite3_column_int(stmt, 14);
            message->user->photo->name = strdup((const char *)sqlite3_column_text(stmt, 15));
            message->user->photo->url = strdup((const char *)sqlite3_column_text(stmt, 16));
            message->user->photo->created_at = sqlite3_column_int(stmt, 17);
        } else {
            mx_free_read_message(message);
            message = NULL;
        }
        sqlite3_finalize(stmt);

    } else {
        mx_free_read_message(message);
        message = NULL;
    }
    return message;
}

//----------------------------------------------------------------
// Use struct to store id of message reader
typedef struct s_message_reader {
    int id;
    
} t_message_reader;

/* 
----------------------------------------------------------------
Get all message readers using message id
----------------------------------------------------------------
*/
t_list *mx_read_messages_repo_get(int message_id) {
    sqlite3 *db = mx_get_env()->db_connection;
    t_list *head = NULL;
    t_list *tail = NULL;
    sqlite3_stmt *stmt;
    //printf("ok");
    const char *sql = "SELECT mr.id FROM message_reader mr "
                      "JOIN message m ON mr.message_id = m.id "
                      "WHERE m.id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return NULL; 
    }

    sqlite3_bind_int(stmt, 1, message_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        //printf("ok1 \n");
        t_message_reader *message_file = malloc(sizeof(t_message_reader));
        if (message_file == NULL) {
            printf("not ok");
            sqlite3_finalize(stmt);
            return NULL;
        }

        message_file->id = sqlite3_column_int(stmt, 0);
        
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
    // mx_push_back(&list, mx_read_message_repo_get(1));
    // return list;
}

/* 
----------------------------------------------------------------
return message_reader id using reader(user)_id and message_id
----------------------------------------------------------------
*/
int mx_message_repo_read(int user_id, int message_id) {
    sqlite3 *db = mx_get_env()->db_connection;
    int message_read_id = -1;
    const char *sql = "SELECT id FROM message_reader WHERE reader_id = ? AND message_id = ?;";
    
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
        sqlite3_bind_int(stmt, 2, message_id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            message_read_id = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
    }

    return message_read_id;
}
