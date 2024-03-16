#include "server.h"

bool mx_is_user_member_of(int room_id, int user_id) {
    t_room_member *member = mx_members_repo_get_by(room_id, user_id);
    bool is_member = !!member;

    mx_room_member_free(member);
    return is_member;
}
