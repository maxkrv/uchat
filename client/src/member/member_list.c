#include "client.h"

void render_room_members(int room_id) {
    GtkWidget *room_members_list = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "room_members_list"));

    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(room_members_list));

    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }

    t_response *members_response = mx_sdk_room_members_get(room_id);

    if (mx_is_response_error(members_response)) {
        g_print("Error: %s\n", mx_sdk_exception_get_message(members_response));
        mx_list_free((t_list **)&members_response->data,
                     (t_func_free)mx_room_member_free);
        mx_sdk_response_free(members_response, NULL);
        return;
    }


    for (t_list *current = members_response->data; current;
         current = current->next) {
        t_room_member *member = current->data;

        GtkWidget *member_button = gtk_button_new();
        GtkWidget *member_name = gtk_label_new(member->user->name);
        GtkWidget *member_info = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);

        gtk_box_pack_start(GTK_BOX(member_info), member_name, FALSE, FALSE, 2);
        gtk_widget_set_halign(member_name, GTK_ALIGN_START);
        gtk_container_add(GTK_CONTAINER(member_button), member_info);

        gtk_box_pack_start(GTK_BOX(room_members_list), member_button, FALSE, FALSE, 2);

        gtk_widget_show_all(room_members_list);
    }

    mx_sdk_response_free(members_response, NULL);
}

