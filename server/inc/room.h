#ifndef UCHAT_ROOM
#define UCHAT_ROOM

#include "base.h"
#include "utils.h"
#include "shared.h"

cJSON *mx_room_to_cjson(t_room *room);

cJSON *mx_rooms_to_cjson(t_list *rooms);

t_string mx_room_stringify(t_room *room);

#endif
