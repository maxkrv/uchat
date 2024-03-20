#ifndef SDK_DTO_H
#define SDK_DTO_H

#include "sdk/base.h"
#include "sdk/utils.h"
#include "sdk/structs.h"

t_message_create_dto *mx_message_create_dto_init(void);
void mx_message_create_dto_free(t_message_create_dto *dto);
t_string mx_message_create_dto_stringify(t_message_create_dto *dto);

t_room_create_dto *mx_room_create_dto_init(void);
void mx_room_create_dto_free(t_room_create_dto *d);
t_string mx_room_create_dto_stringify(t_room_create_dto *dto);

t_string mx_room_member_create_stringify(int room_id, int user_id,
                                         bool is_admin);
t_string mx_room_member_update_stringify(bool is_admin);

t_user_create_dto *mx_user_create_dto_init(void);
void mx_user_create_dto_free(t_user_create_dto *dto);
t_string mx_user_create_dto_stringify(t_user_create_dto *dto);

t_user_update_dto *mx_user_update_dto_init(void);
void mx_user_update_dto_free(t_user_update_dto *dto);
t_string mx_user_update_dto_stringify(t_user_update_dto *dto);

#endif
