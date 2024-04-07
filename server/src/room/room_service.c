#include "room.h"

t_room *mx_room_get(int id) {
    return mx_room_repo_get(id);
}

t_room *mx_room_create(t_room_create_dto *dto, int user_id) {
    int room_id = mx_room_repo_create(dto);
    if (room_id < 0) {
        return NULL;
    }
    int member_id = mx_members_repo_create(user_id, room_id, true);

    if (member_id <= 0) {
        mx_room_repo_delete(room_id);
        return NULL;
    }

    return mx_room_get(room_id);
}

t_room *mx_room_create_direct(t_room_create_dto *dto, int first_id,
                              int second_id) {
    if (mx_room_repo_direct_exist(first_id, second_id)) {
        return NULL;
    }

    return mx_room_create(dto, first_id);
}

t_room *mx_room_put(int id, t_room_create_dto *dto) {
    bool ok = mx_room_repo_put(id, dto);

    if (!ok) {
        return NULL;
    }

    return mx_room_repo_get(id);
}

t_room *mx_room_delete(int id) {
    t_room *room = mx_room_repo_get(id);

    if (!room || mx_strcmp(room->type, MX_ROOM_TYPE_NOTES) == 0) {
        mx_room_free(room);
        return NULL;
    }
    bool ok = mx_room_repo_delete(id);

    if (!ok) {
        mx_room_free(room);
        return NULL;
    }

    return room;
}
