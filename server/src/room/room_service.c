#include "room.h"

t_room *mx_room_get(int id) {
    return mx_room_repo_get(id);
}

t_room *mx_room_create(t_room_create_dto *dto, int user_id) {
    int room_id = mx_room_repo_create(dto);
    if (room_id < 0) {
        return NULL;
    }
    t_room_member_create_dto *member =
        mx_room_member_create_dto_constructor(room_id, user_id, true);

    t_room_member *mem = mx_room_add_member(member);
    mx_delete_room_member_create_dto(member);

    if (!mem) {
        mx_room_repo_delete(room_id);
        return NULL;
    }

    return mx_room_get(room_id);
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
    bool ok = mx_room_repo_delete(id);

    if (!ok) {
        return NULL;
    }

    return room;
}
