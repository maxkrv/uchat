#include "server.h"

t_list *mx_room_get_members(int room_id) {
    return mx_members_repo_get_many(room_id);
}

t_room_member *mx_room_get_member(int id) {
    return mx_members_repo_get(id);
}

t_room_member *mx_room_add_member(t_room_member_create_dto *dto) {
    t_room *room = mx_room_get(dto->room_id);

    if (!room || mx_strcmp(room->type, MX_ROOM_TYPE_NOTES) == 0 ||
        mx_strcmp(room->type, MX_ROOM_TYPE_DIRECT) == 0) {
        mx_room_free(room);
        return NULL;
    }
    mx_room_free(room);

    int id = mx_members_repo_create(dto->user_id, dto->room_id, dto->is_admin);

    if (id < 0) {
        return NULL;
    }

    return mx_room_get_member(id);
}

t_room_member *mx_room_update_member(int id, t_room_member_update_dto *dto) {
    t_room_member *member = mx_room_get_member(id);
    member->room = mx_room_get(member->room_id);

    if (!member || !member->room ||
        mx_strcmp(member->room->type, MX_ROOM_TYPE_NOTES) == 0 ||
        mx_strcmp(member->room->type, MX_ROOM_TYPE_DIRECT) == 0) {
        mx_room_member_free(member);
        return NULL;
    }
    mx_room_member_free(member);

    bool ok = mx_members_repo_update(id, dto->is_admin);

    if (!ok) {
        return NULL;
    }

    return mx_room_get_member(id);
}

t_room_member *mx_room_delete_member(int id) {
    t_room_member *member = mx_room_get_member(id);
    member->room = mx_room_get(member->room_id);

    if (!member || !member->room ||
        mx_strcmp(member->room->type, MX_ROOM_TYPE_NOTES) == 0) {
        mx_room_member_free(member);
        return NULL;
    }

    bool ok = mx_members_repo_delete(member->id);

    if (!ok) {
        mx_room_member_free(member);
        return NULL;
    }
    if (mx_members_repo_count(member->room_id) == 0) {
        mx_room_repo_delete(member->room_id);
    }
    return member;
}
