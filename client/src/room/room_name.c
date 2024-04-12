#include "client.h"

void set_room_name(t_room *room, GtkWidget *room_user_name) {
    t_response *response_user = mx_sdk_user_get_me();

    if (mx_is_response_error(response_user)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response_user));
        mx_sdk_response_free(response_user, (t_func_free)mx_user_free);
        return;
    }
    t_response *response_members = mx_sdk_room_members_get(room->id);
    if (mx_is_response_error(response_members)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(response_members));
        mx_list_free((t_list **)&response_members->data,
                     (t_func_free)mx_room_member_free);
        mx_sdk_response_free(response_members, NULL);
        return;
    }
    t_user *user = response_user->data;
    if (g_strcmp0(room->type, "direct") == 0) {
        t_user *other_user = NULL;

        for (t_list *iter = response_members->data; iter != NULL;
             iter = iter->next) {
            t_room_member *member = (t_room_member *)iter->data;

            if (member->user->id != user->id) {
                other_user = member->user;

                break;
            }
        }

        if (other_user != NULL) {
            gtk_label_set_text(GTK_LABEL(room_user_name), other_user->name);

        } else {
            g_print("Error: Other user not found.\n");
        }

    } else {

        gtk_label_set_text(GTK_LABEL(room_user_name), room->name);
    }

    mx_sdk_response_free(response_user, (t_func_free)mx_user_free);

    mx_list_free((t_list **)&response_members->data,
                 (t_func_free)mx_room_member_free);

    mx_sdk_response_free(response_members, NULL);
}
