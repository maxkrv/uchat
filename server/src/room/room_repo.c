#include "room.h"

t_room *mx_room_repo_get(int id) {
    return mx_room_init();
}

int mx_room_repo_create(t_room_create_dto *dto) {
    return 1;
}

bool mx_room_repo_put(int id, t_room_create_dto *dto) {
    return true;
}

bool mx_room_repo_delete(int id) {
    return true;
}
