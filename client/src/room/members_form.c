#include "client.h"

static char *new_member_username = NULL;
static int new_room_id = -1;

// static void
// set_members_scrollbar_to_bottom(GtkWidget *scrolled_members_window) {
//     GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(
//         GTK_SCROLLED_WINDOW(scrolled_members_window));
//     double upper = gtk_adjustment_get_upper(adjustment);
//     double page_size = gtk_adjustment_get_page_size(adjustment);
//     gtk_adjustment_set_value(adjustment, upper - page_size);
// }

static void on_member_entry_changed(GtkEntry *entry) {
    const gchar *text = gtk_entry_get_text(entry);
    new_member_username = g_strdup(text);
}

static void on_add_member_submit_clicked(GtkButton *button) {
    if (new_member_username == NULL) {
        return;
    }

    t_response *user_response = mx_sdk_user_find_by_name(new_member_username);

    if (mx_is_response_error(user_response)) {
        t_string message = mx_sdk_exception_get_message(user_response);

        GtkWidget *member_error_message = GTK_WIDGET(
            gtk_builder_get_object(global_builder, "member_error_message"));

        gtk_label_set_text(GTK_LABEL(member_error_message), message);
        mx_sdk_response_free(user_response, (t_func_free)mx_user_free);

        return;
    }

    t_user *user = user_response->data;
    t_response *response = mx_sdk_room_member_post(new_room_id, user->id, false);

    if (mx_is_response_error(response)) {
        t_string message = mx_sdk_exception_get_message(response);

        GtkWidget *member_error_message = GTK_WIDGET(
            gtk_builder_get_object(global_builder, "member_error_message"));

        gtk_label_set_text(GTK_LABEL(member_error_message), message);
    }

    mx_sdk_response_free(response, (t_func_free)mx_room_member_free);

    render_room_members(new_room_id);

    GtkWidget *add_member_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "add_member_entry"));

    gtk_entry_set_text(GTK_ENTRY(add_member_entry), "");

    (void)button;
}

void init_members_form(int room_id) {
    GtkWidget *add_member_entry =
        GTK_WIDGET(gtk_builder_get_object(global_builder, "add_member_entry"));
    GtkWidget *add_member_submit = GTK_WIDGET(
        gtk_builder_get_object(global_builder, "add_member_submit"));

    g_signal_connect(add_member_entry, "changed",
                     G_CALLBACK(on_member_entry_changed), NULL);
    g_signal_connect(add_member_submit, "clicked",
                     G_CALLBACK(on_add_member_submit_clicked), NULL);

    new_room_id = room_id;
}

