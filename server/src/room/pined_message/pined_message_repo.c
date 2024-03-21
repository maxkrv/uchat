#include "server.h"

// DELETE
// struct s_room_pined_message {
//     int id;
//     int message_id;
//     int room_id;
//     int created_at;
//     int edited_at;

//     t_room *room;
//     t_message *message;
// };

void mx_free_pinned_message(t_room_pined_message *pinned_message) {
    if (pinned_message != NULL) {
        if (pinned_message->room != NULL) {
            free(pinned_message->room->name);
            free(pinned_message->room->type);
            free(pinned_message->room->description);
            free(pinned_message->room);
        }

        if (pinned_message->message != NULL) {
            mx_free_message(pinned_message->message);
        }

        free(pinned_message);
    }
}


t_room_pined_message *mx_pined_repo_get(int id) {
    t_room_pined_message *m = mx_init_room_pined();
    sqlite3 *db = mx_get_env()->db_connection;
    
    const char *sql = "SELECT id, room_id, message_id ,created_at, edited_at FROM room_pinned_message WHERE id = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Write data in the room struct
            m->id = sqlite3_column_int(stmt, 0);
            m->room_id = sqlite3_column_int(stmt, 1);
            m->message_id = sqlite3_column_int(stmt, 2);
            m->created_at = sqlite3_column_int(stmt, 3);
            m->edited_at = sqlite3_column_int(stmt, 4);

            m->room = malloc(sizeof(t_room));
            m->message = malloc(sizeof(t_message));

            m->room = mx_room_repo_get(sqlite3_column_int(stmt, 1));
            m->message = mx_message_repo_get_many(sqlite3_column_int(stmt, 1));
        } else {
            mx_free_pinned_message(m);
            m = NULL;
        }
        sqlite3_finalize(stmt);

    } else {
        mx_free_pinned_message(m);
        m = NULL;
    }
    return m;
}
/* Returns [{discriptions + room + room.photo}] */
t_list *mx_pined_repo_get_many(int room_id) {
    //mx_push_back(&list, mx_pined_repo_get(1));
    sqlite3 *db = mx_get_env()->db_connection;
    t_list *list = NULL;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT m.id FROM room_pinned_message m WHERE m.room_id = ?";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, room_id);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int message_id = sqlite3_column_int(stmt, 0);
            //printf("%d:\n", message_id);
            mx_push_back(&list, mx_pined_repo_get(message_id));
        }
        
        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "Error preparing SQL statement for message: %s\n", sqlite3_errmsg(db));
    }
    return list;
}

int mx_pined_repo_create(int message_id, int room_id) {
    return 1;
}

bool mx_pined_repo_delete(int id) {
    sqlite3 *db = mx_get_env()->db_connection;

    const char *sql = "DELETE FROM room_pinned_message WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error deleting room_pinned_message: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }

        printf("Room_pinned_message with ID %d deleted successfully!\n", id);
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return true;
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
}
