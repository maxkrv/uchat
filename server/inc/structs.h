#ifndef UCHAT_STRUCTS_H
#define UCHAT_STRUCTS_H

#include "base.h"

typedef struct {
} t_room;


typedef struct {
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
} t_user;

typedef struct {
} t_message;

typedef struct {
    int id;
    int user_id;
    int message_id;

    t_user *user;
    t_message *message;
} t_read_message;

#endif
