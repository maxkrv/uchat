#ifndef SDK_ENTITY_H
#define SDK_ENTITY_H

#include "sdk/base.h"
#include "sdk/utils.h"
#include "sdk/structs.h"

t_file *mx_file_init(void);
void mx_file_free(t_file *u);

t_message *mx_message_init(void);
void mx_message_free(t_message *u);

t_read_message *mx_read_message_init(void);
void mx_read_message_free(t_read_message *u);

t_room *mx_room_init(void);
void mx_room_free(t_room *u);

t_pined_message *mx_room_pined_init(void);
void mx_room_pined_free(t_pined_message *m);

t_room_member *mx_room_member_init(void);
void mx_room_member_free(t_room_member *m);

t_user *mx_user_init(void);
void mx_user_free(t_user *u);

t_favorite_room *mx_favorite_room_init(void);
void mx_favorite_room_free(t_favorite_room *r);
#endif
