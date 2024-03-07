#include "room.h"

bool mx_is_room_admin(int room_id, int user_id) {
    t_room_member *mem = mx_members_repo_get_by(room_id, user_id);
    bool is_admin = mem && mem->is_admin;

    mx_delete_room_member(mem);

    return is_admin;
}
