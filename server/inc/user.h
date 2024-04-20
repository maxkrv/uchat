#ifndef UCHAT_USER
#define UCHAT_USER

#include "base.h"
#include "utils.h"
#include "shared.h"
#include "jwt.h"

typedef struct s_user_create_dto {
    char *name;
    char *tag;
    char *password;
    int photo_id;
    char *status;
    char *description;
} t_user_create_dto;

typedef struct s_user_update_dto {
    char *name;
    char *tag;
    int photo_id;
    char *status;
    char *description;
} t_user_update_dto;

// User router
void mx_user_route(t_connection *conn, t_http_message *req);

/*
///////////////////////////////////////////////
 User module. User { discriptions, photo }
/////////////////////////////////////////////
*/

// structure constructors and destructors
t_user *mx_user_init(void);
void mx_user_free(t_user *u);
t_user_create_dto *mx_user_create_dto_get(struct mg_str body);
void mx_user_create_dto_free(t_user_create_dto *dto);
t_user_update_dto *mx_user_update_dto_get(struct mg_str body);
void mx_user_update_dto_free(t_user_update_dto *dto);

// controllers
void mx_user_ctrl_get(t_connection *c, t_http_message *m);
void mx_user_ctrl_get_me(t_connection *c, t_http_message *m);
void mx_user_ctrl_get_my_rooms(t_connection *c, t_http_message *m);
void mx_user_ctrl_put_me(t_connection *c, t_http_message *m);
void mx_user_ctrl_delete_me(t_connection *c, t_http_message *m);

// services
t_list *mx_user_get_rooms(int user_id);
t_user *mx_user_get(int id);
t_user *mx_user_get_by_tag(char *tag);
t_user *mx_user_get_by_name(char *name);
t_user *mx_user_create(t_user_create_dto *dto);
t_user *mx_user_change_password(int id, char *password);
t_user *mx_user_put(int id, t_user_update_dto *dto);
t_user *mx_user_delete(int id);

// repositories
t_user *mx_user_repo_get(int id);
t_user *mx_user_repo_get_by_name(char *name);
t_user *mx_user_repo_get_by_tag(char *tag);
bool mx_user_repo_change_password(int id, char *password);
t_user_id mx_user_repo_create(t_user_create_dto *dto);
bool mx_user_repo_put(int id, t_user_update_dto *dto);
bool mx_user_repo_delete(int id);

// json
cJSON *mx_user_to_cjson(t_user *user);
t_string mx_user_stringify(t_user *user);
cJSON *mx_users_to_cjson(t_list *users);

bool mx_is_valid_tag(char *tag);
t_user *mx_sqlite_bind_columns_to_user(sqlite3_stmt *stmt, int from);

/*
/////////////////////////////////////////////
 User.favorites.
 Favorite { descriptions,
            room.descriptions,
            room.photo}
/////////////////////////////////////////////
*/
// structure constructors and destructors
t_favorite_room *mx_favorite_room_init(void);
void mx_favorite_room_free(t_favorite_room *r);

// controllers
void mx_user_ctrl_get_favorites(t_connection *c, t_http_message *m);
void mx_user_ctrl_add_favorite(t_connection *c, t_http_message *m);
void mx_user_ctrl_delete_favorite(t_connection *c, t_http_message *m);

// services
t_list *mx_user_get_favorites(int user_id);
t_favorite_room *mx_user_get_favorite(int room_id, int user_id);
t_favorite_room *mx_user_add_favorite(int user_id, int room_id);
t_favorite_room *mx_user_delete_favorite(int room_id, int user_id);

// repositories
t_favorite_room *mx_favorites_repo_get(int room_id, int user_id);
t_list *mx_favorites_repo_get_many(int user_id);
int mx_favorites_repo_create(int user_id, int room_id);
bool mx_favorites_repo_delete(int room_id, int user_id);

// json
cJSON *mx_favorite_to_cjson(t_favorite_room *fav);
t_string mx_favorite_stringify(t_favorite_room *fav);
cJSON *mx_favorites_to_cjson(t_list *members);
t_string mx_favorites_stringify(t_list *members);

t_favorite_room *mx_sqlite_bind_columns_to_favorite(sqlite3_stmt *stmt,
                                                    int from);

/* Stickers */
void mx_user_ctrl_get_stickers(t_connection *c, t_http_message *m);
void mx_user_ctrl_post_sticker(t_connection *c, t_http_message *m);
void mx_user_ctrl_delete_sticker(t_connection *c, t_http_message *m);

t_file *mx_sticker_delete(int user_id, int file_id);
t_file *mx_sticker_add(int user_id, int file_id);
t_file *mx_sticker_get(int user_id, int file_id);
t_list *mx_stickers_get(int user_id);

bool mx_sticker_repo_delete(int user_id, int file_id);
int mx_sticker_repo_create(int user_id, int file_id);
t_file *mx_sticker_repo_get(int user_id, int file_id);
t_list *mx_sticker_repo_get_many(int user_id);
#endif
