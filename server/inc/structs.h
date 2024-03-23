#ifndef UCHAT_STRUCTS_H
#define UCHAT_STRUCTS_H

#include "base.h"

typedef struct s_room_member t_room_member;
typedef struct s_read_message t_read_message;
typedef struct s_favorite_room t_favorite_room;
typedef struct s_room_pined_message t_pined_message;

typedef struct s_room t_room;
typedef struct s_message t_message;
typedef struct s_user t_user;
typedef struct s_file t_file;

struct s_room_member {
    int id;
    int room_id;
    int user_id;
    bool is_admin;
    int created_at;
    int edited_at;

    t_user *user;
    t_room *room;
};

struct s_favorite_room {
    int id;
    int room_id;
    int user_id;
    int created_at;
    int edited_at;

    t_user *user;
    t_room *room;
};
struct s_room_pined_message {
    int id;
    int message_id;
    int room_id;
    int created_at;
    int edited_at;

    t_room *room;
    t_message *message;
};

struct s_room {
    int id;
    char *name;
    char *type;
    char *description;
    int photo_id;
    int created_at;
    int edited_at;

    t_list *members;
    t_list *messages;
    t_list *pined_messages;
    t_file *photo;
};

struct s_user {
    int id;
    char *name;
    char *tag;
    char *password_hash;
    int photo_id;
    char *status;
    char *description;
    int created_at;
    int edited_at;

    t_list *messages;
    t_list *rooms;
    t_list *favorites;
    t_file *photo;
};

struct s_message {
    int id;
    int room_id;
    int author_id;
    int reply_id;
    char *text;
    int created_at;
    int edited_at;

    t_list *readed_by;
    t_user *author;
    t_room *room;
    t_message *reply;
    t_list *files;
};

struct s_read_message {
    int id;
    int user_id;
    int message_id;
    int created_at;
    int edited_at;

    t_user *user;
    t_message *message;
};

struct s_file {
    int id;
    char *name;
    char *url;
    int created_at;
    int edited_at;
};

#endif
