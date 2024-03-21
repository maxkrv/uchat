#include "room.h"

void mx_free_room(t_room *room) {
    if (room != NULL) {
        free(room->name);
        free(room->type);
        free(room->description);

        // Free room photo
        if (room->photo != NULL) {
            free(room->photo->name);
            free(room->photo->url);
            free(room->photo);
        }

        // Free members list
        mx_free_member_list(room->members);

        // Free pinned messages list
        mx_free_pinned_message_list(room->pined_messages);

        // Free messages list
        mx_free_message_list(room->messages);

        free(room);
    }
}

void mx_free_member_list(t_list *members) {
    t_list *current = members;
    while (current != NULL) {
        t_list *next = current->next;
        t_room_member *member = (t_room_member *)current->data;
        if (member != NULL) {
            mx_free_room_member(member);
        }
        free(current);
        current = next;
    }
}

void mx_free_pinned_message_list(t_list *pinned_messages) {
    t_list *current = pinned_messages;
    while (current != NULL) {
        t_list *next = current->next;
        t_room_pined_message *pinned_message = (t_room_pined_message *)current->data;
        if (pinned_message != NULL) {
            mx_free_pinned_message(pinned_message);
        }
        free(current);
        current = next;
    }
}

void mx_free_message_list(t_list *messages) {
    t_list *current = messages;
    while (current != NULL) {
        t_list *next = current->next;
        t_message *message = (t_message *)current->data;
        if (message != NULL) {
            mx_free_message(message);
        }
        free(current);
        current = next;
    }
}


t_room *mx_room_repo_get(int id) {
    t_room *room = mx_init_room();
    sqlite3 *db = mx_get_env()->db_connection;
    
    const char *sql = "SELECT id, name, category, description, photo_id, created_at, edited_at FROM room WHERE id = ?";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Write data in the room struct
            room->id = sqlite3_column_int(stmt, 0);
            room->name = strdup((const char *)sqlite3_column_text(stmt, 1));
            room->type = strdup((const char *)sqlite3_column_text(stmt, 2));
            room->description = strdup((const char *)sqlite3_column_text(stmt, 3));
            room->photo_id = sqlite3_column_int(stmt, 4);
            room->created_at = sqlite3_column_int(stmt, 5);
            room->edited_at = sqlite3_column_int(stmt, 6);

            room->members = malloc(sizeof(t_list));
            room->pined_messages = malloc(sizeof(t_list));
            room->photo = malloc(sizeof(t_file));
            room->messages = malloc(sizeof(t_list));

            room->photo = mx_file_repo_get(sqlite3_column_int(stmt, 4));
            room->messages = mx_message_repo_get_many(id);
            room->pined_messages = mx_pined_repo_get_many(id); 
            room->members = mx_members_repo_get_many(id);
            } else {
            mx_free_room(room);
            room = NULL;
        }
        sqlite3_finalize(stmt);

    } else {
        mx_free_room(room);
        room = NULL;
    }
    return room;
}

int mx_room_repo_create(t_room_create_dto *dto) {
    sqlite3 *db = mx_get_env()->db_connection;
    int room_id = -1; 
    
    if (db == NULL || dto == NULL) {
        fprintf(stderr, "Invalid arguments\n");
        return room_id;
    }

    time_t current_time = time(NULL);

    const char *sql = "INSERT INTO room (name, description, photo_id, category, created_at, edited_at) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, dto->name, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, dto->photo_id);
        sqlite3_bind_text(stmt, 2, dto->description, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, dto->type, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, (int)current_time);
        sqlite3_bind_int(stmt, 6, (int)current_time);
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            room_id = sqlite3_last_insert_rowid(db);
            printf("Room added successfully!\n");
        } else {
            fprintf(stderr, "Error inserting room: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
    }
    return room_id;
}

bool mx_room_repo_put(int id, t_room_create_dto *dto) {
    sqlite3 *db = mx_get_env()->db_connection;
    if (db == NULL || dto == NULL) {
        fprintf(stderr, "Invalid arguments\n");
        return false;
    }
    const char *sql = "UPDATE room SET name = ?, description = ?, photo_id = ?, category = ?, edited_at = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, dto->name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, dto->description, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, dto->photo_id);
        sqlite3_bind_int(stmt, 4, (int)time(NULL)); 
        sqlite3_bind_int(stmt, 5, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error updating room: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }
        printf("Room updated successfully!\n");
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return true;
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
}

bool mx_room_repo_delete(int id) {
    sqlite3 *db = mx_get_env()->db_connection;

    const char *sql = "DELETE FROM room WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error deleting room: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }

        printf("Room with ID %d deleted successfully!\n", id);
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return true;
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
}
