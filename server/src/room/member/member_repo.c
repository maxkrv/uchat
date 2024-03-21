#include "server.h"

// ----------------------------------------------------------------
// free memory for member struct
// ----------------------------------------------------------------
void mx_free_member(t_room_member *member) {
    if (member != NULL) {
        if (member->user != NULL) {
            free(member->user->name);
            free(member->user->tag);
            free(member->user->password_hash);
            free(member->user->status);
            free(member->user->description);
            free(member->user);
        }

        if (member->room != NULL) {
            free(member->room->name);
            free(member->room->type);
            free(member->room->description);
            free(member->room);
        }

        free(member);
    }
}

//--------------------------
// get member by id 
//--------------------------
t_room_member *mx_members_repo_get(int id) {
    sqlite3 *db = mx_get_env()->db_connection;
    t_room_member *member = mx_init_room_member();
    // Create SQL query using Join 
    const char *sql = "SELECT r.id, r.room_id, r.user_id, r.is_admin, r.created_at, r.edited_at"
                      "FROM room_member r WHERE u.id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("ok 2\n");
            // Write data in the member struct
            member->id = sqlite3_column_int(stmt, 0);
            member->room_id = sqlite3_column_int(stmt, 1);
            member->user_id = sqlite3_column_int(stmt, 2);
            member->is_admin = sqlite3_column_int(stmt, 3);
            member->created_at = sqlite3_column_int(stmt, 4);
            member->edited_at = sqlite3_column_int(stmt, 5);

            member->user = malloc(sizeof(t_user));
            member->room = malloc(sizeof(t_room));

            member->room = mx_room_repo_get(sqlite3_column_int(stmt, 1));
            member->user = mx_user_repo_get(sqlite3_column_int(stmt, 2));
        } else {
            mx_free_member(member);
            member = NULL;
        }
        sqlite3_finalize(stmt);

    } else {
        mx_free_member(member);
        member = NULL;
    }
    return member;
}

t_room_member *mx_members_repo_get_by(int user_id, int room_id) {
    return mx_init_room_member();
}

// -------------------------------------------
// add new member to db , return member's id
// -------------------------------------------
int mx_members_repo_create(int user_id, int room_id, bool is_admin) {
    sqlite3 *db = mx_get_env()->db_connection;
    int member_id = -1; 
    
    time_t current_time = time(NULL);

    const char *sql = "INSERT INTO room_member (user_id, room_id, is_admin, created_at, edited_at) "
                      "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user_id);
        sqlite3_bind_int(stmt, 2, room_id);
        sqlite3_bind_int(stmt, 3, is_admin);
        sqlite3_bind_int(stmt, 4, (int)current_time);
        sqlite3_bind_int(stmt, 5, (int)current_time);
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            member_id = sqlite3_last_insert_rowid(db);
            printf("Member added successfully!\n");
        } else {
            fprintf(stderr, "Error inserting member: %s\n", sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
    }
    return member_id;
}

// -------------------------------------------
// Edit the member 
// -------------------------------------------
bool mx_members_repo_update(int id, bool is_admin) {
    sqlite3 *db = mx_get_env()->db_connection;
    
    const char *sql = "UPDATE room_member SET is_admin = ?, edited_at = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, is_admin);
        sqlite3_bind_int(stmt, 2, (int)time(NULL)); 
        sqlite3_bind_int(stmt, 3, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error updating member: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }
        printf("Member updated successfully!\n");
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return true;
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
}

t_list *mx_members_repo_get_many_rooms(int user_id) {
    t_list *r;

    mx_push_back(&r, mx_room_repo_get(1));

    return r;
}

t_list *mx_members_repo_get_many(int room_id) {
    t_list *r;

    mx_push_back(&r, mx_members_repo_get(1));

    return r;
}

// -------------------------------------------
// Delete the member 
// -------------------------------------------
bool mx_members_repo_delete(int id) {
    sqlite3 *db = mx_get_env()->db_connection;

    const char *sql = "DELETE FROM room_member WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error deleting member: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }

        printf("Member with ID %d deleted successfully!\n", id);
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        return true;
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
}
