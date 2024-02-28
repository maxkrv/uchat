#include "room.h"

t_room *mx_room_get(int id) {
    return mx_room_repo_get(id);
}

t_room *mx_room_create(t_room_create_dto *dto) {
    bool ok = mx_room_repo_create(dto);
    if (!ok) {
        return NULL;
    }

    return mx_room_repo_get(1);
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
