#ifndef UCHAT_STRUCTS_H
#define UCHAT_STRUCTS_H

#include "base.h"

typedef struct s_room_member t_room_member;
typedef struct s_read_message t_read_message;

typedef struct s_room t_room;
typedef struct s_message t_message;
typedef struct s_user t_user;

#define MX_MESSAGE_TYPE_UNKNOWN "unknown";
#define MESSAGE_TYPE_TEXT "text";
#define MESSAGE_TYPE_IMAGE "image";
#define MESSAGE_TYPE_AUDIO "audio";
#define MESSAGE_TYPE_VIDEO "video";
#define MESSAGE_TYPE_FILE "file";

struct s_room_member {
    int id;
    int room_id;
    int user_id;
    int created_at;
    int edited_at;

    t_user *user;
    t_room *room;
};

struct s_room {
    int id;
    char *name;
    bool is_direct_room;
    char *discription;
    int created_at;
    int edited_at;

    t_list *members;
    t_list *messages;
};

struct s_user {
    int id;
    char *name;
    char *tag;
    char *password_hash;
    char *photo_url;
    char *status;
    char *description;
    int created_at;
    int edited_at;

    t_list *messages;
    t_list *rooms;
    t_list *read_messages;
};

struct s_message {
    int id;
    int room_id;
    int author_id;
    int reply_id;
    bool is_edited;
    char *text;
    int created_at;
    int edited_at;

    t_list *read_messages;
    t_user *author;
    t_room *room;
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

#endif
