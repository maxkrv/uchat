#include "../inc/user.h"
#include "auth.h"

//--------------------------------------------------
// Free memory allocated for user struct
//--------------------------------------------------
void mx_free_user(t_user *user) {
    if (user != NULL) {
        free(user->name);
        free(user->tag);
        free(user->password_hash);
        free(user->status);
        free(user->description);
        free(user);
    }
}

//------------------------------------------------
// get user by id and get photo information
//------------------------------------------------

// struct s_user {
//     int id;
//     char *name;
//     char *tag;
//     char *password_hash;
//     int photo_id;
//     char *status;
//     char *description;
//     int created_at;
//     int edited_at;

//     t_list *messages;
//     t_list *rooms;
//     t_list *favorites;
//     t_file *photo;
// };

t_user *mx_user_repo_get(int id){
    sqlite3 *db = mx_get_env()->db_connection;
    t_user *user = mx_init_user();
    // Create SQL query using Join 
    const char *sql = "SELECT u.id, u.name, u.tag, u.password_hash, u.photo_id, u.status, u.description, "
                      "u.created_at, u.edited_at, f.id AS photo_id, f.name, f.url, f.created_at AS photo_created_at "
                      "FROM user u LEFT JOIN file f ON u.photo_id = f.id WHERE u.id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("ok 2\n");
            // Write data in the user struct
            user->id = sqlite3_column_int(stmt, 0);
            user->name = strdup((const char *)sqlite3_column_text(stmt, 1));
            user->tag = strdup((const char *)sqlite3_column_text(stmt, 2));
            user->password_hash = strdup((const char *)sqlite3_column_text(stmt, 3));
            user->photo_id = sqlite3_column_int(stmt, 4);
            user->status = strdup((const char *)sqlite3_column_text(stmt, 5));
            user->description = strdup((const char *)sqlite3_column_text(stmt, 6));
            user->created_at = sqlite3_column_int(stmt, 7);
            user->edited_at = sqlite3_column_int(stmt, 8);

            user->photo = malloc(sizeof(t_file));
            user->rooms = malloc(sizeof(t_list));
            user->messages = malloc(sizeof(t_list));
            user->favorites = malloc(sizeof(t_list));

            user->photo = mx_file_repo_get(sqlite3_column_int(stmt, 4));
            user->rooms = mx_members_repo_get_many_rooms(sqlite3_column_int(stmt, 0));
            //user->messages = mx_message_repo_get_many(int room_id);
            while(user->rooms != NULL) {
                t_room *room = (t_room *)user->rooms->data;
                user->messages = mx_message_repo_get_many(room->id);
            }
            user->favorites = mx_favorites_repo_get_many(sqlite3_column_int(stmt, 0));

            // t_file *photo = malloc(sizeof(t_file));
            // photo->id = sqlite3_column_int(stmt, 9);
            // photo->name = strdup((const char *)sqlite3_column_text(stmt, 10));
            // photo->url = sqlite3_column_int(stmt, 11);
            // photo->created_at = sqlite3_column_int(stmt, 12);

            // user->photo = photo;
        } else {
            mx_free_user(user);
            user = NULL;
        }
        sqlite3_finalize(stmt);

    } else {
        mx_free_user(user);
        user = NULL;
    }
    return user;
}

//------------------------------------------------
// get only user by id 
//------------------------------------------------
t_user *mx_only_user_repo_get(int id) {
    sqlite3 *db = mx_get_env()->db_connection;
    t_user *user = mx_init_user();
    const char *sql = "SELECT * FROM user WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            //printf("ok 2");
            // Fill the user struct
            user->id = sqlite3_column_int(stmt, 0);
            user->name = strdup((const char *)sqlite3_column_text(stmt, 1));
            user->tag = strdup((const char *)sqlite3_column_text(stmt, 2));
            user->photo_id = sqlite3_column_int(stmt, 4);
            user->status = strdup((const char *)sqlite3_column_text(stmt, 5));
            user->description = strdup((const char *)sqlite3_column_text(stmt, 6));
            user->created_at = sqlite3_column_int(stmt, 7);
            user->edited_at = sqlite3_column_int(stmt, 8);
        } else {
            //printf("not ok 2 \n");
            mx_free_user(user); 
            user = NULL;
        }
        sqlite3_finalize(stmt);

    } else {
        mx_free_user(user); 
    }
    return user;
}

//------------------------------------------------
// get user by his/her tag
//------------------------------------------------
t_user *mx_user_repo_get_by_tag(char *tag) {
    sqlite3 *db = mx_get_env()->db_connection;

    int user_id = -1;

    const char *sql = "SELECT id FROM user WHERE tag = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, tag, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            user_id = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
    }

    return mx_user_repo_get(user_id);
}


//------------------------------------------------
// get user by his/her name
//------------------------------------------------
t_user *mx_user_repo_get_by_name(char *name) {
    sqlite3 *db = mx_get_env()->db_connection;
    int user_id = -1;

    const char *sql = "SELECT id FROM user WHERE name = ?;";
    
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            user_id = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
    }

    return mx_user_repo_get(user_id);
}

// ---------------------------------------
// add new user to db , return user's id
// ---------------------------------------
t_user_id mx_user_repo_create(t_user_create_dto *dto) {
    sqlite3 *db = mx_get_env()->db_connection;
    t_user_id user_id = -1; 
    
    if (db == NULL || dto == NULL) {
        fprintf(stderr, "Invalid arguments\n");
        return user_id;
    }

    time_t current_time = time(NULL);

    const char *sql = "INSERT INTO user (name, tag, password_hash, status, description, created_at, edited_at) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, dto->name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, dto->tag, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, dto->password, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, dto->status, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, dto->photo_id, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, dto->description, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 7, (int)current_time);
        sqlite3_bind_int(stmt, 8, (int)current_time);
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            user_id = sqlite3_last_insert_rowid(db);
            printf("User added successfully!\n");
        } else {
            fprintf(stderr, "Error inserting user: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
    }
    return user_id;
}

// ---------------------------------------------------
// update user information in database
// ---------------------------------------------------
bool mx_user_repo_put(int id, t_user_update_dto *dto) {
    sqlite3 *db = mx_get_env()->db_connection;
    if (db == NULL || dto == NULL) {
        fprintf(stderr, "Invalid arguments\n");
        return false;
    }
    const char *sql = "UPDATE user SET name = ?, tag = ?, photo_id = ?, status = ?, description = ?, edited_at = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, dto->name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, dto->tag, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, dto->photo_id);
        sqlite3_bind_text(stmt, 4, dto->status, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, dto->description, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, (int)time(NULL)); 
        sqlite3_bind_int(stmt, 7, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error updating user: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }
        printf("User updated successfully!\n");
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return true;
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
}

// -------------------------------------------
// Change the user's password 
// -------------------------------------------
bool mx_user_repo_change_password(int id, char *password) {
    sqlite3 *db = mx_get_env()->db_connection;
    
    const char *sql = "UPDATE user SET password_hash = ?, edited_at = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, password, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, (int)time(NULL)); 
        sqlite3_bind_int(stmt, 3, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error updating user's password: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }
        printf("User updated successfully!\n");
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return true;
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
}

// -------------------------------------------
// Delete the user 
// -------------------------------------------
bool mx_user_repo_delete(int id) {
    sqlite3 *db = mx_get_env()->db_connection;

    const char *sql = "DELETE FROM user WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error deleting user: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }

        printf("User with ID %d deleted successfully!\n", id);
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return true;
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
}
