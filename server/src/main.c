#include "../inc/server.h"
#include "../inc/db.h"

#include "../inc/user.h"
#include "auth.h"
t_user *mx_user_repo_gets(int id, sqlite3 *db) {
    t_user *user = mx_init_user();

    // Construct SQL query to retrieve user data based on id
    const char *sql = "SELECT * FROM user WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the id parameter to the query
        sqlite3_bind_int(stmt, 1, id);
        
        printf("ok 1");
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("ok 2");
            // Retrieve data from the query result and populate the user struct
            user->id = sqlite3_column_int(stmt, 0);
            user->name = strdup((const char *)sqlite3_column_text(stmt, 1));
            user->tag = strdup((const char *)sqlite3_column_text(stmt, 2));
            user->photo_id = sqlite3_column_int(stmt, 4);
            user->status = strdup((const char *)sqlite3_column_text(stmt, 5));
            user->description = strdup((const char *)sqlite3_column_text(stmt, 6));
            user->created_at = sqlite3_column_int(stmt, 7);
            user->edited_at = sqlite3_column_int(stmt, 8);

            // Additional data retrieval and population if needed
        } else {
            printf("not ok 2 \n");
            // User not found with the given id
            // Handle the error or return NULL
            mx_free_user(user); // Free the allocated memory for user
            user = NULL;
        }

        // Finalize the statement
        sqlite3_finalize(stmt);
    } else {
        // Error preparing the SQL statement
        // Handle the error or return NULL
        mx_free_user(user); // Free the allocated memory for user
        user = NULL;
    }
    return user;
}

int get_user_id_by_tag(const char *tag, sqlite3 *db) {
    int user_id = -1; // Default value if user not found

    // Construct SQL query to retrieve user ID based on tag
    const char *sql = "SELECT id FROM user WHERE tag = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the tag parameter to the query
        sqlite3_bind_text(stmt, 1, tag, -1, SQLITE_STATIC);

        // Execute the query
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Retrieve the user ID from the query result
            user_id = sqlite3_column_int(stmt, 0);
        }

        // Finalize the statement
        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
    }

    return user_id;
}

// static void usage(void) {
//     fprintf(stderr,
//             "Usage:\n"
//             "  -port PORT     - listening port, default: %d\n"
//             "  -root ROOT_DIR - root directory, default: '%s'\n"
//             "  -log LEVEL    - debug level, from 0 to 4, default: %d\n"
//             "  -secret SECRET   - jwt auth secret, default: random uiid\n"
//             "  -db DB_PATH   - path to db, default: %s\n",
//             3000, MX_DEFAULT_ROOT_DIR, MG_LL_INFO, MX_DEFAULT_DB_PATH);

//     exit(EXIT_FAILURE);
// }

// static t_env_params *init_env(int argc, char *argv[]) {
//     t_env_params *mx_env = mx_get_env();

//     mx_env->jwt_auth_secret = mx_gen_uuid();
//     mx_env->root_dir = MX_DEFAULT_ROOT_DIR;

//     for (int i = 1; i < argc; i++) {
//         if (strcmp(argv[i], "-log") == 0) {
//             mx_env->log_level = atoi(argv[++i]);
//         } else if (strcmp(argv[i], "-port") == 0) {
//             mx_env->port = atoi(argv[++i]);
//         } else if (strcmp(argv[i], "-root") == 0) {
//             mx_env->root_dir = argv[++i];
//         } else if (strcmp(argv[i], "-secret") == 0) {
//             mx_env->jwt_auth_secret = argv[++i];
//         } else if (strcmp(argv[i], "-db") == 0) {
//             mx_env->db_path = argv[++i];
//         } else {
//             usage();
//         }
//     }
//     if (!mx_env->db_path) {
//         char *tmp = mx_strjoin(mx_env->root_dir, "/");
//         mx_env->db_path = mx_strjoin(tmp, MX_DEFAULT_DB_PATH);

//         mx_strdel(&tmp);
//     }
//     mx_env->db_connection = mx_connect_to_database(mx_env->db_path);

//     return mx_env;
// }
void add_user(sqlite3 *db, const char *name, const char *password_hash,
              const char *status, const char *description, const char *tag) {
    
    time_t current_time = time(NULL);

    //char tag[33];
    //snprintf(tag, sizeof(tag), "%08x%08x%08x%08x", rand(), rand(), rand(), rand());
    const char *sql = "INSERT INTO user (name, tag, password_hash, status, description, created_at, edited_at) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind parameters to the statement
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, tag, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, password_hash, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, status, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, description, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, (int)current_time);
        sqlite3_bind_int(stmt, 7, (int)current_time);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Error inserting user: %s\n", sqlite3_errmsg(db));
        } else {
            printf("User added successfully!: %d\n", sqlite3_last_insert_rowid(db));
        }

        sqlite3_finalize(stmt);
        
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    } else {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
    }
}

