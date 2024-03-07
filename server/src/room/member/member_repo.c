#include "server.h"

t_room_member *mx_members_repo_get(int id) {
    return NULL;
}

t_room_member *mx_members_repo_get_by(int user_id, int room_id) {
    return NULL;
}

int mx_members_repo_create(int user_id, int room_id, bool is_admin) {
    return 1;
}

bool mx_members_repo_update(int id, bool is_admin) {
    return true;
}

t_list *mx_members_repo_get_many_rooms(int user_id) {
    return NULL;
}
t_list *mx_members_repo_get_many(int room_id) {
    return NULL;
}

bool mx_members_repo_delete(int id) {
    return true;
}
