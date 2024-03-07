#include "server.h"

t_room_member *mx_members_repo_get(int id) {
    return mx_init_room_member();
}

t_room_member *mx_members_repo_get_by(int user_id, int room_id) {
    return mx_init_room_member();
}

int mx_members_repo_create(int user_id, int room_id, bool is_admin) {
    return 1;
}

bool mx_members_repo_update(int id, bool is_admin) {
    return true;
}

t_list *mx_members_repo_get_many_rooms(int user_id) {
    t_list *r;

    mx_push_back(&r, mx_room_repo_get(1));

    return r;
}

t_list *mx_members_repo_get_many(int room_id) {
    t_list *r;

    mx_push_back(&r, mx_members_repo_get(1));

    return r;
}

bool mx_members_repo_delete(int id) {
    return true;
}
