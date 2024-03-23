#include "room.h"

bool mx_is_room_admin(int room_id, int user_id) {
    t_room_member *mem = mx_members_repo_get_by(user_id, room_id);
    bool is_admin = mem && mem->is_admin;

    mx_room_member_free(mem);

    return is_admin;
}