t_file *mx_file_repo_gets(int id, sqlite3 *db) {
    //sqlite3 *db = mx_get_env()->db_connection;
    t_file *file = mx_init_file();

    const char *sql = "SELECT * FROM file WHERE id = ?;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            printf("ok 2\n");
            // Write data in the file struct
            file->id = sqlite3_column_int(stmt, 0);
            file->name = strdup((const char *)sqlite3_column_text(stmt, 1));
            file->url = strdup((const char *)sqlite3_column_text(stmt, 2));
            file->created_at = sqlite3_column_int(stmt, 3);
            file->edited_at = sqlite3_column_int(stmt, 4);

        } else {
            mx_free_file(file);
            file = NULL;
        }
        sqlite3_finalize(stmt);

    } else {
        mx_free_file(file);
        file = NULL;
    }
    return file;
}

int mx_free_list(t_list *list) {
    if (list == NULL)
        return 0;
    t_list *tempor = list;
    
    while (list->next != NULL) {
        tempor = list;
        list = list->next;
        free(tempor);
    }
    free(list);
    return 1;
}

typedef struct s_message_readers {
    int id;
    
} t_message_readers;


/*t_list *mx_message_file_repo_get_alls(int user_id, sqlite3 *db) {
    t_list *head = NULL;
    t_list *tail = NULL;
    sqlite3_stmt *stmt;
    printf("ok");
    const char *sql = "SELECT mf.id FROM message_file mf "
                      "JOIN message m ON mf.message_id = m.id "
                      "WHERE m.author_id = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return NULL; 
    }

    /* Bind user_id parameter 
    sqlite3_bind_int(stmt, 1, user_id);

   
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ok1 \n");
        t_message_files *message_file = malloc(sizeof(t_message_files));
        if (message_file == NULL) {
            printf("not ok");
            sqlite3_finalize(stmt);
            return NULL;
        }

        
        // Example:
        message_file->id = sqlite3_column_int(stmt, 0);
        // message_file->message_content = strdup(sqlite3_column_text(stmt, 1));
        // message_file->file_path = strdup(sqlite3_column_text(stmt, 2));
        // ...

        
        t_list *node = malloc(sizeof(t_list));
        if (node == NULL) {
            // Error handling: unable to allocate memory
            free(message_file); // Free allocated memory before returning
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
}*/
t_message *mx_message_repo_getss(int id, bool ignore, sqlite3 *db ) {
    
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

int mx_members_repo_creates(int user_id, int room_id, bool is_admin, sqlite3 *db) {
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
            user_id = sqlite3_last_insert_rowid(db);
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

int main() {
    // srand(time(NULL));

    // t_env_params *mx_env = init_env(argc, argv);
    // char *addr = mx_strjoin("http://localhost:", mx_itoa(mx_env->port));

    // mx_run_migrations(mx_env->db_connection);
    // mx_run_server(mx_env, addr);

    // free(addr);

    sqlite3* db;
    const char* dbname = "/Users/olgatsyban/Documents/Campus/uchat/server/db/sqlite.db";
    // Connection to database
    db = connect_to_database(dbname);
    if (db == NULL) {
        fprintf(stderr, "Failed to connect to the database\n");
        return 1;
    }
    //printf("%d\n", mx_message_file_repo_creates(2, 2, "Filessss", db));

    // try to run migration
    //mx_create_migration_table(db);
    //run_migrations(db);
    //run_tests(db);
    // t_message *u =mx_message_repo_gets(1, db, true);
    // if (u != NULL) {
    //     printf("\n");
    //     printf("%s\n", u->author->name);
    //     // Other attributes can also be accessed using u->attribute_name
    // } else {
    //     printf("User not found.\n");
    // }
//     t_list *message_files = mx_read_messages_repo_gets(1, db);
// if (message_files != NULL) {
//     printf("\n");
//     t_list *current_node = message_files;
//     while (current_node != NULL) {
//         t_message_readers *message = (t_read_message*) current_node->data;
//         printf("Message ID: %d\n", message->id);
//         printf("\n");
//         current_node = current_node->next;
//     }
//     // Don't forget to free the memory allocated for the linked list
//     mx_free_list(message_files);
// } else {
//     printf("Message files not found.\n");
// }
//     t_list *message_list = mx_message_repo_get_manys(1, db);
    
//     // Count the number of messages in the returned list
//     int actual_message_count = 0;
//     t_list *current = message_list;
//     while (current != NULL) {
//     actual_message_count++;
//     printf("names - \n");
//     if (current->data != NULL) { // Check for NULL once is sufficient
//         //printf("ok 2");
//         printf("%d\n", ((t_message *)(current->data))->id);
//     }
//     current = current->next;
// }
    
    
    // for (t_list *node = message_files; node != NULL; node = node->next) {
    //     t_message_file *message_file = (t_message_file *)node->data;
    //     printf("- Message File ID: %d\n", message_file->id);
    // }
    printf("%d\n", mx_members_repo_creates(1, 1, 0, db));
    sqlite3_close(db);
    return 0;
}
